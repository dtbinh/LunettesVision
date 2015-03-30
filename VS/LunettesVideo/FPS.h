#pragma once
#include <iostream>
#include <time.h>
#include <string>

using namespace std;

class FPS
{
public:
	FPS(string);
	~FPS(void);

	std::string getFps();
	void calculate();

private:
	string label;
	time_t startTime, now;
	int imgCount;
	double currentFPS;
};
