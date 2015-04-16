#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "nrutil.h"

const float singularity_tolerance= 1e-20;

typedef struct 
{ 
	int row, col; 
} Pivot;


float **matrix(int nrl, int nrh, int ncl, int nch)
{
	int i;
	float **m;
	
	m= (float **)malloc((unsigned)(nrh-nrl+1)*sizeof(float *));
	if(!m)
		nrerror("malloc error in matrix()");
	
	m-= nrl;
	for(i= nrl; i<=nrh; i++)
	{
		m[i]= (float *)malloc((unsigned)(nch-ncl+1)*sizeof(float));
		if(!m[i])
			nrerror("malloc error in matrix()");

		m[i]-= ncl;
	}

	return m;
}

void free_matrix(float **m, int nrl, int nrh, int ncl, int nch)
{
	int i;
	
	for(i= nrh; i>=nrl; i--)
		free(m[i]+ncl);
	free(m+nrl);
}


float **mat_copy(float **m, int nr, int nc)
{
	int i, j;
	float **c= matrix(1, nr, 1, nc);
	
	for(i= 1; i<=nr; i++)
		for(j= 1; j<=nc; j++)
			c[i][j]= m[i][j];
	
	return c;
}

float **mat_diag(float *v, int n)
{
	int i, j;
	float **m= matrix(1, n, 1, n);
	
	for(i= 1; i<=n; i++)
		for(j= 1; j<=n; j++)
			if(i==j)
				m[i][j]= v[i];
			else
				m[i][j]= 0.0;
	
	return m;
}

float **mat_transp(float **a, int nr, int nc)
{
	int i, j;
	float **m= matrix(1, nc, 1, nr);
	
	for(i= 1; i<=nr; i++)
		for(j= 1; j<=nc; j++)
			m[j][i]= a[i][j];
	
	return m;
}

float **mat_transp0(float **a, int nr, int nc)
{
	int i, j;
	float **m= matrix(0, nc, 0, nr);
	
	for(i= 0; i<nr; i++)
		for(j= 0; j<nc; j++)
			m[j][i]= a[i][j];
	
	return m;
}

float **mat_mul(float **a, int nra, int nca, float **b, int nrb, int ncb)
{
	int i, j, k;
	float **m;
	
	if(nca!=nrb)
		nrerror("bad sized matrices in mat_mult()");
	
	m= matrix(1, nra, 1, ncb);
	
	for(i= 1; i<=nra; i++)
		for(j= 1; j<=ncb; j++)
		{
			m[i][j]= 0.0;
			for(k= 1; k<=nca; k++)
				m[i][j]+= a[i][k]*b[k][j];
		}

	return m;
}

float **mat_mul0(float **a, int nra, int nca, float **b, int nrb, int ncb)
{
	int i, j, k;
	float **m;
	
	if(nca!=nrb)
		nrerror("bad sized matrices in mat_mult()");
	
	m= matrix(0, nra, 0, ncb);
	
	for(i= 0; i<nra; i++)
		for(j= 0; j<ncb; j++)
		{
			m[i][j]= 0.0;
			for(k= 0; k<nca; k++)
				m[i][j]+= a[i][k]*b[k][j];
		}
	
	return m;
}

float *mat_mul_vector(float **a, int nra, int nca, float *x, int nrx)
{
	int i, j, k;
	float *b;
	
	if(nca!=nrx)
		nrerror("bad sized matrice/vector in mat_mul_vector()");
	
	b= vector(1, nrx);
	
	for(i= 1; i<=nra; i++)
	{
		b[i]= 0.0;
		for(k= 1; k<=nca; k++)
			b[i]+= a[i][k]*x[k];
	}	

	return b;
}

float *mat_mul_vector0(float **a, int nra, int nca, float *x, int nrx)
{
	int i, j, k;
	float *b;
	
	if(nca!=nrx)
		nrerror("bad sized matrice/vector in mat_mul_vector()");
	
	b= vector(0, nrx);
	
	for(i= 0; i<nra; i++)
	{
		b[i]= 0.0;
		for(k= 0; k<nca; k++)
			b[i]+= a[i][k]*x[k];
	}	

	return b;
}

float **float2matrix(float *array, int nr, int nc)
{
	int i, j;
	float **m= matrix(1, nr, 1, nc);
	
	for(i= 0; i<nr; i++)
		for(j= 0; j<nc; j++)
			m[i+1][j+1]= array[i*nc + j];

	return m;
}

float **double2matrix(double *array, int nr, int nc)
{
	int i, j;
	float **m= matrix(1, nr, 1, nc);
	
	for(i= 0; i<nr; i++)
		for(j= 0; j<nc; j++)
			m[i+1][j+1]= (float)array[i*nc + j];	

	return m;
}

float **float2matrix0(float *array, int nr, int nc)
{
	int i, j;
	float **m= matrix(0, nr, 0, nc);
	
	for(i= 0; i<nr; i++)
		for(j= 0; j<nc; j++)
			m[i][j]= array[i*nc + j];

	return m;
}

float **double2matrix0(double *array, int nr, int nc)
{
	int i, j;
	float **m= matrix(0, nr, 0, nc);
	
	for(i= 0; i<nr; i++)
		for(j= 0; j<nc; j++)
			m[i][j]= (float)array[i*nc + j];	

	return m;
}

void mat_print(char *name, float **a, int nr, int nc)
{
	int i, j;
	
	printf("%s\n", name);
	for(i= 1; i<=nr; i++)
	{
		for(j= 1; j<=nc; j++)
			printf("% f ", a[i][j]);
		printf("\n");
	}
}

void mat_print0(char *name, float **a, int nr, int nc)
{
	int i, j;
	
	printf("%s\n", name);
	for(i= 0; i<nr; i++)
	{
		for(j= 0; j<nc; j++)
			printf("% f ", a[i][j]);
		printf("\n");
	}
}


float **mat_invd0(float **a, int nRow, int nCol, float *determ_ptr)
{
	int j, k, l;
	int prow, pcol;		
	int no_swaps;
	float temp;
	float determinant= 1;
	float pivot_val, max_value, curr_value;
	float **im;
	Pivot *pivotp;
	Pivot *pivots= (Pivot *)malloc(nCol*sizeof(Pivot));
	int *was_pivoted= (int *)malloc(nRow*sizeof(int));

	if(nRow!=nCol)
		nrerror("matrix must be square in mat_inv()");

	/* cree la matrice resultat */
	im= matrix(0, nRow, 0, nCol);
	for(k= 0; k<nRow; k++)
		for(l= 0; l<nCol; l++)
			im[k][l]= a[k][l];

    for(k= 0; k<nRow; k++)
		was_pivoted[k]= 0;

	for(pivotp= pivots; pivotp < &pivots[nCol]; pivotp++)
	{
		prow= 0; 
		pcol= 0;
		max_value= 0;		
		for(j= 0; j<nCol; j++)
			if(!was_pivoted[j])
			{
				curr_value= 0;
				for(k= 0; k<nRow; k++)
					if(!was_pivoted[k] && (curr_value= fabs(im[k][j])) > max_value)
					{
						max_value= curr_value;
						prow= k;
						pcol= j;
					}
			}
		
		if(max_value < singularity_tolerance)
			if(determ_ptr)
			{
				*determ_ptr= 0;
				return im;
			}
			else
				nrerror("can't invert singular matrix in mat_inv()");

		pivotp->row= prow;
		pivotp->col= pcol;

		if(prow!=pcol)
			for(k= 0; k<nRow; k++)
			{
				temp= im[k][prow];
				im[k][prow]= im[k][pcol];
				im[k][pcol]= temp;
			}
		was_pivoted[prow]= 1;

		pivot_val= im[prow][prow];
		determinant*= pivot_val;		
		im[prow][prow]= 1;
		for(k= 0; k<nRow; k++)
			im[k][prow] /= pivot_val;

		for(k= 0; k<nRow; k++)
			if(k!=prow)
			{
				temp= im[prow][k];
				im[prow][k]= 0;
				for(l=0; l<nRow; l++)
					im[l][k] -= temp * im[l][prow];
			}
	}

	no_swaps= 0;
	for(pivotp= &pivots[nCol-1]; pivotp>=pivots; pivotp--)
		if(pivotp->row!=pivotp->col)
		{
			no_swaps++;
			for(k= 0; k<nCol; k++)
			{
				temp= im[pivotp->row][k];
				im[pivotp->row][k]= im[pivotp->col][k];
				im[pivotp->col][k]= temp;
			}
		}
	
	if(determ_ptr)
		*determ_ptr = (no_swaps & 1) ? -determinant : determinant;

	return im;
}

float **mat_invd(float **a, int nRow, int nCol, float *determ_ptr)
{
	int j, k, l;
	int prow, pcol;		
	int no_swaps;
	float temp;
	float determinant= 1;
	float pivot_val, max_value, curr_value;
	float **im;
	Pivot *pivotp;
	Pivot *pivots= (Pivot *)malloc(nCol*sizeof(Pivot)) - 1;
	int *was_pivoted= (int *)malloc(nRow*sizeof(int)) - 1;

	if(nRow!=nCol)
		nrerror("matrix must be square in mat_inv()");

	/* cree la matrice resultat */
	im= matrix(1, nRow, 1, nCol);
	for(k= 1; k<=nRow; k++)
		for(l= 1; l<=nCol; l++)
			im[k][l]= a[k][l];

    for(k= 1; k<=nRow; k++)
		was_pivoted[k]= 0;

	for(pivotp= &pivots[1]; pivotp<=&pivots[nCol]; pivotp++)
	{
		prow= 0; 
		pcol= 0;
		max_value= 0;		
		for(j= 1; j<=nCol; j++)
			if(!was_pivoted[j])
			{
				curr_value= 0;
				for(k= 1; k<=nRow; k++)
					if(!was_pivoted[k] && (curr_value= fabs(im[k][j])) > max_value)
					{
						max_value= curr_value;
						prow= k;
						pcol= j;
					}
			}
		
		if(max_value < singularity_tolerance)
			if(determ_ptr)
			{
				free(pivots+1);
				free(was_pivoted+1);
				*determ_ptr= 0;
				return im;
			}
			else
				nrerror("can't invert singular matrix in mat_inv()");

		pivotp->row= prow;
		pivotp->col= pcol;

		if(prow!=pcol)
			for(k= 1; k<=nRow; k++)
			{
				temp= im[k][prow];
				im[k][prow]= im[k][pcol];
				im[k][pcol]= temp;
			}
		was_pivoted[prow]= 1;

		pivot_val= im[prow][prow];
		determinant*= pivot_val;		
		im[prow][prow]= 1;
		for(k= 1; k<=nRow; k++)
			im[k][prow] /= pivot_val;

		for(k= 1; k<=nRow; k++)
			if(k!=prow)
			{
				temp= im[prow][k];
				im[prow][k]= 0;
				for(l= 1; l<=nRow; l++)
					im[l][k] -= temp * im[l][prow];
			}
	}

	no_swaps= 0;
	for(pivotp= &pivots[nCol]; pivotp>=&pivots[1]; pivotp--)
		if(pivotp->row!=pivotp->col)
		{
			no_swaps++;
			for(k= 1; k<=nCol; k++)
			{
				temp= im[pivotp->row][k];
				im[pivotp->row][k]= im[pivotp->col][k];
				im[pivotp->col][k]= temp;
			}
		}
	
	if(determ_ptr)
		*determ_ptr = (no_swaps & 1) ? -determinant : determinant;

	return im;
}
