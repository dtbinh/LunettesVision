#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <cvcompat.h>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include "ran.h"
#include <stdio.h>

#include "solveur_Debevec.h"
#include "MTB.h"
#include "responseFunctionEstimator.h"


using namespace std;

IplImage** images = NULL;
float TeI[12]={2.f,1.f,(float)1/2,(float)1/4,(float)1/8,(float)1/16,(float)1/32,(float)1/64,(float)1/128,(float)1/256,(float)1/512,(float)1/1024}; //table de correspondance Initiale
float Te[12]={2.f,1.f,(float)1/2,(float)1/4,(float)1/8,(float)1/16,(float)1/32,(float)1/64,(float)1/128,(float)1/256,(float)1/512,(float)1/1024}; //table de correspondance Modifiable
//float TeI[40]={3.2f,4.f,5.f,6.25f,8.f,10.f,12.8f,16.f,20.f,25.f,32.f,40.f,50.f,64.f,80.f,100.f,128.f,160.f,200.f,266.66f,320.f,390.22f,487.78f,609.72f,812.96f,975.56f,1219.45f,1625.93f,1951.12f,2438.9f,3251.86f,4033.33f,4840.f,6050.f,8066.67f,9307.69f,12100.f,15125.f,20166.67f,24200.f}; //table de correspondance Initiale
//float Te[40]={3.2f,4.f,5.f,6.25f,8.f,10.f,12.8f,16.f,20.f,25.f,32.f,40.f,50.f,64.f,80.f,100.f,128.f,160.f,200.f,266.66f,320.f,390.22f,487.78f,609.72f,812.96f,975.56f,1219.45f,1625.93f,1951.12f,2438.9f,3251.86f,4033.33f,4840.f,6050.f,8066.67f,9307.69f,12100.f,15125.f,20166.67f,24200.f}; //table de correspondance Modifiable
//param�tres r�glables du programme
int NI=12;
int NPI=50,NP=50;

char path2[255]="C:/images/mapremiereHDRI_f_color.jpg";

bool ROBUST_RECONSCTRUCTION = true;
char REC[3]="RR";

bool LUMINANCE = true;
char LUM[3]="1C";

bool FILTRE = false;
char FIL[3]="Fn";

bool RECALAGE = false;
char CAL[3]="Rn";

bool SUPERVISE_PIXEL = false;

bool AFFICHE_COLOR = true;

bool LOADER = false;

bool SAVE = false;

bool pro9000=false;

int RandInit=0;

float gR[256], gG[256], gB[256];
float  **Br, **Bg, **Bb; //tableaux des temps d'expositions des pixels r�cup�r�s
int **Zr, **Zg, **Zb;	//tableau des valeurs R , G et B des pixels r�cup�r�s 

void afficher_image(IplImage *src)
{
	//cvNamedWindow("image :",1);
	cvShowImage("image :", src);
	cvWaitKey(10);
	//cvDestroyWindow("image :");
}

void afficher_tab_Z(int **Z, const int nimg, const int NbPt)
{
	printf("\nAffichage d'un tableau de Z\n");
	for (int i=0 ; i<NbPt ; i++)
	{
		printf("point %02d |",i+1);
		for (int j=0 ; j<nimg ; j++)
		{
			printf(" %03d |",Z[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void allocate_vars(const int nimg, const int NbPt = 3)
{
	int NbTe = nimg;
	images= new IplImage* [nimg];
	Zr=new int*[NbPt];
	Zg=new int*[NbPt];
	Zb=new int*[NbPt];
	Br=new float*[NbPt];
	Bg=new float*[NbPt];
	Bb=new float*[NbPt];
	for(int i = 0 ; i < NbPt; i++)
	{
		Zr[i] = new int[NbTe];
		Zg[i] = new int[NbTe];
		Zb[i] = new int[NbTe];
		Br[i] = new float [NbTe];
		Bg[i] = new float [NbTe];
		Bb[i] = new float [NbTe];
	}
}

//renvoie une image en niveau de gris (dst) repr�sentative de la luminance d'une image couleur (src)
void luminance_from_color(const IplImage *src, IplImage *dst)
{
	int i,j;
	for(i=0;i<src->height;i++) 
	{
		for(j=0;j<src->width;j++) 
		{
			(dst->imageData+dst->widthStep*i)[j]=((src->imageData + src->widthStep*i)[j*3]*19 + (src->imageData + src->widthStep*i)[j*3+1]*183 + (src->imageData + src->widthStep*i)[j*3+2]*54)/256;
		}
	}
}

void load_G_func_channel(const char file[256], float gfunction[256])
{
	setlocale(LC_ALL,"POSIX");
	int k;
	FILE *f=fopen(file,"r");
	for (k=0;k<256;k++)
	{	
		fscanf(f, "%f\n", &gfunction[k]);
		printf("gfunction[%i]= %f\n", k, gfunction[k]);
	}
}

void load_G_func(float gfunc[3][256])
{
	load_G_func_channel("C:/images/fonctions_gR.txt",gfunc[0]);
	load_G_func_channel("C:/images/fonctions_gG.txt",gfunc[1]);
	load_G_func_channel("C:/images/fonctions_gB.txt",gfunc[2]);
}


void save_G_func_channel(const char file[256], float gfunction[256])
{
	setlocale(LC_ALL,"POSIX");
	FILE *f=fopen(file,"w");
	for (int i=0 ; i<256 ; i++)
	{
		fprintf(f,"%f\n",gfunction[i]);
		printf("%f\n",gfunction[i]);
	}
	fclose(f);
}
void save_G_func(float gfunc[3][256])
{
	save_G_func_channel("C:/images/fonctions_gR.txt", gfunc[0]);
	save_G_func_channel("C:/images/fonctions_gG.txt", gfunc[1]);
	save_G_func_channel("C:/images/fonctions_gB.txt", gfunc[2]);
}
void lecture_multi_images(const int nimg, IplImage **images)

{
	//on ne considere que des sets de 12 images dont la premiere a 2s de temps d'exposition puis 1s, 1/2, 1/4, 1/8, 1/15, 1/30, 1/60, 1/125, 1/250 et 1/500.
	//r�pertoire + chemin des images
	//char rep[256]="C:/images/set1/%d.png";//set1
	char rep[256]="C:/images/set3/DSC_%db.JPG";//set3
	//char rep[256]="C:/images/set6/IMG_%04db.JPG";
	
	//cvNamedWindow("image :",1);
	// numero de la premi�re image du set de 12, on considere que les autres images suivent naturellement (nom identique avec un chiffre et +1 au chiffre � chaque fois)
	//int num1=1;//set1
	int num1=2838;//set3

	if (pro9000)
	{
		char rep[256]="C:/images/set_9000pro/expo_%d.bmp";//camera 9000pro
		int num1=5;
		NI=9;
		for (int i=0;i<9;i++)
		{
			char directory[256];
			sprintf(directory,rep,(unsigned int) i+num1);
			//printf("%s\n",directory);
			images [i]= cvLoadImage( directory );
			Te[i]=(float)(1.f/pow(2.f,(float)i));
			//cvShowImage("image :", images[i]);
			//cvWaitKey(1000);
		}
		return;
	}

	if(nimg==40)
	{
		num1=1;
		for (int i=0;i<nimg;i++)
		{
			char directory[256];
			sprintf(directory,rep,(unsigned int) i+num1);
			//printf("%s\n",directory);
			images [i]= cvLoadImage( directory );
			//Te[i]=Te[i]/50000;
			//cvShowImage("image :", images[i]);
			//cvWaitKey(1000);
		}
		return;
	}

	//if(nimg==3)
	//{
	//	num1=1;
	//	for (int i=0;i<nimg;i++)
	//	{
	//		char directory[256];
	//		sprintf(directory,rep,(unsigned int) (10*i+10+num1));
	//		//printf("%s\n",directory);
	//		images [i]= cvLoadImage( directory );
	//		Te[i]=Te[10*i+10+num1];
	//		//cvShowImage("image :", images[i]);
	//		//cvWaitKey(1000);
	//	}
	//	return;
	//}

	//initialisation des tableaux images et Te
	if (FILTRE)
	{
		//on exclut les 4 premi�res images (2s , 1s , 1/2s et 1/4s) des calculs ainsi que les 2 derni�res (1/250 et 1/500) 
		num1+=4;
		switch (nimg)
		{
		case 6:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) i+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[i+4];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 5:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) i+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[i+4];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 4:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 3*i/2+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[3*i/2+4];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 3:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 2*i+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[2*i+4];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 2:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 3*i+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[3*i+4];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}
		}
	}
	
	else
	{

		switch (nimg)
		{
		case 12:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];

					sprintf(directory,rep,(unsigned int) i+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 6:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 2*i+1+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[2*i+1];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 4:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 3*i+1+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[3*i+1];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 3:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 4*i+2+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[4*i+2];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}

		case 2:
			{
			for (int i=0;i<nimg;i++)
				{
					char directory[256];
					sprintf(directory,rep,(unsigned int) 6*i+3+num1);
					//printf("%s\n",directory);
					images [i]= cvLoadImage( directory );
					Te[i]=Te[6*i+3];
					//cvShowImage("image :", images[i]);
					//cvWaitKey(1000);
				}
			break;
			}
		}
	}
}

void tirage_point(const int NbPt, int **tab_point, const int width ,const int height)
{
	srand(RandInit);
	for (int i=0 ; i<NbPt ; i++)
	{
		
		tab_point[i][0]=(int)(width*((float)rand()/(float)RAND_MAX));
		tab_point[i][1]=(int)(height*((float)rand()/(float)RAND_MAX));
		//printf("point %d : ( %d , %d )\n",i+1,tab_point[i][0],tab_point[i][1]);
	}
}
void tirage_point2(const int NbPt, int **tab_point, const int width ,const int height)
{
	Ranq1 myran(RandInit);
	for (int i=0 ; i<NbPt ; i++)
	{
		
		tab_point[i][0]=(int)(myran.int64() % (width));
		tab_point[i][1]=(int)(myran.int64() % (height));
		printf("point %d : ( %d , %d )\n",i+1,tab_point[i][0],tab_point[i][1]);
	}
}

void lecture_pixel_images(const int nimg, IplImage **images, const float *Te, int **Zr , int **Zg , int **Zb , float **Br, float **Bg, float **Bb, const int NbPt = 3)
{
	CvSize size=cvGetSize(images[0]);
	//r�cup�ration des valeurs des pixels 
	int **pix=new int*[NbPt];
	for (int i=0 ; i<NbPt ; i++)
	{
		pix[i]=new int[2];
	}
	//tirage_point(NbPt,pix,size.width,size.height);
	tirage_point2(NbPt,pix,size.width,size.height);

	//visualisation des pixels dont on r�cup�re les valeurs
 	IplImage *imgWork=cvCreateImage(size,IPL_DEPTH_8U,3);
	cvCopyImage(images[nimg/2],imgWork);
	for (int i=0 ; i<NbPt ;i++)
		{
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]]=(uchar)255;
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]+1]=(uchar)255;
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]+2]=(uchar)255;
		}
	cvNamedWindow("image :");
	cvShowImage("image :", imgWork);
	cvWaitKey();
	cvDestroyWindow("image :");
	
	//r�cup�ration et affichage des valeurs des pixels
	CvScalar s;

	for (int j=0 ; j<nimg ; j++)
		{
			/*cvNamedWindow("image :");
			cvShowImage("image :", images[j]);
			cvWaitKey();
			cvDestroyWindow("image :");*/
			//printf("Te=%f :\n",Te[j]);
   			for (int i=0 ; i<NbPt ; i++)
				{
					s=cvGet2D(images[j],pix[i][1],pix[i][0]);
 					Zr[i][j]=(int)s.val[2];
					Zg[i][j]=(int)s.val[1];
					Zb[i][j]=(int)s.val[0];

					Br[i][j]=log(Te[j]);
					Bg[i][j]=log(Te[j]);
					Bb[i][j]=log(Te[j]);
/*
					valeurs[0][j][i]=s.val[0];
					valeurs[1][j][i]=s.val[1];
					valeurs[2][j][i]=s.val[2];
					
					printf("pixel %d : r = %f  g = %f  b = %f\n",i,valeurs[2][j][i],valeurs[1][j][i],valeurs[0][j][i]);
					printf("pixel %d : r = %d  g = %d  b = %d\n",i,Zr[i][j],Zg[i][j],Zb[i][j]);
			*/	}
			//printf("\n");
		}
}

void lecture_pixel_supervisee(const int nimg, IplImage **images, const float *Te, int **Zr , float **Br, int *NbPoint)
{
	srand(RandInit);
	clock_t fin,deb=clock();
	int	NbPt = *NbPoint;
	int cpt_max= 2*nimg;
	IplImage** images2 = new IplImage* [nimg];
	int* verif=new int[256];
	float* moy=new float[NbPt];
	

	for (int i=0 ; i<nimg ; i++)
	{
		images2[i]=cvCreateImage(cvGetSize(images[i]),IPL_DEPTH_8U,1);
		//luminance_from_color(images[i], images2[i]);
		cvCvtColor(images[i], images2[i],CV_RGB2GRAY);
	}

	CvSize size=cvGetSize(images[0]);
	//r�cup�ration des valeurs des pixels 
	int **pix=new int*[NbPt];
	int **pix_ord=new int*[NbPt];
	for (int i=0 ; i<NbPt ; i++)
	{
		pix[i]=new int[2];
		pix_ord[i]=new int[2];
	}
	


	//r�cup�ration et affichage des valeurs des pixels
	CvScalar s;
	bool valid;
	int cpt_valid=0;

	for (int i=0 ; i<NbPt ; i++)
	{
		int cpt=0;
		float current_moy=0;
		
		while (cpt<cpt_max)
		{
			valid=true;
			pix[i][0]=(int)((size.width-1)*((float)rand()/(float)RAND_MAX));
			pix[i][1]=(int)((size.height-1)*((float)rand()/(float)RAND_MAX));
			current_moy=0;
			for (int j=0 ; j<nimg ; j++) 
				{
					s=cvGet2D(images2[j],pix[i][1],pix[i][0]);
					//verif[Zr[i][j]]++;
					current_moy+=(int)s.val[0];
				}
			current_moy=current_moy/nimg;
			printf("moyenne courante point %d : %f\n",i+1,current_moy);
			for (int k=0 ; k<NbPt ; k++)
			{
				if(fabs(current_moy-moy[k])<((float)256/NbPt))
				{
					valid=false;
				}
			}
		
			if (valid)
			{
				moy[i]=current_moy;
				for (int j=0 ; j<nimg ; j++) 
				{
					s=cvGet2D(images2[j],pix[i][1],pix[i][0]);
					Zr[cpt_valid][j]=(int)s.val[0];
					Br[cpt_valid][j]=log(Te[j]);
				}
				cpt_valid++;
				break;
			}
		cpt++;
		}
	}

	
	for (int i=0 ; i<NbPt ; i++)
	{
		printf("moyenne point %d : %f\n",i+1,moy[i]);
	}

	//visualisation des pixels dont on r�cup�re les valeurs
 	IplImage *imgWork=cvCreateImage(size,IPL_DEPTH_8U,3);
	cvCopyImage(images[nimg/2],imgWork);
	for (int i=0 ; i<NbPt ;i++)
		{
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]]=(uchar)255;

			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]+1]=(uchar)255;
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]+2]=(uchar)255;
		}
	cvNamedWindow("image :");
	cvShowImage("image :", imgWork);
	cvWaitKey();
	cvDestroyWindow("image :");


	afficher_tab_Z(Zr,nimg,cpt_valid);
	*NbPoint=cpt_valid;
	printf("nombre de points recuperes : %d\n", cpt_valid);
	fin=clock();
	printf("temps dans selection pixel supervisee : %d ms\n",(int)(fin-deb));
	system("Pause");
	
	delete images2;
	delete verif;
	delete moy;
	delete pix;
	delete pix_ord;
}

//fonction calculant les nouvelles valeurs Ei*dTj pour 
void reconstruction_G_lineaire(int **Z, float **B, float l, float *G, const int NbPt, const int NbTe)
{
	//creation d'une structure de donn�e accueillant les nouvelles valeurs de Y des points de la courbe recal�s
	float **Y=new float*[NbPt];
	for(int i = 0 ; i < NbPt; i++)
	{
		Y[i] = new float [NbTe];
	}

	//on instantie et on remplit le tableau des moyennes
	float *tab_moy=(float *)calloc(NbPt,sizeof(float));
	for (int i=0 ; i<NbPt ; i++)
	{
		for (int j=0 ; j<NbTe ; j++)
		{
			tab_moy[i]+=Z[i][j];
		}
		tab_moy[i]=tab_moy[i]/NbTe;
		printf("moyenne ligne %d : %f\n",1+i,tab_moy[i]);
	}


	//on ordonne les lignes selon leur moyenne
	int *ordre_ligne=(int*)malloc(NbPt*sizeof(int));
	bool place;
	ordre_ligne[0]=0;
	for (int i=1 ; i<NbPt ; i++)
	{
	
		for (int j=0 ; j<i ; j++)
		{
			place=false;
			if(tab_moy[i]<tab_moy[ordre_ligne[j]])
			{
				for(int k=i-1 ; k>=j ;k--)
				{
					ordre_ligne[k+1]=ordre_ligne[k];
				}
				ordre_ligne[j]=i;
				place=true;
				break;
			}
		}
		
		if (place==false)
		{
			ordre_ligne[i]=i;
		}
	}

	for(int i=0 ; i<NbPt ;i++)
	{
		printf("moyenne %d : %f\n",i+1,tab_moy[ordre_ligne[i]]);
	}


	//on r�cup�re la ligne dont la moyenne est la plus proche de 128
	int i_min=0;
	for (int i=1 ; i<NbPt ; i++)
	{
		if(fabs(tab_moy[i_min]-128)>fabs(tab_moy[i]-128))
		{
			i_min=i;
		}
	}
	printf("indice de ligne mediane : %d\n",i_min+1);

	//recalage du set de points median
	int jInf=0, jSup=NbTe;
	for (int j=0 ; j<NbTe ; j++)
	{
		if(Z[i_min][j]<=Z[i_min][jInf] && Z[i_min][j]>=128)
		{
			jInf=j;
		}

		if(Z[i_min][j]>=Z[i_min][jSup] && Z[i_min][j]<=128)
		{
			jSup=j;
		}
	}
	printf("jInf = %d\n",jInf);
	printf("jSup = %d\n",jSup);
	float a,b,h;	
	if(jInf==jSup)
	{
		Y[i_min][jInf]=0;
	}

	else
	{
		a=(B[i_min][jSup]-B[i_min][jInf])/(Z[i_min][jSup]-Z[i_min][jInf]);
		printf("a = %f\n",a);
		Y[i_min][jInf]=a*(Z[i_min][jInf]-128);
	}

	h=Y[i_min][jInf]-B[i_min][jInf];
	printf("h = %f\n",h);

	for(int k=0 ; k<jInf ; k++)
	{
		Y[i_min][k]=h+B[i_min][k];	
		printf("le point d'abscisse %d : B = %f devient Y = %f\n",Z[i_min][k],B[i_min][k],Y[i_min][k]);
	}
	for(int k=jInf+1 ; k<NbTe ; k++)
	{
		Y[i_min][k]=h+B[i_min][k];
		printf("le point d'abscisse %d : B = %f devient Y = %f\n",Z[i_min][k],B[i_min][k],Y[i_min][k]);
	}

	int iMedian;
	for (int j=0 ; j<NbTe ; j++)
	{
		if(ordre_ligne[j]==i_min)
		{
			iMedian=j;
			printf("iMedian = %d\n",iMedian);
			break;
		}
	}

	//recalage des sets situ�s avant le set m�dian
	for (int i=iMedian-1 ; i>=0 ; i--)
	{
//		recalage_ligne(Z[i],Z[i+1],Y[i],Y[i+1],B[i],tab_moy[i],tab_moy[i]);	
	}

	//recalage des sets situ�s apr�s le set m�dian
	for (int i=iMedian+1 ; i<NbPt ; i++)
	{
		
	}
}

void lecture_pixel_images_luminance(const int nimg, IplImage **images, const float *Te, int **Zr , float **Br, const int NbPt = 3)
{
	IplImage** images2 = new IplImage* [nimg];

	for (int i=0 ; i<nimg ; i++)
	{
		images2[i]=cvCreateImage(cvGetSize(images[i]),IPL_DEPTH_8U,1);
		//luminance_from_color(images[i], images2[i]);
		cvCvtColor(images[i], images2[i],CV_BGR2GRAY);
	}

	CvSize size=cvGetSize(images[0]);
	//r�cup�ration des valeurs des pixels 
	int **pix=new int*[NbPt];
	for (int i=0 ; i<NbPt ; i++)
	{
		pix[i]=new int[2];
	}
	//tirage_point(NbPt,pix,size.width,size.height);
	tirage_point2(NbPt,pix,size.width,size.height);

	//visualisation des pixels dont on r�cup�re les valeurs
 	IplImage *imgWork=cvCreateImage(size,IPL_DEPTH_8U,3);
	cvCopyImage(images[nimg/2],imgWork);
	for (int i=0 ; i<NbPt ;i++)
		{
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]]=(uchar)255;
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]+1]=(uchar)255;
			(imgWork->imageData+imgWork->widthStep*pix[i][1])[3*pix[i][0]+2]=(uchar)255;
		}
	cvNamedWindow("image :");
	cvShowImage("image :", imgWork);
	cvWaitKey();
	cvDestroyWindow("image :");

	//r�cup�ration et affichage des valeurs des pixels
	CvScalar s;

	for (int j=0 ; j<nimg ; j++)
		{
			//printf("Te=%f :\n",Te[j]);
			for (int i=0 ; i<NbPt ; i++)
				{
					s=cvGet2D(images2[j],pix[i][1],pix[i][0]);
 					Zr[i][j]=(int)s.val[0];
					Br[i][j]=log(Te[j]);
				}

		}
}
