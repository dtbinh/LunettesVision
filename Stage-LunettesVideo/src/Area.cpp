#include "Area.h"
#include <math.h> 
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
/// THREAD AND HDR
//////////////////////////////////////////////////////////////////////////////

int weight_func(const int Z)
{
	if (Z <= 128)   return Z;
	if (Z>128 && Z<250)   return (256 - Z);
	else return 6;
}

int create_EXR_channels_from_LDR_image(vector<Mat>images, vector<float> Te, const Mat response, float* EXRDataPtr)
{
	clock_t deb, fin, diff;
	deb = clock();
	float numerateur[3] = { 0.0, 0.0, 0.0 }, denominateur[3] = { 0.0, 0.0, 0.0 };
	float *dataf = EXRDataPtr;
	float *Mdataf;
	Mdataf = dataf;
	
	int i, j, k, m, cpt = 0;
	for (j = 0; j<images[0].rows; j++)
	{
		for (i = 0; i<images[0].cols; i++)//double boucle parcourant tous les pixels de l'image d'entrée
		{
			if (true)
			{
				numerateur[0] = 0.0;
				numerateur[1] = 0.0;
				numerateur[2] = 0.0;
				denominateur[0] = 0.0;
				denominateur[1] = 0.0;
				denominateur[2] = 0.0;
				for (k = 0; k<images.size(); k++)//boucle traitant les N images d'entrée
				{
					for (m = 0; m<3; m++)//boucle traitant les 3 canaux
					{
						numerateur[m] += weight_func(images[k].at<Vec3b>(j, i)[m])*(response.at<float>((int)images[k].at<Vec3b>(j, i)[m]) - log(Te[k]));
						denominateur[m] += weight_func(images[k].at<Vec3b>(j, i)[m]);
					}
				}

				if (denominateur[0] == 0.0)
					dataf[2] = 0;
				else
					dataf[2] = exp(numerateur[0] / denominateur[0]);

				if (denominateur[1] == 0.0)
					dataf[1] = 0;
				else
					dataf[1] = exp(numerateur[1] / denominateur[1]);

				if (denominateur[2] == 0.0)
					dataf[0] = 0;
				else
					dataf[0] = exp(numerateur[2] / denominateur[2]);
				dataf += 3;
			}

			else
			{
				k = images.size() / 2;
				for (m = 0; m<3; m++)//boucle traitant les 3 canaux
				{
					numerateur[m] = weight_func(images[k].at<Vec3b>(j, i)[m]) - log(Te[k]);
				}
				dataf[0] = exp(numerateur[2]);
				dataf[1] = exp(numerateur[1]);
				dataf[2] = exp(numerateur[0]);
				dataf += 3;
			}
		}
	}

	EXRDataPtr = Mdataf;
	fin = clock();
	printf("create_HDR : %d ms\n", (int)(fin - deb));
	return 0;
}//*/


cv::Mat Area::HDR(std::vector<cv::Mat>& images, std::vector<float>& times){

	Mat hdr(camera->height, camera->width, CV_8UC3);
	/*cv::Ptr<cv::MergeDebevec> merge_debevec = createMergeDebevec();
	cv::Ptr<cv::TonemapDurand> tonemap = createTonemapDurand(2.2);
	merge_debevec->process(images, hdr, times, camera->getResponse());
	tonemap->process(hdr, ldr);
	ldr = ldr * 255;
	imwrite("ldrobtenu32f.png", ldr);
	Mat result;
	ldr.convertTo(result, CV_8UC3);
	imwrite("ldrobtenu8u.png", result);*/

	return hdr;
}

void Area::setHdrThreadFunction(){
	cv::Mat areaFrame(camera->height, camera->width, CV_8UC3);
	int numSequence = -1;
	UEYEIMAGEINFO ImageInfo;
	while(imagesHdr.size() < 2){ //on veut deux images pour HDR
		getCamFrame(areaFrame, numSequence);
		imagesHdr.push_back(areaFrame.clone()); ///Clone très important, c'est une nouvelle mat qu'on veut
	}

	timesExpo.push_back(0.005);
	timesExpo.push_back(0.015);
	matHDR = HDR(imagesHdr, timesExpo);
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
	Param.dblExposure = 5;
	Param.s32DetachImageParameters = 1; //changes of Params does not affect others AOI
	is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
	
	Param.s32AOIIndex = IS_AOI_SEQUENCE_INDEX_AOI_2;
	Param.s32NumberOfCycleRepetitions = 1;
	Param.dblExposure = 15;
	Param.s32DetachImageParameters = 1; //changes of Params does not affect others AOI
	is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
	
	nMask = IS_AOI_SEQUENCE_INDEX_AOI_1 | IS_AOI_SEQUENCE_INDEX_AOI_2;
	// enable sequence mode
    is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask));
}

void Area::disableSequenceAOI(){
	double pParam = 10;
	INT nMask = 0;
	is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask)); //Stop Sequence HDR
	is_Exposure(camera->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&pParam, sizeof(pParam));
}

void  Area::getCamFrame( cv::Mat& frame, int& numSequence )
{
	UEYEIMAGEINFO ImageInfo;
	int nRet, pbo = 0;
	if (numSequence == -1){
		do {
			is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
			is_LockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
			nRet = is_IsVideoFinish(camera->hCam, &pbo);
			nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId, &ImageInfo, sizeof(ImageInfo));
		} while ((pbo != IS_VIDEO_FINISH));
		//cout << "pbo = : " << pbo << endl;
		numSequence = ImageInfo.wAOIIndex;
	}
	else {
		do {
			is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
			nRet = is_IsVideoFinish(camera->hCam, &pbo);
			nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId, &ImageInfo, sizeof(ImageInfo));
		} while (pbo != IS_VIDEO_FINISH || ImageInfo.wAOIIndex == numSequence );
		//cout << "pbo = : " << pbo << endl;
		is_LockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
	}
	frame = cv::Mat(camera->height, camera->width, CV_8UC3, camera->m_pcImageMemory, camera->width*(camera->m_bitsPerPixel / 8)); // last param = number of bytes by cols
	is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
}

/////NOT USED ANYMORE /////
void Area::startThread()
{
	// If the area is a color area, we dont need a thread !
	if(type == CAMERA) {
		needClose = false;
		currentFrame.create(camera->getSize().height,camera->getSize().width,CV_16UC3);
		invalidate();	// If needed, compute the remap matrix   
	}
}

void Area::stopThread() {
	if(remapThread.joinable()) {
		needClose = true;
		remapThread.join();
	}
}

/////////////////////

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

