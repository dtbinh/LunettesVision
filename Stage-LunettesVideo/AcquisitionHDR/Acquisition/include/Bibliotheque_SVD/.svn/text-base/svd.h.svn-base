#ifndef _SVD_H
#define _SVD_H

extern void svbksb(float **u, float w[], float **v, int m, int n, float b[], float x[]);
extern void svdcmp(float **a, int m, int n, float w[], float **v);
extern void svdvar(float **v, int ma, float w[], float **cvm);
extern void svdfit(float x[], float y[], float sig[], int ndata, float a[], int ma, 
			float **u, float **v, float w[], float *chisq, 
			void (*funcs)(float, float [], int));

#endif
