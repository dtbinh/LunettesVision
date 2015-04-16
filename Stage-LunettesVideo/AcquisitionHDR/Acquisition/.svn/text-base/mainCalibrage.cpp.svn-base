#include <iostream>
#include <cstdio>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "utilitaire_calibrage_HDR.h"

using namespace std;

int main (int argc, char** argv)
{
	//argv[1] : prototype des noms d'images
	//argv[2] : nombre d'images (max 12)
	//argv[3] : savingFileName
	//argv[4] : index of first image
	if(argc < 5)
	{
		cout << "Not enough arguments"<< endl;
		cout << "Usage : " << argv[0] << " ImageNameTemplate ImageCount SavingFileName FirstImageIndex" << endl;
		cout << "Template name example : ../itisthere/my_picture_\%04d.png" << endl;
		return 0;
	}
	char imTempName[2048];
	strcpy(imTempName, argv[1]);
	int imNb = atoi(argv[2]);
	char saveFilePath[2048];
	strcpy(saveFilePath, argv[3]);
	int firstId = atoi(argv[4]);
	
	char imName[2048];

	IplImage** ldrImages = new IplImage* [imNb];
	float* expTimes = new float [imNb];
	
	//load images and manually associate exposure times
	for(int i = 0 ; i < imNb ; i++)
	{
		sprintf(imName, imTempName, i+firstId);
		cout << "loading image : " << imName << endl;
		ldrImages[i] = cvLoadImage(imName);
		cout << "Please enter exposure time for image " << i << endl;
		cin >> expTimes[i];	
	}
	
	//extract camera responce function
	calibrate_camera_for_HDR(ldrImages, imNb, expTimes, imNb, 20, saveFilePath);
		
	delete[] ldrImages;
	return 0;
}
