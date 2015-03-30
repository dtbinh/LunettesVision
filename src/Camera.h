#pragma once

#include "Matrix.h"
#include <thread>
#include <uEye.h>

#include <sys/mman.h>
#include <opencv2/core/core.hpp>
#include <mutex>
#include "FPS.h"

#define MAX_SEQ_BUFFERS 32767
class Camera
{
public:

	HIDS hCam;
	int cameraID;
	int width, height;
	int m_bitsPerPixel;
	double m_actualFrameRate;
	size_t m_numberOfFrames;
	bool active;
	SENSORINFO m_sInfo;
	
	//NO SEQUENCE
	int	m_nMemoryId; // camera memory -  ID
	char* m_pcImageMemory; // camera memory - pointer to memory

	
	//// Prototypes ////
	Camera(int index);
	~Camera();
	void exitCamera();
	void getMaxImageSize(INT *pnSizeX, INT *pnSizeY);
	cv::Size getSize();
	void loadDistortionMatrix();
	
	//SEQUENCE
	//std::vector<int>	m_nMemoryId; // camera memory - buffer ID
	//std::vector<char*> m_pcImageMemory; // camera memory - pointer to buffer
	
	
	cv::Mat map1, map2; // matrix for camera distortion
};

INT initCamera (HIDS , HWND );
