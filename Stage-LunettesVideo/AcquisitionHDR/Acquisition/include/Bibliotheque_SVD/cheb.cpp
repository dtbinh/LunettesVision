/*
	polynomes de chebishev
	
	numerical recipes
*/

#include <math.h>
#include "nrutil.h"

void chebfit(float a, float b, float c[], int n, float (*func)(float))
{
	int k, j;
	float y;
	double sum;
	float fac, bpa, bma, *f;
	
	f= vector(0, n-1);
	bma= .5*(b-a);
	bpa= .5*(a+b);
	
	for(k= 0; k<n; k++)
	{
		y= cos(M_PI*(k+.5)/n);
		f[k]= (*func)(y*bma+bpa);
	}
	
	fac= 2./n;
	for(j= 0; j<n; j++)
	{
		sum= 0.;
		for(k= 0; k<n; k++)
			sum+= f[k]*cos(M_PI*j*(k+.5)/n);
		
		c[j]= fac*sum;
	}
	
	free_vector(f, 0, n-1);
}


float chebev(float a, float b, float c[], int m, float x)
{
	int j;
	float d= 0., dd= 0., tmp, y, y2;
	
	if((x-a)*(x-b) > 0.)
		nrerror("chebev: x not in range");
	
	y= (2.*x-a-b)/(b-a);
	y2= 2.*y;
	
	for(j= m-1; j>=1; j--)
	{
		tmp= d;
		d= y2*d - dd + c[j];
		dd= tmp;
	}
	
	return y*d - dd + .5*c[0];
}

void chebpc(float c[], float d[], int n)
{
	int k, j;
	float tmp, *dd;
	
	dd= vector(0, n-1);
	for(j= 0; j<n; j++)
	{
		d[j]= 0.;
		dd[j]= 0.;
	}
	
	d[0]= c[n-1];
	for(j= n-2; j>=1; j--)
	{
		for(k= n-j; k>=1; k--)
		{
			tmp= d[k];
			d[k]= 2.*d[k-1] - dd[k];
			dd[k]= tmp;
		}
		
		tmp= d[0];
		d[0]= -dd[0] + c[j];
		dd[0]= tmp;
	}
	
	for(j= n-1; j>=1; j--)
		d[j]= d[j-1] - dd[j];
	d[0]= -dd[0] + .5*c[0];
	
	free_vector(dd, 0, n-1);
}
