#ifndef _KL_H
#define _KL_H

extern void tred2(float **a, int n, float *d, float *e);
extern void tqli(float *d, float *e, int n, float **z);
extern void covcol(float **data, int n, int m, float **cov);
extern void covar(float **data, int n, int m, float **cov);
extern void kl(float **data, int n, int m, float *values, float **vectors);


#endif
