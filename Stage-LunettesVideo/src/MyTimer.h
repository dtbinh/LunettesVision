#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <time.h>
#include <sys/stat.h>


class MyTimer
{
public:
	MyTimer(void);
	~MyTimer(void);

	enum State {
		GET_CAM_FRAME, REMAP_RESIZE, MEMORY_TRANSIT, DISPLAY, OSD, USER_INPUT, OTHER, IMAGE_COPY
	};

	std::map<State,double> time;
	State currentState;
	clock_t currentTime;
	std::ofstream myfile;

	void start();
	void changeState(State s);
	void stop();
	void reset();
	void print();
	void printInFile(int resX, int resY);
	bool fileExists(const std::string& filename);
};
