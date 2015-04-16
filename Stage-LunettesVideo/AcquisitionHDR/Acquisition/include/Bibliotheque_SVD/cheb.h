#ifndef _CHEB_H
#define _CHEB_H

extern void chebfit(float a, float b, float c[], int n, float (*func)(float));
extern float chebev(float a, float b, float c[], int m, float x);
extern void chebpc(float c[], float d[], int n);

#endif
