#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void nrerror(char *error_text)
{
	fprintf(stderr, "nr run-time error\n");
	fprintf(stderr, "%s\n", error_text);
	
	exit(1);
}

float *vector(int nl, int nh)
{
	float *v;

	v= (float *)malloc((unsigned)(nh-nl+1)*sizeof(float));
	if(!v)	nrerror("malloc error in vector()");
	
	return v-nl;
}

void free_vector(float *v, int nl, int nh)
{
	free(v+nl);
}

int *ivector(int nl, int nh)
{
	int *v;

	v= (int *)malloc((unsigned)(nh-nl+1)*sizeof(int));
	if(!v)	nrerror("malloc error in ivector()");
	
	return v-nl;
}

void free_ivector(int *v, int nl, int nh)
{
	free(v+nl);
}

float ***f3tensor(int n0, int n, int m0, int m, int k0, int k) 
{
	int i,j; 
	float ***v;
	
	if(n0!=1 || m0!=1 || k0!=1)
		fprintf(stderr, "allocation f3tensor %d..%d x %d..%d x %d..%d\n", n0, n, m0, m, k0, k);
	
	v= (float ***) malloc(sizeof(float **)*n);			// new T**[n];
	if(!v)
		nrerror("malloc error in f3tensor()");
	
	v[0] = (float **) malloc(sizeof(float *)*n*m);		// new T*[n*m]; 
	if(!v[0])
		nrerror("malloc error in f3tensor()");

	v[0][0] = (float *) malloc(sizeof(float)*n*m*k);	// new T[n*m*k]; 
	if(!v[0][0])
		nrerror("malloc error in f3tensor()");
	
	for(j=1; j<m; j++) 
		v[0][j] = v[0][j-1] + k; 
	
	for(i=1; i<n; i++) 
	{
		v[i] = v[i-1] + m;
		v[i][0] = v[i-1][0] + m*k;
		
		for(j=1; j<m; j++) 
			v[i][j] = v[i][j-1] + k;
	}
	
	return v;
}

void free_f3tensor(float ***v, int n0, int n, int m0, int m, int k0, int k)
{
	if(v)
	{
		free(v[0][0]);
		free(v[0]);
		free(v);
	}
}
