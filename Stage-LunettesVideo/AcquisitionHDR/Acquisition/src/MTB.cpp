#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <cvcompat.h>
#include <cmath>
#include <iostream>
#include <string>
#include <ctime>

#include "MTB.h"
#include "responseFunctionEstimator.h"



IplImage* reduction_image_fact2 (IplImage *src)
{
	CvSize size=cvGetSize(src) , newsize;
	newsize.width=(int)(size.width/2);
	newsize.height=(int)(size.height/2);
	IplImage *dst=cvCreateImage(newsize, IPL_DEPTH_8U, 1);
	for (int i=0 ; i<newsize.width ; i++)
	{
		for (int j=0 ; j<newsize.height ; j++)
		{
			(dst->imageData+dst->widthStep*j)[i]=(int)((cvGet2D(src,2*j,2*i).val[0]+cvGet2D(src,2*j,2*i+1).val[0]+cvGet2D(src,2*j+1,2*i).val[0]+cvGet2D(src,2*j+1,2*i+1).val[0])/4);
		}
	}
	
	//afficher_image(dst);
	return dst;
}


IplImage* binarisation(IplImage *src)
{
	CvSize size=cvGetSize(src);
	IplImage *dst=cvCreateImage(size, IPL_DEPTH_8U, 1);
	int *hist=(int *)calloc(256,sizeof(int));
	
	for (int i=0 ; i<size.width ; i++)
	{
		for (int j=0 ; j<size.height ; j++)
		{
			hist[(int)(cvGet2D(src,j,i).val[0])]++;	
		}
	}

	int somme=0, n=0;
	for (int i=0 ; i<256 ; i++)
	{
		somme+=hist[i];
		if (somme>(size.width*size.height/2))
		{
			n=i;
			break;
		}
	}
	//printf("%d\n",n);

	for (int i=0 ; i<size.width ; i++)
	{
		for (int j=0 ; j<size.height ; j++)
		{
			if(cvGet2D(src,j,i).val[0]<=n)
			{
				(dst->imageData+dst->widthStep*j)[i]=0;
			}
			else
			{
				(dst->imageData+dst->widthStep*j)[i]=255;
			}
		}
	}

	//system("Pause");
	return dst;
}

void decomposer_image(IplImage* src, IplImage** images, int NbEch)
{
	clock_t fin,deb=clock();

	IplImage *imgray=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	luminance_from_color(src,imgray);
	IplImage *imred=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1), *imwork=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	cvCopyImage(imgray,imwork);
	images[0]=binarisation(imwork);
	for (int i=1 ; i<NbEch ; i++)
	{

		imred=reduction_image_fact2(imwork);
		imwork=cvCreateImage(cvGetSize(imred),IPL_DEPTH_8U,1);
		cvCopyImage(imred,imwork);
		images[i]=binarisation(imred);
		//afficher_image(images[i]);
	}
	fin=clock();
	//printf("decompositin d'une image : %d ms\n", (fin-deb));
}

void diff_images(IplImage *ref, IplImage *im, int Nx, int Ny, int *somme)
{
	int Xmin, Xmax, Ymin, Ymax;
	CvSize size=cvGetSize(ref);
	int som=0;
	
	if(Nx<=0)
	{
		Xmin=0;
		Xmax=size.width+Nx;
	}
	else
	{
		Xmin=Nx;
		Xmax=size.width;
	}

	if(Ny<=0)
	{
		Ymin=0;
		Ymax=size.height+Ny;
	}
	else
	{
		Ymin=Ny;
		Ymax=size.height;
	}

	for (int i=Xmin ; i<Xmax ; i++)
	{
		for (int j=Ymin ; j<Ymax ; j++)
		{
			if(cvGet2D(ref,j,i).val[0]!=cvGet2D(im,j-Ny,i-Nx).val[0])
			{
				som++;
			}
		}
	}
	//printf("Difference entre images pour le decalage X=%d , Y=%d : %d pxl\n", Nx,Ny,som);
	*somme=som;
}

void difference_minimum(IplImage* ref, IplImage* image, int *Nx , int *Ny)
{
	int Xt=*Nx,Yt=*Ny;
	//facteur pour le changement d'échelle
	int X=2*Xt, Y=2*Yt, NXmin=2*Xt , NYmin=2*Yt;
	int somme_min=cvGetSize(ref).width*cvGetSize(ref).height, somme;
	//diff_images(ref,image,X,Y,&somme);
	//printf("Somme = %d , somme_min = %d\n",somme,somme_min);
	
	for (int i=-1 ; i<=1 ; i++)
	{
		for (int j=-1 ; j<=1 ; j++)
		{
			diff_images(ref,image,X+i,Y+j,&somme);
			//printf("Somme = %d , somme_min = %d\n",somme,somme_min);
			if(somme<somme_min)
			{
				somme_min=somme;
				NXmin=X+i;
				NYmin=Y+j;
				//printf("Nxmin = %d , Nymin = %d\n",NXmin,NYmin);
			}
		}
	}
	
	//printf("Nxmin = %d , Nymin = %d\n",NXmin,NYmin);
	*Nx=NXmin;
	*Ny=NYmin;
}

void MTB(IplImage *ref, IplImage *im, int *Nx, int *Ny)
{
	*Nx=0;
	*Ny=0;
	int NbEch=4;
	IplImage** refs = new IplImage* [NbEch];
	IplImage** images = new IplImage* [NbEch];
	decomposer_image(ref, refs, NbEch);
	decomposer_image(im, images, NbEch);
	for (int i=(NbEch-1); i>=0; i--)
	{
		//printf("\nDifference minimum pour l'echelle %d\n",i);
		difference_minimum(refs[i], images[i], Nx , Ny);
		//printf("Nx = %d , Ny = %d\n",*Nx,*Ny);
	}
}

void recalage_image_MTB(const int nimg, IplImage **images)
{
	clock_t fin,deb=clock();

	IplImage *imwork;
	CvSize size=cvGetSize(images[0]);
	int **dec = new int *[nimg];
	for (int i=0 ; i<nimg ; i++)
	{
		dec[i]= new int[2];
	}
	
	for (int i=0 ; i<nimg/2 ; i++)
	{
		MTB(images[nimg/2],images[i],&dec[i][0],&dec[i][1]);
	}
	dec[nimg/2][0]=0;
	dec[nimg/2][1]=0;
	for (int i=nimg/2+1 ; i<nimg ; i++)
	{
		MTB(images[nimg/2],images[i],&dec[i][0],&dec[i][1]);
	}

	int minX=0,minY=0,maxX=0,maxY=0;
	for (int i=0 ; i<nimg ; i++)
	{
		if (dec[i][0]<minX)
		{
			minX=dec[i][0];
		}
		if (dec[i][0]>maxX)
		{
			maxX=dec[i][0];
		}
		if (dec[i][1]<minY)
		{
			minY=dec[i][1];
		}
		if (dec[i][1]>maxY)
		{
			maxY=dec[i][1];
		}
	}
	
	imwork=cvCreateImage(cvSize(size.width-maxX+minX,size.height-maxY+minY),IPL_DEPTH_8U,3);
	CvPoint2D32f center;
	for (int i=0 ; i<nimg ; i++)
	{
		center.x=(float)(size.width+minX-dec[i][0]+maxX-dec[i][0])/2;
		center.y=(float)(size.height+minY-dec[i][1]+maxY-dec[i][1])/2;
		cvGetRectSubPix( images[i], imwork, center );
		
		images[i]=cvCreateImage(cvGetSize(imwork),IPL_DEPTH_8U,3);
		cvCopyImage(imwork,images[i]);
	}
	fin=clock();
	printf("temps pour le recalage : %d ms\n", (int)(fin-deb));
}

void test_MTB()
{
	int nimg=2;
	IplImage **images = new IplImage* [nimg];
	images[0]=cvLoadImage("C:/images/set3/DSC_2848b.JPG");
	images[1]=cvLoadImage("C:/images/set3/DSC_2844b.JPG");
	afficher_image(images[0]);
	afficher_image(images[1]);
	recalage_image_MTB(nimg, images);
	afficher_image(images[0]);
	afficher_image(images[1]);
	system("Pause");
}
