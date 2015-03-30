#pragma once

#include "Matrix.h"
#include "Camera.h"
#include <mutex>
#include "MyTimer.h"

class Area
{
public:

	enum Type { CAMERA, COLOR};
	enum AreaType {DISPLAY_ZONE, REMAP_ZONE, CROP_ZONE};

	int camIndex;
	double zoom;		// For remap
	cv::Scalar color;	//If this Area is a color Area
	Type type;			// Camera or color ?

	Matrix* matrix;			// Remap matrix
	cv::Rect displayZone;	
	cv::Rect cameraROI;		
	cv::Rect centralZone;

	bool baseRemap, needRemap, baseCrop, needCrop, needClose, hidden, needHdr;

	//cv::Mat currentFrame;	// Shared frame with the main thread
	cv::gpu::GpuMat currentFrame;
	Camera* camera;

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
	cv::Mat getCamFrame();
	cv::Rect& getRect(AreaType t);
	void HideAndShow();


	int getWidth(AreaType t);
	int getHeight(AreaType t);
	int getX(AreaType t);
	int getY(AreaType t);

	void setWidth(AreaType t, int i);
	void setHeight(AreaType t, int i);
	void setX(AreaType t, int i);
	void setY(AreaType t, int i);

};
