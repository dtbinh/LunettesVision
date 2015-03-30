#include "Area.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\photo\photo.hpp"
#include <vector>
#include <iostream>
#include <fstream>

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
	camIndex = -1;
}

Area::~Area(void)
{
}

//////////////////////////////////////////////////////////////////////////////
/// THREAD
//////////////////////////////////////////////////////////////////////////////

/*
Mat HDR(std::vector<Mat>& images, std::vector<float>& times){

	Mat response;
    Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
    calibrate->process(images, response, times);

    Mat hdr;
    Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
    merge_debevec->process(images, hdr, times, response);

    Mat ldr;
    Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
    tonemap->process(hdr, ldr);

   // imwrite("ldr.png", ldr * 255);
   // imwrite("hdr.hdr", hdr);
	
}*/


// Remapthreadfunction without FPS computation (faster, no lag)
void remapThreadFunction(Area* r) 
{
	
	while(!r->needClose) {
		// Get frame from camera
		// Upload it on the GPU
		cv::gpu::GpuMat srcGpu(r->getCamFrame());
		// Create the output GpuMat
		cv::gpu::GpuMat dstGpu(r->getDisplaySize(),srcGpu.type());
		// Remap or resize the camera frame
		if(r->needRemap && r->matrix!=NULL) {
			r->matrixMutex.lock();
			cv::gpu::remap(srcGpu,dstGpu,r->matrix->getGpuXmat(),r->matrix->getGpuYmat(),CV_INTER_LINEAR,cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
			r->matrixMutex.unlock();
		} else {
			cv::gpu::resize(srcGpu,dstGpu,r->getDisplaySize());
		}
		// Copy the output frame on the shared memory with the main thread
		r->frameMutex.lock();
		dstGpu.copyTo((r->currentFrame));
		r->frameMutex.unlock();
	}
}




// Remapthreadfunction with FPS computation
/*
void remapThreadFunction(Area* r) 
{
	FPS fps("Area");	// For performances control
	cv::Mat src, dst;
	
	
	MyTimer* timer = new MyTimer();
	timer->changeState(MyTimer::OTHER);
	timer->start();

	while(!r->needClose) {
		timer->changeState(MyTimer::GET_CAM_FRAME);
		
		// Get frame from camera
		src = r->getCamFrame();
		
		timer->changeState(MyTimer::MEMORY_TRANSIT);

		// Upload it on the GPU
		cv::gpu::GpuMat srcGpu;
		srcGpu.upload(src);
		timer->changeState(MyTimer::OTHER);

		// Create the output GpuMat
		cv::gpu::GpuMat dstGpu(r->getDisplaySize(),src.type());

		timer->changeState(MyTimer::REMAP_RESIZE);

		// Remap or resize the camera frame
		if(r->needRemap && r->matrix!=NULL) {
			r->matrixMutex.lock();
			cv::gpu::remap(srcGpu,dstGpu,r->matrix->getGpuXmat(),r->matrix->getGpuYmat(),CV_INTER_LINEAR,cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
			r->matrixMutex.unlock();
		} else {
			cv::gpu::resize(srcGpu,dstGpu,r->getDisplaySize());
		}

		timer->changeState(MyTimer::MEMORY_TRANSIT);

		// Transfer the output frame from the Gpu
		dst = cv::Mat(dstGpu);


		timer->changeState(MyTimer::OTHER);

		// There was one computed frame ! (for FPS computing)
		
		
		fps.calculate();  
		timer->changeState(MyTimer::IMAGE_COPY);

		// Copy the output frame on the shared memory with the main thread
		r->frameMutex.lock();
		dst.copyTo((r->currentFrame));
		r->frameMutex.unlock();
		timer->changeState(MyTimer::OTHER);
	}
	timer->stop();
	timer->print();
}*/

void Area::startThread()
{
	// If the area is a color area, we dont need a thread !
	if(type == CAMERA) {
		needClose = false;
		currentFrame.create(camera->getSize().height,camera->getSize().width,CV_8UC3);
		invalidate();	// If needed, compute the remap matrix
		remapThread = std::thread(remapThreadFunction, this);
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

cv::Mat Area::getCamFrame()
{
	/*cv::Mat frame;
	do {
		camera->videoStream->retrieve(frame);
		
	} while(frame.empty());*/

	cv::Mat frame(1200,1600,CV_8UC3,camera->m_pcImageMemory,1600*3);
	if(needCrop) return frame(cameraROI);
	else return frame;
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

