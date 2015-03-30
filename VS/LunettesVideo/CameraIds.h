#pragma once

#include "Matrix.h"
#include <thread>
#include <opencv2\core\core.hpp>
#include <mutex>
#include "FPS.h"

#include "uEye.h"
#include "uEye_tools.h"
#include "ueye_deprecated.h"

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
	cv::VideoCapture *videoStream;
	cv::Mat map1, map2; // matrix for camera distortion
};