#ifndef _MTB_H
#define _MTB_H

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cvaux.h>

IplImage* reduction_image_fact2 (IplImage *src);
IplImage* binarisation(IplImage *src);
void MTB(IplImage *ref, IplImage *im, int *Nx, int *Ny);
void difference_minimum(IplImage* ref, IplImage* image, int *Nx , int *Ny);
void diff_images(IplImage *ref, IplImage *im, int Nx, int Ny, int *somme);
void decomposer_image(IplImage* src, IplImage** images, int NbEch);
void recalage_image_MTB(const int nimg, IplImage **images);
void test_MTB();

#endif
