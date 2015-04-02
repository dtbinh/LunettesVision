#include "Area.h"

//////////////////////////////////////////////////////////////////////////////
/// CONSTRUCTEUR - DESTRUCTEUR
//////////////////////////////////////////////////////////////////////////////
using namespace cv;
using namespace std;


Area::Area(void)
{
	matrix = NULL;
	camera = NULL;
	needClose = false;
	baseCrop = false;
	needCrop = false;
	baseRemap = false;
	needRemap = false;
	hidden = false;
	needHdr = false;
	empty = true;
	
}

Area::~Area(void)
{
}

//////////////////////////////////////////////////////////////////////////////
/// THREAD
//////////////////////////////////////////////////////////////////////////////
void Area::initHDR(){
	calibrate =  createCalibrateRobertson();
	merge_debevec = createMergeDebevec();
	tonemap = createTonemap();
	cout <<"create calibrate and merge ptrs ";
}

Mat Area::HDR(std::vector<cv::Mat>& images, std::vector<double>& times){

	Mat response;
	imwrite("image1.png", images[0]);
	imwrite("image2.png", images[1]);
    calibrate->process(images, response, times);
	cout <<"done calibrate" <<endl;
	Mat hdr;
    merge_debevec->process(images, hdr, times, response);
    cout <<"done merge" <<endl;

    cv::Mat ldr;
    
    tonemap->process(hdr, ldr);
	//imwrite("hdr.hdr", hdr);
	ldr = ldr * 255;
	return ldr;
 
}


void remapThreadFunction(Area* r){
	cv::cuda::GpuMat srcGpu;
	INT nMemID = 0;
	char *pBuffer = NULL;
	int nRet, count = 0;
	while(!r->needClose) {
		
		nRet = is_LockSeqBuf(r->camera->hCam, IS_IGNORE_PARAMETER, r->camera->m_pcImageMemory);
		srcGpu = (r->getCamFrame());
		nRet = is_UnlockSeqBuf(r->camera->hCam, IS_IGNORE_PARAMETER, r->camera->m_pcImageMemory);

		cv::cuda::GpuMat dstGpu(r->getDisplaySize(),srcGpu.type());	
				
		if(r->needRemap && r->matrix!=NULL) {
			r->matrixMutex.lock();
			cv::cuda::remap(srcGpu,dstGpu,r->matrix->getGpuXmat(),r->matrix->getGpuYmat(),cv::INTER_LINEAR,cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
			r->matrixMutex.unlock();
		} else {
			cv::cuda::resize(srcGpu,dstGpu,r->getDisplaySize());
		}
		r->frameMutex.lock();
		dstGpu.copyTo((r->currentFrame));
		r->empty = false;
		r->frameMutex.unlock();
		
	}
	r->camera->exitCamera();
}

void Area::setHdrThreadFunction(){
	int numSequence = -1;
	while(imagesHdr.size() < 2){ //onveut deux images pour HDR
		UEYEIMAGEINFO ImageInfo;
		INT nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId, &ImageInfo, sizeof(ImageInfo));
		/*AOI_SEQUENCE_PARAMS Param;
		is_AOI(r->camera->hCam, IS_AOI_SEQUENCE_GET_PARAMS, (void*)&Param, sizeof(Param));*/
		cv::Mat areaFrame(camera->height,camera->width,CV_8UC3, camera->m_pcImageMemory, camera->width*(camera->m_bitsPerPixel/8)); // last param = number of bytes by cols
		if(numSequence == -1){
			numSequence= ImageInfo.wAOIIndex;
			imagesHdr.push_back(areaFrame);
			timesExpo.push_back(1/8);
			cout<<"temps d'expo : "<<timesExpo[0]<<endl;
		}
		else if(numSequence != ImageInfo.wAOIIndex){
			cout<<"Numeros de sequence = "<<numSequence<<" et "<< ImageInfo.wAOIIndex<<endl;
			imagesHdr.push_back(areaFrame);
			timesExpo.push_back(1/12);
			cout<<"temps d'expo : "<<timesExpo[0]<<" "<<timesExpo[1]<<endl;
			imwrite("imagesHdr1.png", imagesHdr[0]);
			imwrite("imagesHdr2.png", imagesHdr[1]);
			cout<<"début hdr"<<endl;
			matHDR = HDR(imagesHdr, timesExpo);
			cout<<"upload"<<endl;
			imwrite("HDR.png",matHDR);
		}
	}
	imagesHdr.clear();
	timesExpo.clear();
}

void Area::initSequenceAOI(){
	
	INT nMask = 0;
	
	//Parameters Initialization
	AOI_SEQUENCE_PARAMS Param;
	
	// Set parameters of AOI 1
	Param.s32AOIIndex = IS_AOI_SEQUENCE_INDEX_AOI_1;
	Param.s32NumberOfCycleRepetitions = 1;
	Param.s32X = 0;
	Param.s32Y = 0;
	Param.dblExposure = 8;
	Param.s32DetachImageParameters = 1; //changes of Params does not affect others AOI
	is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
	
	Param.s32AOIIndex = IS_AOI_SEQUENCE_INDEX_AOI_2;
	Param.s32NumberOfCycleRepetitions = 1;
	Param.dblExposure = 12;
	Param.s32DetachImageParameters = 1; //changes of Params does not affect others AOI
	is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
	
	nMask = IS_AOI_SEQUENCE_INDEX_AOI_1 |
                IS_AOI_SEQUENCE_INDEX_AOI_2;

    // enable sequence mode
    is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask));
	
}

void Area::startThread()
{
	// If the area is a color area, we dont need a thread !
	if(type == CAMERA) {
		needClose = false;
		currentFrame.create(camera->getSize().height,camera->getSize().width,CV_8UC3);
		invalidate();	// If needed, compute the remap matrix
		remapThread = std::thread(remapThreadFunction, this);          
		remapThread.detach();
	}
}

void Area::stopThread() {
	if(remapThread.joinable()) {
		needClose = true;
		remapThread.join();
	}
}

//////////////////////////////////////////////////////////////////////////////
/// AREA PROPERTIES
//////////////////////////////////////////////////////////////////////////////

void Area::invalidate() {
	if(needRemap) {
		if(matrix == NULL) // Compute matrices for the remap function
			matrix = new Matrix(camera->getSize(),cv::Size(displayZone.width,displayZone.height));

		///// INPUT SIZE FOR MATRICES /////
		if(needCrop)
			matrix->setInputSize(cv::Size(cameraROI.width, cameraROI.height));
		else
			matrix->setInputSize(cv::Size(camera->getSize()));

		///// REMAP /////
		matrix->setRemapZone(centralZone, zoom);
		matrixMutex.lock();
		matrix->invalidate();
		matrixMutex.unlock();

	}
}

void Area::switchCrop() {
	needCrop = (!needCrop && baseCrop);
}

void Area::switchHdr() {
	needHdr = !needHdr;
}

void Area::switchRemap() {
	needRemap = (!needRemap && baseRemap);
	if(needRemap && matrix == NULL)
		invalidate();
}

void Area::HideAndShow()
{
	//Hide the video stream
	hidden = !hidden;
}



//////////////////////////////////////////////////////////////////////////////
/// GET
//////////////////////////////////////////////////////////////////////////////

cv::Size Area::getDisplaySize() {
	cv::Size s = cv::Size(displayZone.width,displayZone.height);
	return s;
}

cv::Rect Area::getDisplayRect() {
	return displayZone;
}

cv::Rect Area::getCameraCropRect()
{
	if(needCrop) return displayZone;
	else {
		int x = displayZone.x +  (cameraROI.x * displayZone.width) /camera->getSize().width;
		int y =  displayZone.y + (cameraROI.y * displayZone.height) /camera->getSize().height;
		int width = (cameraROI.width * displayZone.width) /camera->getSize().width;
		int height = (cameraROI.height * displayZone.height) /camera->getSize().height;
		return cv::Rect(x,y,width,height);
	}
}

cv::Rect Area::getCentralZoneRect()
{
	int x,y,width,height;

	x =  centralZone.x + displayZone.x;
	y =   centralZone.y + displayZone.y;
	width = centralZone.width;
	height = centralZone.height;

	if(!needRemap) {
		x += (width - (double)(width*zoom) ) / 2;
		y += (height - (double)(height*zoom) ) / 2;
		width *= (double)zoom;
		height *= (double)zoom;
	}

	return cv::Rect(x,y,width,height);
}

cv::Rect& Area::getRect(AreaType t)
{
	switch(t)
	{
	case REMAP_ZONE:
		return centralZone;
		break;
	case CROP_ZONE:
		return cameraROI;
		break;
	case DISPLAY_ZONE:
	default:
		return displayZone;
		break;
	}
}

cv::cuda::GpuMat  Area::getCamFrame(char * pBuffer)
{
	
	cv::Mat frame(camera->height,camera->width,CV_8UC3,pBuffer, camera->width*(camera->m_bitsPerPixel/8)); // last param = number of bytes by cols
	cv::cuda::GpuMat frameGpu;
	frameGpu.upload(frame);
	if(needCrop) {
		cv::cuda::GpuMat frameRect(frameGpu, cameraROI);
		return frameRect;
	}
	else 
		return frameGpu;
}
cv::cuda::GpuMat  Area::getCamFrame()
{
	cv::Mat frame(camera->height,camera->width,CV_8UC3, camera->m_pcImageMemory, camera->width*(camera->m_bitsPerPixel/8)); // last param = number of bytes by cols
	cv::cuda::GpuMat frameGpu;
	
	frameGpu.upload(frame);
	if(needCrop) {
		cv::cuda::GpuMat frameRect(frameGpu, cameraROI);
		return frameRect;
	}
	else 
		return frameGpu;
}

int Area::getWidth(AreaType t)
{
	cv::Rect r = getRect(t);
	return r.width;
}

int Area::getHeight(AreaType t)
{
	cv::Rect r = getRect(t);
	return r.height;
}

int Area::getX(AreaType t)
{
	cv::Rect r = getRect(t);
	return r.x;
}

int Area::getY(AreaType t)
{
	cv::Rect r = getRect(t);
	return r.y;
}

//////////////////////////////////////////////////////////////////////////////
/// SET
//////////////////////////////////////////////////////////////////////////////

void Area::setWidth(AreaType t, int i)
{
	switch(t) {
	case CROP_ZONE:
		if(cameraROI.x + i <= camera->getSize().width && cameraROI.x + i >= 0) {
			cameraROI.width = i;
			invalidate();
		}
		break;
	case REMAP_ZONE:
		if(centralZone.x + i <= displayZone.width && centralZone.x + i >= 0) {
			centralZone.width = i;
			invalidate();
		}
		break;
	}
}

void Area::setHeight(AreaType t, int i)
{
	switch(t) {
	case CROP_ZONE:
		if(cameraROI.y + i <= camera->getSize().height && cameraROI.y + i >= 0) {
			cameraROI.height = i;
			invalidate();
		}
		break;
	case REMAP_ZONE:
		if(centralZone.y + i <= displayZone.height && centralZone.y + i >= 0) {
			centralZone.height = i;
			invalidate();
		}
		break;
	}
}

void Area::setX(AreaType t, int i)
{
	switch(t) {
	case CROP_ZONE:
		if(cameraROI.width + i <= camera->getSize().width && i >= 0) {
			cameraROI.x = i;
			invalidate();
		}
		break;
	case REMAP_ZONE:
		if(centralZone.width + i <= displayZone.width && i >= 0) {
			centralZone.x = i;
			invalidate();
		}
		break;
	}
}

void Area::setY(AreaType t, int i)
{
	switch(t) {
	case CROP_ZONE:
		if(cameraROI.height + i <= camera->getSize().height && i >= 0) {
			cameraROI.y = i;
			invalidate();
		}
		break;
	case REMAP_ZONE:
		if(centralZone.height + i <= displayZone.height && i >= 0) {
			centralZone.y = i;
			invalidate();
		}
		break;
	}
}

