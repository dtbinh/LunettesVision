#include "FPS.h"

FPS::FPS(string s)
{
	label = s;
	time(&startTime);
	currentFPS = 0;
	imgCount = 0;
}

FPS::~FPS(void)
{
}

void FPS::calculate() {
	imgCount++;
	time(&now);
	double d = difftime(now,startTime);
	if(d >= 1) {
		currentFPS = imgCount / d;
		imgCount = 0;
		startTime = now;
		std::cout << label << " " << currentFPS  << " FPS"<< std::endl;
	}
}

std::string FPS::getFps() {
	char s[6];
	sprintf(s, "%i", (int)currentFPS);
	return s;
}