#pragma once

#include "Matrix.h"
#include <thread>
#include <uEye.h>
#include <opencv2/core/core.hpp>
#include <mutex>
#include "FPS.h"

class Camera
{
public:

	//// Prototypes ////
	Camera(int index, cv::Size camSize);
	~Camera(void);

	cv::Size getSize();
	void loadDistortionMatrix();

	//// Attributs ////
	bool active;
	//cv::VideoCapture *videoStream;
	char* m_pcImageMemory;
	int   m_nMemoryId;
	cv::Mat map1, map2; // matrix for camera distortion
};

INT InitCamera (HIDS , HWND );
