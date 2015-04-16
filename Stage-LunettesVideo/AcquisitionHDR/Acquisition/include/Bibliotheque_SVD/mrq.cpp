#include "nrutil.h"
#include "matrix.h"
#include "gaussj.h"

#define SWAP(a, b) {float temp= (a); (a)= (b); (b)= temp;}

void covsrt(float **covar, int ma, int ia[], int mfit)
{
	int i, j, k;
	
	for(i= mfit+1; i<=ma; i++)
		for(j= 1; j<=i; j++)
			covar[i][j]= covar[j][i]= 0.;
	
	k= mfit;
	for(j= ma; j>=1; j--)
		if(ia[j])
		{
			for(i= 1; i<=ma; i++)
				SWAP(covar[i][k], covar[i][j])
			for(i= 1; i<=ma; i++)
				SWAP(covar[k][i], covar[j][i])
			k--;
		}
}

void mrqcof(float x[], float y[], float sig[], int ndata,  float a[], int ia[], int ma, 
			float **alpha, float beta[], float *chisq, 
			void (*funcs)(float, float [], float *, float [], int))
{
	int i, j, k, l, m, mfit= 0;
	float ymod, wt, sig2i, dy, *dyda;
	
	dyda= vector(1, ma);
	
	for(j= 1; j<=ma; j++)
		if(ia[j])
			mfit++;
	
	for(j= 1; j<=mfit; j++)
	{
		for(k= 1; k<=j; k++)
			alpha[j][k]= 0.;
		beta[j]= 0.;
	}
	
	*chisq= 0.;
	
	for(i= 1; i<=ndata; i++)
	{
		(*funcs)(x[i], a, &ymod, dyda, ma);
		sig2i= 1./(sig[i]*sig[i]);
		dy= y[i]-ymod;
		for(j= 0, l= 1; l<=ma; l++)
			if(ia[l])
			{
				wt= dyda[l]*sig2i;
				for(j++, k= 0, m= 1; m<=l; m++)
					if(ia[m])
						alpha[j][++k]+= wt*dyda[m];
				beta[j]+= dy*wt;
			}
		*chisq+= dy*dy*sig2i;
	}
	
	for(j= 2; j<=mfit; j++)
		for(k= 1; k<j; k++)
			alpha[k][j]= alpha[j][k];

	free_vector(dyda, 1, ma);
}

void mrqmin(float x[], float y[], float sig[], int ndata, float a[], int ia[], int ma, 
			float **covar, float **alpha, float *chisq, 
			void (*funcs)(float, float [], float *, float [], int), float *alambda)
{
	int j, k, l;
	static int mfit;
	static float ochisq, *atry, *beta, *da, **oneda;
	
	if(*alambda<0.)
	{
		atry= vector(1, ma);
		beta= vector(1, ma);
		da= vector(1, ma);
		for(mfit= 0, j= 1; j<=ma; j++)
			if(ia[j])
				mfit++;
		oneda= matrix(1, mfit, 1, 1);
		*alambda= 0.001;
		mrqcof(x, y, sig, ndata, a, ia, ma, alpha, beta, chisq, funcs);
		ochisq= *chisq;
		for(j= 1; j<=ma; j++)
			atry[j]= a[j];
	}
	
	for(j= 1; j<=mfit; j++)
	{
		for(k= 1; k<=mfit; k++)
			covar[j][k]= alpha[j][k];
			covar[j][j]= alpha[j][j]*(1. + *alambda);
			oneda[j][1]= beta[j];
	}
	
	gaussj(covar, mfit, oneda, 1);
	for(j= 1; j<=mfit; j++)
		da[j]= oneda[j][1];
	
	if(*alambda==0.0)
	{
		covsrt(covar, ma, ia, mfit);
		free_matrix(oneda, 1, mfit, 1, 1);
		free_vector(da, 1, ma);
		free_vector(beta, 1, ma);
		free_vector(atry, 1, ma);
		return;
	}

	for(j= 0, l= 1; l<=ma; l++)
		if(ia[l])
			atry[l]= a[l]+da[++j];

	mrqcof(x, y, sig, ndata, atry, ia, ma, covar, da, chisq, funcs);

	if(*chisq < ochisq)
	{
		*alambda*= 0.1;
		ochisq= *chisq;
		
		for(j= 1; j<=mfit; j++)
		{
			for(k= 1; k<=mfit; k++)
				alpha[j][k]= covar[j][k];
			beta[j]= da[j];
		}
		for(l= 1; l<=ma; l++)
			a[l]= atry[l];
	}
	else
	{
		*alambda*= 10.;
		*chisq= ochisq;
	}
}
