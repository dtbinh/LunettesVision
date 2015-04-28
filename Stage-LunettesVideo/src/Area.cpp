#include "Area.h"
#include <math.h> 
//////////////////////////////////////////////////////////////////////////////
/// CONSTRUCTEUR - DESTRUCTEUR
//////////////////////////////////////////////////////////////////////////////
using namespace cv;
using namespace std;

//Preuve que OPENCV marche !
void loadExposure(String path, vector<Mat>& images, vector<float>& times){
	path = path + std::string("/"); /// A CHANGER EN CHEMIN RELATIF
	ifstream list_file((path + "expositionTimes.txt").c_str());
	if (list_file){
		string name;
		float val;
		while (list_file >> name >> val) {
			//cout << "load image " << name << endl;
			Mat img = imread(path + name);
			images.push_back(img);
			times.push_back(val);
		}
	}
	else {
		cout << "Error load images" << endl;
	}
	list_file.close();
}


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

cv::Mat create_EXR_channels_from_LDR_image(vector<Mat> images, vector<float> Te, const Mat& response)
{
	clock_t deb, fin, diff;
	deb = clock();
	float numerateur[3] = { 0.0, 0.0, 0.0 }, denominateur[3] = { 0.0, 0.0, 0.0 };
	Mat hdr(images[0].rows, images[0].cols, CV_16UC3);
	
	int i, j, k, m, cpt = 0;
	for (j = 0; j<images[0].rows; j++)
	{
		for (i = 0; i<images[0].cols; i++)//double boucle parcourant tous les pixels de l'image d'entrée
		{
			//if (true)
			//{
				numerateur[0] = 0.0;
				numerateur[1] = 0.0;
				numerateur[2] = 0.0;
				denominateur[0] = 0.0;
				denominateur[1] = 0.0;
				denominateur[2] = 0.0;
				for (k = 0; k < images.size(); k++)//boucle traitant les N images d'entrée
				{
					for (m = 0; m<3; m++)//boucle traitant les 3 canaux
					{
						numerateur[m] += weight_func(images[k].at<Vec3f>(j, i)[m])*(response.at<float>((int)images[k].at<Vec3b>(j, i)[m]) - log(Te[k]));
						denominateur[m] += weight_func(images[k].at<Vec3f>(j, i)[m]);
					}
				}
				if (denominateur[0] == 0.0)
					hdr.at<Vec3f>(j,i)[2] = 0;
				else
					hdr.at<Vec3f>(j, i)[2] = exp(numerateur[0] / denominateur[0]);

				if (denominateur[1] == 0.0)
					hdr.at<Vec3f>(j, i)[1] = 0;
				else
					hdr.at<Vec3f>(j, i)[1] = exp(numerateur[1] / denominateur[1]);

				if (denominateur[2] == 0.0)
					hdr.at<Vec3f>(j, i)[0] = 0;
				else
					hdr.at<Vec3f>(j, i)[0] = exp(numerateur[2] / denominateur[2]);
			/*}

			else
			{
				k = images.size() / 2;
				for (m = 0; m<3; m++)//boucle traitant les 3 canaux
				{
					numerateur[m] = weight_func(images[k].at<Vec3b>(j, i)[m]) - log(Te[k]);
				}
				hdr.at<Vec3f>(j, i)[0] = exp(numerateur[2]);
				hdr.at<Vec3f>(j, i)[1] = exp(numerateur[1]);
				hdr.at<Vec3f>(j, i)[2] = exp(numerateur[0]);
			}*/
		}
		cout << j << " " << i << endl;
	}
	cout << "fin" << endl;
	hdr.convertTo(hdr, CV_8UC3);
	fin = clock();
	printf("create_HDR : %d ms\n", (int)(fin - deb));
	return hdr;
}


cv::Mat Area::HDR(const std::vector<cv::Mat>& images, const std::vector<float>& times){

	Mat hdr, ldr;
	cv::Mat result(camera->height, camera->width, CV_8UC3);
	//cout << "size vectore images = " << images.size() << endl;
	//cout << "size vectore times = " << times.size() << endl;
	
	/*for (int i = 0; i < images.size(); i++){
		std::ostringstream oss;
		oss << "results/image" << i << ".png";
		imwrite(oss.str(), images[i]);
		cout << images[i].cols << " et " << times[i] << endl;
	}*/
	Mat image0, image1;
	cvtColor(images[0], image0, CV_BGR2GRAY);
	cvtColor(images[1], image1, CV_BGR2GRAY);
	for (int j = 0; j < image0.rows; j++)
	{
		for (int i = 0; i < image0.cols; i++)//double boucle parcourant tous les pixels de l'image d'entrée
		{
			if (image0.at<uchar>(j, i) > image1.at<uchar>(j, i)){
				if (image0.at<uchar>(j, i) > 200){
					result.at<Vec3b>(j, i) = images[1].at<Vec3b>(j, i);
				}
				else{
					result.at<Vec3b>(j, i) = images[0].at<Vec3b>(j, i);
				}
			}
			else {
				if (image1.at<uchar>(j, i) > 200){
					result.at<Vec3b>(j, i) = images[0].at<Vec3b>(j, i);
				}
				else{
					result.at<Vec3b>(j, i) = images[1].at<Vec3b>(j, i);
				}
			}
		}
	}
	///VRAI HDR
	/*Mat response = (camera->getResponse()).clone();
	//camera->merge_debevec->process(images, hdr, times, response);
	hdr = create_EXR_channels_from_LDR_image(images, times, response);
	camera->tonemap->process(hdr, ldr);
	ldr.convertTo(result, CV_8UC3, 255);*/
	//imwrite("results/imagehdr.png", result);
	///////////

	return result;
}


void Area::setHdrThreadFunction(){
	cv::Mat areaFrame(camera->height, camera->width, CV_8UC3);
	int numSequence = -1;
	double time;
	UEYEIMAGEINFO ImageInfo;
	while(imagesHdr.size() < 2){ //on veut deux images pour HDR
		getCamFrame(areaFrame, numSequence, time);
		imagesHdr.push_back(areaFrame.clone()); ///Clone très important, c'est une nouvelle mat qu'on veut
		timesExpo.push_back(time);
	}
	/*String path = "res/HDR_calib-set";
	loadExposure(path, imagesHdr, timesExpo);*/
	HDR(imagesHdr, timesExpo).copyTo(matHDR);
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
	
	nMask = IS_AOI_SEQUENCE_INDEX_AOI_1 | IS_AOI_SEQUENCE_INDEX_AOI_2;
	// enable sequence mode
    is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask));
}

void Area::disableSequenceAOI(){
	double pParam = 10;
	INT nMask = 0;
	is_AOI(camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask)); //Stop Sequence HDR
	//is_Exposure(camera->hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_DEFAULT, (void*)&pParam, sizeof(pParam));
	is_Exposure(camera->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&pParam, sizeof(pParam));
}

void  Area::getCamFrame( cv::Mat& frame, int& numSequence, double& time )
{
	UEYEIMAGEINFO ImageInfo;
	int nRet, pbo = 0;
	int pnNum;
	char * ppcMem;
	char * ppcMemLast;
	if (numSequence == -1){
		/*do {
			is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
			is_LockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
			nRet = is_IsVideoFinish(camera->hCam, &pbo);
			nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId, &ImageInfo, sizeof(ImageInfo));
		} while ((pbo != IS_VIDEO_FINISH));
		//cout << "pbo = : " << pbo << endl;
		numSequence = ImageInfo.wAOIIndex;
		*/
		is_GetActSeqBuf(camera->hCam, &pnNum, &ppcMem, &ppcMemLast);
		is_LockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, ppcMemLast);
		if (ppcMemLast == camera->m_pcImageMemory[0]){
			nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId[0], &ImageInfo, sizeof(ImageInfo));
		}
		else {
			nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId[1], &ImageInfo, sizeof(ImageInfo));
		}
		numSequence = ImageInfo.wAOIIndex;
	}
	else {
		/*do {
			is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
			nRet = is_IsVideoFinish(camera->hCam, &pbo);
			nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId, &ImageInfo, sizeof(ImageInfo));
		} while (pbo != IS_VIDEO_FINISH || ImageInfo.wAOIIndex == numSequence );
		//cout << "pbo = : " << pbo << endl;
		is_LockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, camera->m_pcImageMemory);
		*/
		do {
			is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, ppcMemLast);
			is_GetActSeqBuf(camera->hCam, &pnNum, &ppcMem, &ppcMemLast);
			is_LockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, ppcMemLast);
			if (ppcMemLast == camera->m_pcImageMemory[0]){
				nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId[0], &ImageInfo, sizeof(ImageInfo));
			}
			else {
				nRet = is_GetImageInfo(camera->hCam, camera->m_nMemoryId[1], &ImageInfo, sizeof(ImageInfo));
			}
		} while (ImageInfo.wAOIIndex == numSequence);
	}
	frame = cv::Mat(camera->height, camera->width, CV_8UC3, ppcMemLast, camera->width*(camera->m_bitsPerPixel / 8)); // last param = number of bytes by cols
	is_Exposure(camera->hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &time, 8);

	is_UnlockSeqBuf(camera->hCam, IS_IGNORE_PARAMETER, ppcMemLast);

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

