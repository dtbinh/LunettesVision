#pragma once

#include <iostream>
#include <time.h>
#include <map>
#include "Matrix.h"
#include "tinyxml2.h"
#include "OsdNode.h"

#define ON_SCREEN_TIME 10

class LunettesVideo;

using namespace std;
using namespace cv;

class OSD
{
public:

	OSD(LunettesVideo* parent);

	~OSD(void);
	void addInput(int key);
	bool isDisplay();
	void blendOSD(Mat& m);

private:

	time_t startTime, now;
	LunettesVideo* parent;
	void showOSD();
	void exitOSD();
	Mat backgroud;
	bool displayProfile, displayOsd;
	OsdNode *root, *current;
	int currentIndex;
	void applyEffect();
};
