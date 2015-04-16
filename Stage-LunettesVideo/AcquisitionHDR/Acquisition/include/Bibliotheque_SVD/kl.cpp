/*
	Karhunen-Loeve transform
	
	voir
	monitoring fault condition during manufacturing using the kl transform
	IY Tumer, KL Wood, IJ Bush-Vishniac
	
	Karhunen-Loeve expansion, section 2.3
	...
	
	+ routines numerical recipes

	jciehl nov 97
*/

#include <math.h>
#include "nrutil.h"
#include "matrix.h"
#include "kl.h"

/* Reduce a real, symmetric matrix to a symmetric, tridiag. matrix. */
/* Householder reduction of matrix a to tridiagonal form.
   Algorithm: Martin et al., Num. Math. 11, 181-195, 1968.
   Ref: Smith et al., Matrix Eigensystem Routines -- EISPACK Guide
        Springer-Verlag, 1976, pp. 489-494.
        W H Press et al., Numerical Recipes in C, Cambridge U P,
        1988, pp. 373-374.  */

void tred2(float **a, int n, float *d, float *e)
{
	int l, k, j, i;
	float scale, hh, h, g, f;

	for (i = n; i >= 2; i--)
	{
		l = i - 1;
		h = scale = 0.0;
		if (l > 1)
		{
			for (k = 1; k <= l; k++)
				scale += fabs(a[i][k]);
			if (scale == 0.0)
				e[i] = a[i][l];
			else
			{
				for (k = 1; k <= l; k++)
				{
					a[i][k] /= scale;
					h += a[i][k] * a[i][k];
				}
				f = a[i][l];
				g = f>0 ? -sqrt(h) : sqrt(h);
				e[i] = scale * g;
				h -= f * g;
				a[i][l] = f - g;
				f = 0.0;
				for (j = 1; j <= l; j++)
				{
					a[j][i] = a[i][j]/h;
					g = 0.0;
					for (k = 1; k <= j; k++)
						g += a[j][k] * a[i][k];
					for (k = j+1; k <= l; k++)
						g += a[k][j] * a[i][k];
					e[j] = g / h;
					f += e[j] * a[i][j];
				}
				hh = f / (h + h);
				for (j = 1; j <= l; j++)
				{
					f = a[i][j];
					e[j] = g = e[j] - hh * f;
					for (k = 1; k <= j; k++)
						a[j][k] -= (f * e[k] + g * a[i][k]);
				}
			}
		}
		else
			e[i] = a[i][l];
		d[i] = h;
	}
	d[1] = 0.0;
	e[1] = 0.0;
	for (i = 1; i <= n; i++)
	{
		l = i - 1;
		if (d[i])
		{
			for (j = 1; j <= l; j++)
			{
				g = 0.0;
				for (k = 1; k <= l; k++)
					g += a[i][k] * a[k][j];
				for (k = 1; k <= l; k++)
					a[k][j] -= g * a[k][i];
			}
		}
		d[i] = a[i][i];
		a[i][i] = 1.0;
		for (j = 1; j <= l; j++)
			a[j][i] = a[i][j] = 0.0;
	}
}

/* Tridiagonal QL algorithm -- Implicit */
void tqli(float *d, float *e, int n, float **z)
{
	int m, l, iter, i, k;
	float s, r, p, g, f, dd, c, b;

	for (i = 2; i <= n; i++)
		e[i-1] = e[i];
	e[n] = 0.0;
	for (l = 1; l <= n; l++)
	{
		iter = 0;
		do
		{
			for (m = l; m <= n-1; m++)
			{
				dd = fabs(d[m]) + fabs(d[m+1]);
				if (fabs(e[m]) + dd == dd) break;
			}
			if (m != l)
			{
				if (iter++ == 30) nrerror("No convergence in TLQI.");
				g = (d[l+1] - d[l]) / (2.0 * e[l]);
				r = sqrt((g * g) + 1.0);
				g = d[m] - d[l] + e[l] / (g + SIGN(r, g));
				s = c = 1.0;
				p = 0.0;
				for (i = m-1; i >= l; i--)
				{
					f = s * e[i];
					b = c * e[i];
					if (fabs(f) >= fabs(g))
					{
						c = g / f;
						r = sqrt((c * c) + 1.0);
						e[i+1] = f * r;
						c *= (s = 1.0/r);
					}
					else
					{
						s = f / g;
						r = sqrt((s * s) + 1.0);
						e[i+1] = g * r;
						s *= (c = 1.0/r);
					}
					g = d[i+1] - p;
					r = (d[i] - g) * s + 2.0 * c * b;
					p = s * r;
					d[i+1] = g + p;
					g = c * r - b;
					for (k = 1; k <= n; k++)
					{
						f = z[k][i+1];
						z[k][i+1] = s * z[k][i] + c * f;
						z[k][i] = c * z[k][i] - s * f;
					}
				}
				d[l] = d[l] - p;
				e[l] = g;
				e[m] = 0.0;
			}
		}  while (m != l);
	}
}

/* Create m * m covariance matrix from given n * m data matrix. */
void covcol(float **data, int n, int m, float **cov)
{
	int i, j, j1, j2;
	float a, b;
	float *mean;

	mean= vector(1, m);

	/* Determine mean of column vectors of input data matrix */
	for(j= 1; j<=m; j++)
	{
		mean[j] = 0.0;
		for (i= 1; i<=n; i++)
			mean[j]+= data[i][j];
		mean[j]/= (float)n;
	}

	/* Calculate the m * m covariance matrix. */
	for(j1= 1; j1<=m; j1++)
		for(j2= j1; j2<=m; j2++)
		{
			cov[j1][j2]= 0.0;
			for(i= 1; i<=n; i++)
			{
				a= data[i][j1] - mean[j1];
				b= data[i][j2] - mean[j2];
				cov[j1][j2]+= a*b;
			}
			cov[j2][j1]= cov[j1][j2];
		}

	free_vector(mean, 1, m);
}

/* Create n * n covariance matrix from given n * m data matrix. */
void covar(float **data, int n, int m, float **cov)
{
	int i, j, j1, j2;
	float a, b;
	float *mean;

	mean= vector(1, m);

	/* efface la matrice */
	for(i= 1; i<=n; i++)
		for(j= 1; j<=n; j++)
			cov[i][j]= 0.;

	/* Determine mean of column vectors of input data matrix */
	for(j= 1; j<=m; j++)
	{
		mean[j] = 0.0;
		for(i= 1; i<=n; i++)
			mean[j]+= data[i][j];
		mean[j]/= (float)n;
	}

	/* Calculate the n * n covariance matrix. */
	for(j= 1; j<=m; j++)
		for(j1= 1; j1<=n; j1++)
		{
			a= data[j1][j] - mean[j];
			for(j2= 1; j2<=n; j2++)
			{
				b= data[j2][j] - mean[j];
				cov[j1][j2]+= a*b;
			}
		}
		
	free_vector(mean, 1, m);
}

/* calcule la transformee de KL de la matrice data[1..n][1..m] et 
   copie les valeurs propres dans values[1..n] et les vecteurs 
   propres dans vectors[1..n][1..n] */
void kl(float **data, int n, int m, float *values, float **vectors)
{
	int  i, j, k, perm;
	float aux;
	float *interm;
		
	covar(data, n, m, vectors);		/* calcule la matrice de covariance n*n */
	
	interm= vector(1, n);
	tred2(vectors, n, values, interm);  /* Triangular decomposition */
	tqli(values, interm, n, vectors);   /* Reduction of sym. trid. matrix */
	/* values now contains the eigenvalues,
       columns of vectors now contain the associated eigenvectors. */

	free_vector(interm, 1, n);
}

#if 0
	/* tri decroissant des valeurs propres et des vecteurs propres */
	/* tri a bulles, beurrgll ?!?! */
	perm= 1;
	while(perm)
	{
		perm= 0;
		for(i= 1; i<m; i++)
			if(values[i]<values[i+1])
			{
				perm= 1;

				/* permutte les 2 valeurs propres */
				aux= values[i];
				values[i]= values[i+1];
				values[i+1]= aux;
				
				/* permutte les 2 vecteurs propres */
				for(k= 1; k<=m; k++)
				{
					aux= vectors[i][k];
					vectors[i][k]= vectors[i+1][k];
					vectors[i+1][k]= aux;
				}
			}
	}
#endif

