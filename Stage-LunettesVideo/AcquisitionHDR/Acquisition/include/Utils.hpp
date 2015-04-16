#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#define NULLSTRING ""

#define UNIT_TEST_GOOGLE ///< use to perform unit tests.
//~ #define __ULTRA_VERBOSE_MODE__ ///< use to perform all call graph, etc. Very Very Very verbose mode! Use it carefully!
#undef __ULTRA_VERBOSE_MODE__		///< To be sure, Ultra verbose mode is unactivated.

#ifdef UNIT_TEST_GOOGLE
	#include <gtest/gtest.h>
#endif // UNIT_TEST_GOOGLE

#include "cvCaptureIEEE1394.h"

#include "libMesaSR.h"
#include "cameraUVC.h"
#include "capture.h"

#include <vector>
#include <iostream>
#include <cmath>

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>


#include "Vector3d.hpp"


/// Delete operator redefinition with test.
#define safe_delete(ptr) if(ptr!=NULL) delete ptr;
/// Delete[] operator redefinition with test.
#define safe_delete_tab(ptr) if(ptr!=NULL) delete[] ptr;

/// Macro use to create an offset instead of an address.
#define BUFFER_OFFSET(a) ((char*)NULL + (a))



/// Clamp macro (useful)
#define CLAMP(val,min,max) (((val)<(min))?(min):(((val)>(max))?(max):(val)))

/// LOGHERE Macro : use to test good passage here.
#define LOGHERE() std::cerr << __FILE__ << "[" << __LINE__ << "]: in {" << __PRETTY_FUNCTION__ << "}\n";
/// LOGVALUE Macro : use to trace a value
#define LOGSTART()		printf("%s[%d] : %s\t",__FILE__ , __LINE__ , __PRETTY_FUNCTION__  );
#define LOGVALUE(val)  	printf("%s = %0.5f\n", #val, (float)val);
#define LOGEND() printf( "\r" );


#define COLOR_PRINT_ON(colour) {printf( "\033[0;3%sm", GetAnsiColorCode( colour ) );}
#define COLOR_PRINT_OFF() {printf("\033[m");fflush(stdout);}

#define ERROR 	COLOR_PRINT_ON( COLOR_RED ); std::cerr
#define ENDERROR	std::endl;COLOR_PRINT_OFF();
#define LOG 		COLOR_PRINT_ON( COLOR_YELLOW ); std::clog
#define ENDLOG	std::endl;COLOR_PRINT_OFF();
#define INFO 		COLOR_PRINT_ON( COLOR_GREEN ); std::cout
#define ENDINFO	std::endl;COLOR_PRINT_OFF();


#ifdef __ULTRA_VERBOSE_MODE__
#define INDENT_MAX 50
static const char __indent_string[] = "| | | | | | | | | | | | | | | | | | | | | | | | | \0\0\0";
static const char *indent = __indent_string + INDENT_MAX;

#define IN() INf("");
#define INf(fmt,args...) do{\
        printf("%s>>>[%s]:%s:%d " fmt "\n", indent, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## args);\
        if ((indent-2) >= __indent_string) indent-=2;\
} while(0)

#define OUT() OUTf("")
#define OUTp(ptr) OUTf("= %p",ptr)
#define OUTd(num) OUTf("= %d",num)
#define OUTf(fmt,args...) do{\
        if (indent <= __indent_string+INDENT_MAX) indent+=2;\
        printf("%s<<<[%s]:%s:%d " fmt "\n", indent, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## args);\
} while(0)
#else // !__ULTRA_VERBOSE_MODE__
#define IN()
#define OUTf()
#endif // __ULTRA_VERBOSE_MODE__



typedef enum { COLOR_RED, COLOR_GREEN, COLOR_YELLOW } COLOR; ///< define color type to print in color.

/**
 * Return color in ansi mode (char* string)
 *
 * @param COLOR type to convert.
 */
inline static const char* GetAnsiColorCode( COLOR eColor )
{
	switch ( eColor )
	{
		case COLOR_RED:
			return "1";
		case COLOR_GREEN:
			return "2";
		case COLOR_YELLOW:
			return "3";
		default:
			return NULL;
	};
}

/// static step (max value return by camera over precision --> lack from SwissRanger API!
static float step[4] = {(float)0xff / 30000.0f, (float)0xff / 15000.0f, (float)0xff / 30000.0f, (float)0xff / 15000.0f };
/// max distance in meter for the selected camera mode --> lack from SwissRanger API!
static float Distance [] = {3.75f, 5.f, 7.142857f, 7.5f, 7.894837f, 2.5f, 10.f, 15.f, 5.172414f, 4.838710f, 10.344828f, 9.677419f};

/// Initialiaze to Identity matrix an array of float.
static inline void MatrixIdentity( GLfloat pMatrix[16] )
{
	pMatrix[0]	= 1.0;
	pMatrix[1]	= 0.0;
	pMatrix[2]	= 0.0;
	pMatrix[3]	= 0.0;
	pMatrix[4]	= 0.0;
	pMatrix[5]	= 1.0;
	pMatrix[6]	= 0.0;
	pMatrix[7]	= 0.0;
	pMatrix[8]	= 0.0;
	pMatrix[9]	= 0.0;
	pMatrix[10]	= 1.0;
	pMatrix[11]	= 0.0;
	pMatrix[12]	= 0.0;
	pMatrix[13]	= 0.0;
	pMatrix[14]	= 0.0;
	pMatrix[15]	= 1.0;
}

static inline void copy( float* pDest, float* pSrc, unsigned int uiSize )
{
	for( unsigned int i = 0 ; i  < uiSize ; i++ )
	{
		pDest[ i ] = pSrc[ i ];
	}
}

//#define CAM_FIREWIRE

#ifdef CAM_FIREWIRE
	 //~ #define GL_CAMERA_COLOR_MODE GL_LUMINANCE // To draw correctly grabed images. (because of Bayer)
	 #define GL_CAMERA_COLOR_MODE GL_RGB // To draw correctly grabed images. (because of Bayer)
	#undef CvCapture
	#define CvCapture				cvCaptureIeee1394
	#define cvCaptureFromCAMExt(a,b)	{a=new cvCaptureIeee1394(); cvCaptureIeee1394Create(&a,b);}
	#define cvQueryFrameExt(a,b)		cvCaptureIeee1394GetFrame(a,(b->imageData))
	#define cvReleaseCapture(a)		cvCaptureIeee1394Release(*a)
	#define cvSetCaptureProperty(...)
	#error Firewire camera!
#else
	#undef CvCapture
	#define CvCapture				Capture
	#define GL_CAMERA_COLOR_MODE GL_RGB // To draw image in RGB format.
	#define cvCaptureFromCAMExt(a,b)	{ \
        UVCOpenParameters UVCParam; \
        UVCParam.width = 640; \
        UVCParam.height = 480; \
        UVCParam.fRate = 30; \
        UVCParam.format = V4L2_PIX_FMT_YUYV; \
        if( a->open( CAM_TYPE_UVC, &UVCParam) <= 0 ) \
        { \
            a->close(); \
        } \
        a->setParameter( V4L2_CID_EXPOSURE_AUTO, 1); \
        a->setParameter( V4L2_CID_EXPOSURE_ABSOLUTE, 1000); \
        a->getFrameFromCam(b);\
    }
	#define cvQueryFrameExt(a,b)		b=(IplImage*)(a->get1Frame());
	#define cvReleaseCapture(a) delete a;
#endif //CAM_FIREWIRE



#endif // __UTILS_HPP__
