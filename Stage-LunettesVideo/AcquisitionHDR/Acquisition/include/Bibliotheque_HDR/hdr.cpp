/*
	outils format hdr / RGBE
	radiance ward

	jciehl
	avril 2004

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "img.h"
#include "hdr.h"

#include "rgbe.h"


IMG *hdr_read(char *fname)
{
	rgbe_header_info info;
	IMG *img;
	FILE *in;
	int largeur, hauteur;
	int code;

	in= fopen(fname, "rb");
	if(in==NULL)
	{
		printf("\n -- read error '%s'\n", fname);
		return NULL;
	}

	// int RGBE_ReadHeader(FILE *fp, int *width, int *height, rgbe_header_info *info)
	code= RGBE_ReadHeader(in, &largeur, &hauteur, &info);
	if(code != RGBE_RETURN_SUCCESS)
	{
		fclose(in);
		return NULL;
	}

	img= new_img_datafloat(largeur, hauteur);

	// int RGBE_ReadPixels_RLE(FILE *fp, float *data, int scanline_width, int num_scanlines)
	code= RGBE_ReadPixels_RLE(in, img->dataf, largeur, hauteur);
	if(code != RGBE_RETURN_SUCCESS)
	{
		fclose(in);
		free_img(img);
		return NULL;
	}

	//
	fclose(in);

	return img;
}


int hdr_write(IMG *img, char *fname)
{
	FILE *out;
	int code;

	out= fopen(fname, "wb");
	if(out==NULL)
	{
		printf("\n -- write error '%s'\n", fname);
		return 0;
	}

	// int RGBE_WriteHeader(FILE *fp, int width, int height, rgbe_header_info *info)
	code= RGBE_WriteHeader(out, img->largeur, img->hauteur, NULL);
	if(code != RGBE_RETURN_SUCCESS)
	{
		printf("\n -- write error '%s'\n", fname);
		fclose(out);
		return 0;
	}

	code= RGBE_WritePixels_RLE(out, img->dataf, img->largeur, img->hauteur);
	if(code != RGBE_RETURN_SUCCESS)
	{
		printf("\n -- write error '%s'\n", fname);
		fclose(out);
		return 0;
	}

	fclose(out);
	return 1;
}


char *hdr_basename(char *fn)
{
    char *ext;
    
    ext= strrchr(fn , '.');
    if(ext)
        if(strcmp(ext, ".hdr")==0)
            *ext= 0;
    
    return fn;
}

int is_hdr(char *fn)
{
    char *ext;
    
    ext= strrchr(fn, '.');
    if(ext)
        if(strcmp(ext, ".hdr")==0)
            return 1;
    
    return 0;
}


/* float */
void img_hdr_normalise(IMG *hdr, float norm)
{
	float v, min, max;
	int size;
	int i;

	v= hdr->dataf[0];
	min= v;
	max= v;

	size= 3*hdr->largeur*hdr->hauteur;
	for(i= 1; i < size; i++)
	{
		v= hdr->dataf[i]; 
		if(v > max)
			max= v;
		else if(v < min)
			min= v;
	}

	//
	if(hdr->data == NULL)
	{
		hdr->data= (PIX *) malloc(sizeof(PIX [3]) * hdr->largeur*hdr->hauteur);
		assert(hdr->data != NULL);
                hdr->bits= 24;
                hdr->alignement= hdr->largeur * sizeof(PIX[3]);
	}

	for(i= 0; i < size; i+= 3)
	{
		hdr->data[i]= color_clamp((hdr->dataf[i] - min) / (max - min) * norm + 1.f);
		hdr->data[i +1]= color_clamp((hdr->dataf[i +1] - min) / (max - min) * norm + 1.f);
		hdr->data[i +2]= color_clamp((hdr->dataf[i +2] - min) / (max - min) * norm + 1.f);
	}
}


void img_hdr_preview(IMG *hdr, float compression, float saturation)
{
	float r, g, b, y;
	float k;
	int size;
	int i;

	if(hdr->data == NULL)
	{
		hdr->data= (PIX *) malloc(sizeof(PIX [3]) * hdr->largeur*hdr->hauteur);
		assert(hdr->data != NULL);
                hdr->bits= 24;
                hdr->alignement= hdr->largeur * sizeof(PIX[3]);
	}

	//
	k= 255.f / powf(saturation, 1.f / compression);

	size= 3*hdr->largeur*hdr->hauteur;
	for(i= 0; i < size; i+= 3)
	{
		y= .3f*hdr->dataf[i] + .59f*hdr->dataf[i +1] + .11f*hdr->dataf[i +2];

		r= hdr->dataf[i]    / y;
		g= hdr->dataf[i +1] / y;
		b= hdr->dataf[i +2] / y;

		if(y < saturation)
		{
			y= k * powf(y, 1.f / compression);
			r*= y;
			g*= y;
			b*= y;
		}
		else
		{
			r= 255.f;
			g= 255.f;
			b= 255.f;
		}

		hdr->data[i]=    color_clamp(r);
		hdr->data[i +1]= color_clamp(g);
		hdr->data[i +2]= color_clamp(b);
	}
}
