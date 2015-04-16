#include <cstdio>

#include "solveur_Debevec.h"
#include "responseFunctionEstimator.h"

#include "ExrWrite.hpp"



int calibrate_camera_for_HDR(IplImage **images, int NbImage, const float *TabTe, int NbTe, int NbPt, const char fileName[256])
{
	int NbPtMAX = 20;
	int** Z=new int*[NbPt];
	float** B=new float*[NbPt];
	float* Gfunction=new float[256];
	for(int i = 0 ; i < NbPt; i++)
	{
		Z[i] = new int[NbTe];
		B[i] = new float [NbTe];
	}
	float* lE=new float[NbPt];
	int NbPtValide= NbPt;
	
	
/*	// 2) selection des pixels dans les images
	lecture_pixel_supervisee(	const int nimg, //ENTREE : nombre d'images du set
			IplImage **images, //ENTREE :pointeur vers le tableau de pointeurs sur images
			const float *Te, //ENTREE temps d'exposition des images du set
			int **Zr , //SORTIE : tableau de valeurs des pixels sur chaque image pour les NbPoint sélectionnés
			float **Br, //SORTIE : log des temps d'exposition pour chacun des Zr
			int *NbPoint); //ENTREE/SORTIE : nombre de point à utilisés pour la recherche de la fonction G en entrée puis nombre de point valides trouvés en sortie*/
	lecture_pixel_supervisee(NbImage, images, TabTe, Z, B, &NbPtValide);
	
	
/*	// 3) détermination de la courbe de réponse inverse de la caméra à partir des pixels sélectionnés
	// /!\ la fonction ne fonctionne qu'avec des images où les points sont en niveau de gris de 0 à 255
	void G_solve(int **Z,  //ENTREE: tableau de valeurs des pixels sur chaque image pour les NbPoint sélectionnés
	 	float **B, //ENTREE : log des temps d'exposition pour chacun des Zr
	 	float l, //ENTREE : Constante de lissage : 10.0 par défaut
	 	float *G, //SORTIE : la fonction G qu'on cherche
	 	float *lE, //SORTIE : valeur HDR des NbPt points selectionnés
		const int NbPt, //ENTREE : nombre de point utilisés pour la recherche de la fonction G	
		const int NbTe);//ENTREE : nombre de temps d'expositins pour la recherche de la fonction G*/
	G_solve(Z, B, 10.0, Gfunction, lE, NbPtValide, NbTe);	

	
	// 4) stockage des courbes de réponse par couleur dans un fichier
	save_G_func_channel(fileName, Gfunction);
		
	delete Z;
	delete B;
	delete Gfunction;
	return 0;
}

int create_EXR_channels_from_LDR_image(IplImage **images, const float *Te, int nimg, float gfunction[256], const int width, const int height, float* EXRDataPtr)
{
	cvNamedWindow("images LDR", 0);
	clock_t deb,fin,diff;
	deb=clock();
	CvScalar Z;
	CvSize size=cvGetSize(images[0]);
	float numerateur[3]={0.0,0.0,0.0},denominateur[3]={0.0,0.0,0.0};
	float *dataf = EXRDataPtr;
	float *Mdataf;
	Mdataf = dataf;
			
	int i,j,k,m,cpt=0;
	
	/*for(k=0;k<nimg;k++)//boucle traitant les N images d'entrée
	{
		printf("affichage image %d \n", k);
		cvShowImage("images LDR", images[k]);
		cvWaitKey();
	}*/

	for (j=0;j<size.height;j++)
		{
		for (i=0;i<size.width;i++)//double boucle parcourant tous les pixels de l'image d'entrée
		{
			if (true)
			{
				numerateur[0]=0.0;
				numerateur[1]=0.0;
				numerateur[2]=0.0;
				denominateur[0]=0.0;
				denominateur[1]=0.0;
				denominateur[2]=0.0;
				for(k=0;k<nimg;k++)//boucle traitant les N images d'entrée
				{
					Z=cvGet2D(images[k],j,i);
					for (m=0;m<3;m++)//boucle traitant les 3 canaux
					{
						numerateur[m]+=weight_func(Z.val[m])*(gfunction[(int)Z.val[m]]-log(Te[k]));
						denominateur[m]+=weight_func(Z.val[m]);
					}
				}
				
				if (denominateur[0]==0.0)
					dataf[2]=0;
				else
					dataf[2] = exp(numerateur[0]/denominateur[0]);

				if (denominateur[1]==0.0)
					dataf[1]=0;
				else
					dataf[1] = exp(numerateur[1]/denominateur[1]);

				if (denominateur[2]==0.0)
					dataf[0]=0;
				else
					dataf[0] = exp(numerateur[2]/denominateur[2]);
				//printf("Pixels (%3d , %3d) : (%.5f , %.5f , %.5f)\n",i,j,dataf[0],dataf[1],dataf[2]);
				dataf+=3;
			}

			else
			{
				k=nimg/2;
				Z=cvGet2D(images[k],j,i);
					for (m=0;m<3;m++)//boucle traitant les 3 canaux
					{
						numerateur[m]=(gfunction[(int)Z.val[m]]-log(Te[k]));
					}
				dataf[0] = exp(numerateur[2]);
				dataf[1] = exp(numerateur[1]);
				dataf[2] = exp(numerateur[0]);
				dataf+=3;
			}	
		}
	}

	EXRDataPtr=Mdataf;
	fin=clock();
	printf("create_HDR : %d ms\n",(int)(fin-deb));
	return 0;
}//*/

int create_EXR_RGB_image_from_LDR_image(IplImage **images, const float *Te, int nimg, float gfunction[256], const int width, const int height, char* exrImagePath)
{
	float* exrData = new float [3*height*width];
	create_EXR_channels_from_LDR_image(images, Te, nimg, gfunction, width, height, exrData);
	WriteEXRImage (exrImagePath, width, height, exrData );
	delete[] exrData;
}

/*int create_EXR_channels_from_LDR_image(IplImage **images, const float *Te, int nimg, float gfunction[256], const int width, const int height, IplImage* EXRDataPtr)
{
	clock_t deb,fin,diff;
	deb=clock();
	CvScalar Z;
	CvSize size=cvGetSize(images[0]);
	EXRDataPtr = cvCreateImage(size,IPL_DEPTH_32F,3);

	float numerateur[3]={0.0,0.0,0.0},denominateur[3]={0.0,0.0,0.0};
	CvScalar result;

	int i,j,k,m,cpt=0;
	for (j=0;j<size.height;j++)
		{
		for (i=0;i<size.width;i++)//double boucle parcourant tous les pixels de l'image d'entrée
		{
			if (true)
			{
				numerateur[0]=0.0;
				numerateur[1]=0.0;
				numerateur[2]=0.0;
				denominateur[0]=0.0;
				denominateur[1]=0.0;
				denominateur[2]=0.0;
				for(k=0;k<nimg;k++)//boucle traitant les N images d'entrée
				{
					Z=cvGet2D(images[k],j,i);
					for (m=0;m<3;m++)//boucle traitant les 3 canaux
					{
						numerateur[m]+=weight_func(Z.val[m])*(gfunction[(int)Z.val[m]]-log(Te[k]));
						denominateur[m]+=weight_func(Z.val[m]);
					}
				}
				
				if (denominateur[0]==0.0)
					result.val[0]=0.0;

				else
					result.val[0] = exp(numerateur[0]/denominateur[0]);

				if (denominateur[1]==0.0)
					result.val[1]=0.0;

				else
					result.val[1] = exp(numerateur[1]/denominateur[1]);

				if (denominateur[2]==0.0)
					result.val[2]=0.0;

				else
					result.val[2] = exp(numerateur[2]/denominateur[2]);
				//printf("Pixels (%3d , %3d) : (%.5f , %.5f , %.5f)\n",i,j,dataf[0],dataf[1],dataf[2]);
			}
	
			cvSet2D(EXRDataPtr, j, i, result);
		}
	}

	fin=clock();
	printf("create_HDR : %d ms\n",(int)(fin-deb));
	return 0;
}//*/

