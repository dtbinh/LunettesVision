#ifndef _EXR_WRITING_FUNCTIONS
#define _EXR_WRITING_FUNCTIONS

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfFrameBuffer.h>

int WriteEXRImage (const char* fileName, int width, int height, float* ColorImPtr, float* ZImPtr, char* ConfidentImPtr )
{
	using namespace Imf; 
	float *rPixels=new float [width*height];
	float *gPixels=new float [width*height];
	float *bPixels=new float [width*height];
	float *xPixels=new float [width*height];
	float *yPixels=new float [width*height];
	float *zPixels=new float [width*height];
	char *cPixels=new char [width*height];

	for (int u = 0; u < width ; ++u)
	{
		for (int v = 0; v < height ; ++v)
		{
			rPixels[v*width+u]=ColorImPtr[3*(v*width+u)+0];
			gPixels[v*width+u]=ColorImPtr[3*(v*width+u)+1];
			bPixels[v*width+u]=ColorImPtr[3*(v*width+u)+2];
			xPixels[v*width+u]=ZImPtr[3*(v*width+u)+0];
			yPixels[v*width+u]=ZImPtr[3*(v*width+u)+1];
			zPixels[v*width+u]=ZImPtr[3*(v*width+u)+2];
			cPixels[v*width+u]=ConfidentImPtr[v*width+u];
		}
	}
	
	Header header (width, height);
	header.channels().insert ("R", Channel (FLOAT));
	header.channels().insert ("G", Channel (FLOAT));
	header.channels().insert ("B", Channel (FLOAT));
	header.channels().insert ("X", Channel (FLOAT));
	header.channels().insert ("Y", Channel (FLOAT));
	header.channels().insert ("Z", Channel (FLOAT));
	header.channels().insert ("C", Channel (FLOAT));
	OutputFile file (fileName, header);
	
	FrameBuffer frameBuffer; 
	frameBuffer.insert ("R",Slice (FLOAT,  (char *) rPixels,sizeof (*rPixels) * 1, sizeof (*rPixels) * width)); 
	frameBuffer.insert ("G",Slice (FLOAT,  (char *) gPixels,sizeof (*gPixels) * 1, sizeof (*gPixels) * width));
	frameBuffer.insert ("B",Slice (FLOAT,  (char *) bPixels,sizeof (*bPixels) * 1, sizeof (*bPixels) * width));  
	frameBuffer.insert ("X",Slice (FLOAT,  (char *) xPixels,sizeof (*xPixels) * 1, sizeof (*xPixels) * width)); 
	frameBuffer.insert ("Y",Slice (FLOAT,  (char *) yPixels,sizeof (*yPixels) * 1, sizeof (*yPixels) * width));  
	frameBuffer.insert ("Z",Slice (FLOAT,  (char *) zPixels,sizeof (*zPixels) * 1, sizeof (*zPixels) * width));   
	frameBuffer.insert ("C",Slice (FLOAT,  (char *) cPixels,sizeof (*cPixels) * 1, sizeof (*cPixels) * width));  


	file.setFrameBuffer (frameBuffer);
	file.writePixels (height);
	return 0;
}

int WriteEXRImage (const char* fileName, int width, int height, IplImage* ColorImPtr, float* ZImPtr, char* ConfidentImPtr )
{
	CvScalar Pix;
	using namespace Imf; 
	float *rPixels=new float [width*height];
	float *gPixels=new float [width*height];
	float *bPixels=new float [width*height];
	float *zPixels=new float [width*height];
	char *cPixels=new char [width*height];

	for (int u = 0; u < width ; ++u)
	{
		for (int v = 0; v < height ; ++v)
		{
			Pix=cvGet2D(ColorImPtr,u,v);
			rPixels[v*width+u]=Pix.val[0];
			gPixels[v*width+u]=Pix.val[1];
			bPixels[v*width+u]=Pix.val[2];
			zPixels[v*width+u]=ZImPtr[3*(v*width+u)+2];
			cPixels[v*width+u]=ConfidentImPtr[v*width+u];
		}
	}
	
	Header header (width, height);
	header.channels().insert ("R", Channel (FLOAT));
	header.channels().insert ("G", Channel (FLOAT));
	header.channels().insert ("B", Channel (FLOAT));
	header.channels().insert ("Z", Channel (FLOAT));
	header.channels().insert ("C", Channel (FLOAT));
	OutputFile file (fileName, header);
	
	FrameBuffer frameBuffer; 
	frameBuffer.insert ("R",Slice (FLOAT,  (char *) rPixels,sizeof (*rPixels) * 1, sizeof (*rPixels) * width)); 
	frameBuffer.insert ("G",Slice (FLOAT,  (char *) gPixels,sizeof (*gPixels) * 1, sizeof (*gPixels) * width));
	frameBuffer.insert ("B",Slice (FLOAT,  (char *) bPixels,sizeof (*bPixels) * 1, sizeof (*bPixels) * width));  
	frameBuffer.insert ("Z",Slice (FLOAT,  (char *) zPixels,sizeof (*zPixels) * 1, sizeof (*zPixels) * width));  
	frameBuffer.insert ("C",Slice (FLOAT,  (char *) cPixels,sizeof (*cPixels) * 1, sizeof (*cPixels) * width));  


	file.setFrameBuffer (frameBuffer);
	file.writePixels (height);
	return 0;
}

int WriteEXRImage (const char* fileName, int width, int height, float* ColorImPtr )
{
	CvScalar Pix;
	using namespace Imf; 
	float *rPixels=new float [width*height];
	float *gPixels=new float [width*height];
	float *bPixels=new float [width*height];

	for (int u = 0; u < width ; ++u)
	{
		for (int v = 0; v < height ; ++v)
		{
			rPixels[v*width+u]=ColorImPtr[3*(v*width+u)+0];
			gPixels[v*width+u]=ColorImPtr[3*(v*width+u)+1];
			bPixels[v*width+u]=ColorImPtr[3*(v*width+u)+2];
		}
	}
	
	Header header (width, height);
	header.channels().insert ("R", Channel (FLOAT));
	header.channels().insert ("G", Channel (FLOAT));
	header.channels().insert ("B", Channel (FLOAT));
	OutputFile file (fileName, header);
	
	FrameBuffer frameBuffer; 
	frameBuffer.insert ("R",Slice (FLOAT,  (char *) rPixels,sizeof (*rPixels) * 1, sizeof (*rPixels) * width)); 
	frameBuffer.insert ("G",Slice (FLOAT,  (char *) gPixels,sizeof (*gPixels) * 1, sizeof (*gPixels) * width));
	frameBuffer.insert ("B",Slice (FLOAT,  (char *) bPixels,sizeof (*bPixels) * 1, sizeof (*bPixels) * width));  


	file.setFrameBuffer (frameBuffer);
	file.writePixels (height);
	return 0;
}
#endif //_EXR_WRITING_FUNCTIONS
