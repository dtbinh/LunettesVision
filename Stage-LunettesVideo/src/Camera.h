#pragma once

#include "Matrix.h"
#include <thread>
#include <uEye.h>
//#include <sys/mman.h>
#include <opencv2/core/core.hpp>
#include <mutex>
#include "FPS.h"
#include "opencv2/photo/photo.hpp"
#include "opencv2/cudawarping.hpp"

#define MAX_SEQ_BUFFERS 32767
class Camera
{

private : 
	cv::Mat response; //matrice de fonction reponse du capteur
	

public:
	HIDS hCam;
	HWND m_hwndDisp;
	int cameraID;
	int width, height;
	int m_bitsPerPixel;
	double m_actualFrameRate;
	size_t m_numberOfFrames;
	bool active;
	SENSORINFO m_sInfo;
	
	int	m_nMemoryId; // camera memory -  ID
	char* m_pcImageMemory; // camera memory - pointer to memory

	//For HDR
	const cv::Mat& getResponse ()const { return response; }
	std::vector<cv::Mat> images; //vectors d'images et temps de calibration du capteur, pour fonction réponse
	std::vector<float> times;
	cv::Ptr<cv::CalibrateDebevec> calibrate;
	cv::Ptr<cv::MergeDebevec> merge_debevec;
	cv::Ptr<cv::TonemapDurand> tonemap;

	//// Prototypes ////
	Camera(int index);
	~Camera();
	void exitCamera();
	void setFrameRate(int desiredFrameRate);
	void setShutterMode(int nMode);
	void enableHdrMode();
	void getMaxImageSize(INT *pnSizeX, INT *pnSizeY);
	cv::Size getSize();
	void loadDistortionMatrix();

	cv::Mat map1, map2; // matrix for camera distortion
};

INT initCamera (HIDS , HWND );
