#pragma once

#define WIDTH 1280
#define HEIGHT 720
//#define WIDTH 1920
//#define HEIGHT 1080

#define GPU false

#include <iostream>
#include <math.h>
#include <stdio.h>

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\gpu\gpu.hpp"
#include "Function.h"

/*
#include <cuda.h>
#include <cuda_runtime_api.h>*/

class Matrix
{
public:
	Matrix();
	Matrix(cv::Size input, cv::Size output);
	~Matrix(void);
	void invalidate();
	cv::Mat getXmat();
	cv::Mat getYmat();

	cv::gpu::GpuMat getGpuXmat();
	cv::gpu::GpuMat getGpuYmat();

	cv::Rect getCenterRect();
	void setInputSize(cv::Size s);
	void setRemapZone(cv::Rect, double zoom);

	void moveCenterX(int px);
	void moveCenterY(int px);
	void changeWidth(int px);
	void changeHeight(int px);
	void changeZoom(float px);

	bool isInitialized();
	void resize(cv::Size outputSize);
	cv::Size getOutputSize();

private:
	cv::gpu::GpuMat mXgpu, mYgpu;
	cv::Mat mX, mY;
	Function* func;

	bool initialized;

	cv::Size inputSize, outputSize;
	cv::Point* center;
	int width, height;
	float zoomCenter;

	float getCoef(int i);
};
