#include "Camera.h"

INT InitCamera (HIDS *hCam, HWND hWnd);

//////////////////////////////////////////////////////////////////////////////
/// CONSTRUCTEUR - DESTRUCTEUR
//////////////////////////////////////////////////////////////////////////////

Camera::Camera(int index, cv::Size camSize)
{
	///////////////////////////////////////
	//          ANCIEN CODE
	//////////////////////////////////////////
	int m_hcam=10;
	is_CaptureVideo(m_hCam, IS_DONT_WAIT);
	videoStream = new cv::VideoCapture(index);
	if(!videoStream->isOpened()) { active = false; return; }
	else active = true;

	videoStream->set(CV_CAP_PROP_FRAME_WIDTH,camSize.width);
	videoStream->set(CV_CAP_PROP_FRAME_HEIGHT,camSize.height);
	videoStream->set(CV_CAP_PROP_FPS,60);

	double dWidth = videoStream->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = videoStream->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the  video
	cout << "Frame size: " << dWidth << " x " << dHeight << " Status : ";
	/////////////////////////////////////////////

	
	
}

Camera::~Camera(void)
{
	delete videoStream;
}

//////////////////////////////////////////////////////////////////////////////
/// OTHER FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

cv::Size Camera::getSize()
{
	double dWidth = videoStream->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = videoStream->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the  video
	return cv::Size(dWidth, dHeight);
}

void Camera::loadDistortionMatrix()
{
	cv::FileStorage fs("calibrationProperties", cv::FileStorage::READ);
	// Load matrices with the corresponding camera Id
}

