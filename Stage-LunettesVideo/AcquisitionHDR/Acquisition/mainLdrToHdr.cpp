#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "responseFunctionEstimator.h"
#include "utilitaire_calibrage_HDR.h"

using namespace std;

int main (int argc, char** argv)
{
	//argv[1] : prototype des noms d'images
	//argv[2] : nombre d'images (max 12)
	//argv[3] : savingFileName
	//argv[3] : responce function saving File
	//argv[4] : first image index
	if(argc < 6)
	{
		cout << "Not enough arguments"<< endl;
		cout << "Usage : " << argv[0] << " ImageNameTemplate ImageCount ResponseFunctionFile SavingFileName FirstImageIndex" << endl;
		cout << "Template name example : ../hereweare/my_picture_\%02d.png" << endl;
		return 0;
	}
	char imTempName[2048];
	strcpy(imTempName, argv[1]);
	int imNb = atoi(argv[2]);
	char gfuncfile[2048];
	strcpy(gfuncfile, argv[3]);
	char saveFilePath[2048];
	strcpy(saveFilePath, argv[4]);
	int firstId = atoi(argv[5]);
	
	float gfunction[256];
	char imName[2048];

	IplImage** ldrImages = new IplImage* [imNb];
	float* expTimes = new float [imNb];
	
	//load images and manually associate exposure times
	for(int i = 0 ; i < imNb ; i++)
	{
		sprintf(imName, imTempName, i+firstId);
		ldrImages[i] = cvLoadImage(imName);
		cout << "Please enter exposure time for image " << i << endl;
		cin >> expTimes[i];
	}
	
	//load response function
	load_G_func_channel(gfuncfile, gfunction);
	
	//create EXR image from ldr images
	create_EXR_RGB_image_from_LDR_image(ldrImages, expTimes, imNb, gfunction,ldrImages[0]->width, ldrImages[0]->height, saveFilePath);
		
	delete[] ldrImages;
	return 0;
}
