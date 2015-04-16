#ifndef _SOLVEUR_DEBEVEC_H
#define _SOLVEUR_DEBEVEC_H

int weight_func(const int Z);
void G_solve(int **Z, float **B, float l, float *G, float *lE, const int NbPt, const int NbTe);

#endif
