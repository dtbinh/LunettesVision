#ifndef _MATRIX_H
#define _MATRIX_H

extern float **mat_copy(float **m, int nr, int nc);

extern float **mat_diag(float *v, int n);
extern float **mat_transp(float **a, int nr, int nc);
extern float **mat_transp0(float **a, int nr, int nc);
extern float **mat_mul(float **a, int nra, int nca, float **b, int nrb, int ncb);
extern float **mat_mul0(float **a, int nra, int nca, float **b, int nrb, int ncb);
extern float *mat_mul_vector(float **a, int nra, int nca, float *x, int nrx);
extern float *mat_mul_vector0(float **a, int nra, int nca, float *x, int nrx);

extern float **mat_invd(float **a, int nRow, int nCol, float *det);
extern float **mat_invd0(float **a, int nRow, int nCol, float *det);
extern void mat_print(char *name, float **a, int nr, int nc);
extern void mat_print0(char *name, float **a, int nr, int nc);
extern float **float2matrix(float *array, int nr, int nc);
extern float **double2matrix(double *array, int nr, int nc);
extern float **float2matrix0(float *array, int nr, int nc);
extern float **double2matrix0(double *array, int nr, int nc);
#endif
