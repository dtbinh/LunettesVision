#ifndef _NRUTIL_H
#define _NRUTIL_H

extern void nrerror(char *error_text);

extern float *vector(int nl, int nh);
extern void free_vector(float *v, int nl, int nh);

extern int *ivector(int nl, int nh);
extern void free_ivector(int *v, int nl, int nh);

extern float ***f3tensor(int n0, int n, int m0, int m, int k0, int k);
extern void free_f3tensor(float ***v, int n0, int n, int m0, int m, int k0, int k);

extern float **matrix(int nrl, int nrh, int ncl, int nch);
extern void free_matrix(float **m, int nrl, int nrh, int ncl, int nch);

#define SIGN(a, b) ((b)<0 ? (-fabs(a)) : fabs(a))
#define SQR(a) ((a)*(a))
#define FMAX(a, b) ((a)>(b) ? (a) : (b))
#define IMIN(a, b) ((a)<(b) ? (a) : (b))

#endif
