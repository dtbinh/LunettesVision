#include "solveur_Debevec.h"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <time.h>
#include "Bibliotheque_SVD/svd.h"


//int weight_func(const int Z)
//{
//	if (Z<=128)   return Z;
//	if (Z>128)    return (256-Z);
//}

int weight_func(const int Z)
{
	if (Z<=128)   return Z;
	if (Z>128 && Z<250)   return (256-Z);
	else return 6;
}

//int weight_func(const int Z)
//{
//	if (Z<=128)   return Z+128;
//	if (Z>128 && Z<256)   return (256-Z+128);
//}

//int weight_func(const int Z)
//{
//	return 1;
//}

//int weight_func(const int Z)
//{
//	if (Z<50) return 10;
//	if (Z>=50 && Z<=206)   return 15;
//	else   return 10;
//}


void G_solve(int **Z, float **B, float l, float *G, float *lE, const int NbPt, const int NbTe)
{
	int n=256;
	clock_t deb=clock(), fin;
	//nombre de lignes de A
	int lA=(NbPt*NbTe+n+1);
	//nombre de colonnes de A
	int cA=n+NbPt;
	
	//matrice des coefficients des équations que Debevec propose de résoudre
	float **A = new float *[lA+1];
	for (int i=0 ; i<=lA ; i++)
	{
		A[i]=new float [cA+1];
	}
	
	//vecteur de réponse b	
	float *b = new float [lA+1];

	//remplissage de A et b avec les bons coefficients
	/*possibilité de réduire le nombre de boucle en imbriquant avec les boucles précédentes*/
	for (int i=0; i<=lA ;i++)
	{
		b[i]=0;
		for (int j=1; j<=cA; j++)
		{
			A[i][j]=0;
		}
	}

	
	int k=1, wij=0;
	for (int i=0; i<NbPt ;i++)
	{
		for (int j=0; j<NbTe; j++)
		{
			wij=weight_func(Z[i][j]+1); 
			A[k][Z[i][j]+1]= (float)wij; 
			A[k][n+i+1]= -(float)wij;    
			b[k]=(float)wij*B[i][j];
			k+=1;
		}
	}

	A[k][129]=1.0f; 
	k+=1;

	for (int i=1; i<=n-2; i++)
	{
		A[k][i]=l*weight_func(i+1);
		A[k][i+1]=-2*l*weight_func(i+1);
		A[k][i+2]=l*weight_func(i+1);
		k+=1;
	}


	//vérification du remplissage de la matrice
				//A FAIRE

	//résolution du système en utilisant une SVD
	float *x= new float [lA+1];

	float *w= new float [cA+1];
	float **v= new float* [cA+1];
	for (int i=0 ; i<=lA ; i++)
	{
		v[i]=new float [cA+1];
	} 

	
	svdcmp(A,lA,cA,w,v);
	svbksb(A,w,v,lA,cA,b,x);

	/*for (int i=0; i<=n+NbPt ;i++)
	{
		printf("x[%d]= %f\n",i,x[i]);
	}*/
	//system("Pause");

	for (int i=1 ; i<=n ; i++)
	{
		G[i-1]=x[i];
	}
	//printf("G[0] = %f \nG[1] = %f \nG[255] = %f \n",G[0],G[1],G[255]);

	/*
	//correction des abhérations de sur-exposition
	G[255]=G[254]*2-G[253];

	//correction des abhérations de sous-exposition
	G[1]=G[2]*2-G[3];
	G[0]=G[1]*2-G[2];
	printf("G[0] = %f \nG[1] = %f \n",G[0],G[1]);
*/

	for (int i=n+1 ; i<=n+NbPt ; i++)
	{
		lE[i-n-1]=x[i];
	}

	fin=clock();
	printf("temps d'execution G_solve : %d ms\n",(fin-deb));

}
