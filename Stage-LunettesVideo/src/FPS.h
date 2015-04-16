#pragma once
#include <iostream>
#include <time.h>
#include <string>

class FPS
{
public:
	FPS(std::string);
	~FPS(void);

	std::string getFps();
	void calculate();

private:
	std::string label;
	time_t startTime, now;
	int imgCount;
	double currentFPS;
};
