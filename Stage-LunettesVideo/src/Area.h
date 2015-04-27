#pragma once

#include "Matrix.h"
#include "Camera.h"
#include <mutex>
#include "MyTimer.h"
#include "opencv2/photo/photo.hpp"
#include "opencv2/cudawarping.hpp"

class Area
{
public:

	enum Type { CAMERA, COLOR};
	enum AreaType {DISPLAY_ZONE, REMAP_ZONE, CROP_ZONE};

	int camIndex;
	Camera* camera;		//If this Area is a camera Area
	double zoom;		// For remap
	cv::Scalar color;	//If this Area is a color Area
	Type type;			// Camera or color ?
	
	//For HDR 
	std::vector<cv::Mat> imagesHdr; 
	std::vector<float> timesExpo;
	cv::Mat matHDR;
	
	Matrix* matrix;			// Remap matrix
	cv::Rect displayZone;	
	cv::Rect cameraROI;		
	cv::Rect centralZone;

	bool baseRemap, needRemap, baseCrop, needCrop, needClose, hidden, needHdr;

	//cv::Mat currentFrame;	// Shared frame with the main thread
	cv::cuda::GpuMat currentFrame;
	bool empty;
	
	double pParam;
	std::thread remapThread;
	std::mutex frameMutex;
	std::mutex matrixMutex;
	
	// Methods

	Area(void);
	~Area(void);

	// Threads
	void startThread();
	void stopThread();

	void invalidate();
	void switchCrop();
	void switchRemap();
	void switchHdr();

	cv::Size getDisplaySize();
	cv::Rect getDisplayRect();
	cv::Rect getCameraCropRect();
	cv::Rect getCentralZoneRect();
	void getCamFrame(cv::Mat& frame, int& numSequence, double& time);
	cv::Rect& getRect(AreaType t);
	void HideAndShow();

	void setHdrThreadFunction();
	cv::Mat HDR(const std::vector<cv::Mat>& images, const std::vector<float>& times);
	void initSequenceAOI();
	void disableSequenceAOI();

	int getWidth(AreaType t);
	int getHeight(AreaType t);
	int getX(AreaType t);
	int getY(AreaType t);

	void setWidth(AreaType t, int i);
	void setHeight(AreaType t, int i);
	void setX(AreaType t, int i);
	void setY(AreaType t, int i);

};
