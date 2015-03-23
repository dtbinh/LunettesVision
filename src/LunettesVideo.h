#pragma once

#define WINDOWCOORDSX 0
#define TEST_MODE false
#define FRAMES_COUNT 500
#define DEFAULT_SIZE cv::Size(640,480)

#include "Matrix.h"
//#include <windows.h> //Window specific
#include "Camera.h"
#include "FPS.h"
#include "MyTimer.h"
#include <mutex>
#include <thread>
#include "Profile.h"
#include <string>

using namespace std;
using namespace cv;

class OSD;

class LunettesVideo
{
public:
	LunettesVideo(void);
	~LunettesVideo(void);
	void run();
	bool isInitialized();
	void loadProfiles();
	map<int,Camera*> camList;
	std::vector<Profile*> profiles;
	cv::Mat remapImage(Camera* c);
	cv::Mat remapImageGpu(Camera* c);
	bool showEffects;
	mutex currentProfileMutex;
	Profile* currentProfile;
	void switchCrop();
	void switchHdr();
	void switchRemap();

	void selectNextArea();
	void selectPrevArea();
	void resetSelectArea();
	bool showSelectArea;

	Rect& getCurrentRect();
	Area* getCurrentArea();

	int getCurrentRectWidth();
	int getCurrentRectHeight();
	int getCurrentRectX();
	int getCurrentRectY();

	void setCurrentRectWidth(int i);
	void setCurrentRectHeight(int i);
	void setCurrentRectX(int i);
	void setCurrentRectY(int i);

	void zoomInCurrentZone(double d);
	void zoomOutCurrentZone(double d);

	bool switchProfile(int i);
	void hideCurrentArea();
	void saveCurrentProfile();
	void swapProfileZones();

	void exit();

private:
	//Variables
	bool initialized, needRemap, needHdr, changeExposure;
	OSD* osd;
	int resX, resY;
	int cameraCount;
	UEYE_CAMERA_LIST* pucl;
	void blendAreasZones(Mat& img);
	void initSequenceAOI(Area *r);

	int currentAreaRectangle;

	//Prototypes
	bool setWindowsParams(void);
	bool needClose;
	void loadXMLParams();
	Size getCamRes(int idcam);
	void setMatrixParams();

	int currentProfileIndex;
	bool initProfilCameras();
	void startRemapThreads();
	void stopRemapThreads();

	bool isCameraAviable(Profile* p);
};
