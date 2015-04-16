/*
	outils format targa 24 

	jciehl
	fevrier 2002
	
	+ lecture commentaires 
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "tga.h"

int lire_tga_entete(FILE *in, TGA_HEADER *tga)
{
	unsigned char *header;
	char *id;
	size_t n;
	int xflip, yflip;

	header= (unsigned char *) malloc(sizeof(unsigned char[18]));	
	assert(header);
	n= fread(header, sizeof(unsigned char[18]), 1, in);
	if(n==1)
	{
		tga->idlength= header[0];
		tga->colourmaptype= header[1];
		tga->datatypecode= header[2];
		tga->colourmaporigin= LOHI(header[3], header[4]);
		tga->colourmaplength= LOHI(header[5], header[6]);
		tga->colourmapdepth= header[7];
		tga->x_origin= LOHI(header[8], header[9]);
		tga->y_origin= LOHI(header[10], header[11]);
		tga->width= LOHI(header[12], header[13]);
		tga->height= LOHI(header[14], header[15]);
		tga->bitsperpixel= header[16];
		tga->imagedescriptor= header[17];

		free(header);

		// printf("image descriptor 0x%x, bits %d\n", tga->imagedescriptor, tga->bitsperpixel);
		// printf("origine x %d  y %d\n", tga->x_origin, tga->y_origin);
		
		xflip= (tga->imagedescriptor & 0x10) >> 4;
		yflip= (tga->imagedescriptor & 0x20) >> 5;

		if(tga->datatypecode!=2 
		|| tga->colourmaptype!=0 
		|| (tga->bitsperpixel!=24 && tga->bitsperpixel!=32)
		|| xflip!=0)
		{
			printf("\n -- unknown TGA format\n");
			return 0;
		}

		if(tga->idlength!=0)
		{
			id= (char *) malloc(sizeof(char) * tga->idlength);
			assert(id);

			fread(id, tga->idlength, 1, in);
			id[tga->idlength]= 0;
			printf("\n -- comment :\n%s\n", id);
			free(id);
		}

		return tga->bitsperpixel;
	}

	free(header);
	return 0;
}


IMG *tga_read(char *fname)
{
	TGA_HEADER tga;
	IMG *img;
	PIX *data;
	FILE *in;
	size_t n;
	size_t i, size;
	PIX r, v, b, a;
	int yflip;
	int y;
	int code;

	in= fopen(fname, "rb");
	if(in==NULL)
	{
		printf("\n -- read error '%s'\n", fname);
		return NULL;
	}

	img= NULL;
	code= lire_tga_entete(in, &tga);
	if(code==24)
	{
		img= new_img_data24(tga.width, tga.height);

		yflip= (tga.imagedescriptor & 0x20) >> 5;
		if(yflip==0)
		{
			for(y= img->hauteur -1; y >= 0; y--)
			{
				data= &img->data[y*img->largeur*3];
				
				n= fread(data, sizeof(unsigned char[3]), img->largeur, in);
				if(n != img->largeur)
					break;
				
				size= 3*img->largeur;
				for(i= 0; i < size; i+= 3)
				{
					r= data[i+2];
					v= data[i+1];
					b= data[i];
					
					data[i]= r;
					data[i+1]= v;
					data[i+2]= b;
				}
			}
		}
		else
		{
			for(y= 0; y < img->hauteur; y++)
			{
				data= &img->data[y*img->largeur*3];
				
				n= fread(data, sizeof(unsigned char[3]), img->largeur, in);
				if(n != img->largeur)
					break;
				
				size= 3*img->largeur;
				for(i= 0; i < size; i+= 3)
				{
					r= data[i+2];
					v= data[i+1];
					b= data[i];
					
					data[i]= r;
					data[i+1]= v;
					data[i+2]= b;
				}
			}
		}
		
	}
	else if(code==32)
	{
		img= new_img_data32(tga.width, tga.height);

		yflip= (tga.imagedescriptor & 0x20) >> 5;
		if(yflip==0)
		{
			for(y= 0; y < img->hauteur; y++)
			{
				data= &img->data[y*img->largeur*4];
				
				n= fread(data, sizeof(unsigned char[4]), img->largeur, in);
				if(n != img->largeur)
					break;
				
				size= 4*img->largeur;
				for(i= 0; i < size; i+= 4)
				{
					a= data[i+3];
					r= data[i+2];
					v= data[i+1];
					b= data[i];
					
					data[i]= r;
					data[i+1]= v;
					data[i+2]= b;
					data[i+3]= a;
				}
			}
		}
		else
		{
			for(y= img->hauteur -1; y >= 0; y--)
			{
				data= &img->data[y*img->largeur*4];
				
				n= fread(data, sizeof(unsigned char[4]), img->largeur, in);
				if(n != img->largeur)
					break;
				
				size= 4*img->largeur;
				for(i= 0; i < size; i+= 4)
				{
					a= data[i+3];
					r= data[i+2];
					v= data[i+1];
					b= data[i];
					
					data[i]= r;
					data[i+1]= v;
					data[i+2]= b;
					data[i+3]= a;
				}
			}
		}
	}
	else
		printf("\n -- unknown format '%s'\n", fname);
	
	fclose(in);

	return img;
}

int ecrire_tga_entete(FILE *out, TGA_HEADER *tga)
{
	size_t n;
	
	unsigned char *header= (unsigned char *) malloc(sizeof(unsigned char[18]));
	assert(header);

	header[0]= LO(tga->idlength);
	header[1]= LO(tga->colourmaptype);
	header[2]= LO(tga->datatypecode);
	header[3]= LO(tga->colourmaporigin);
	header[4]= HI(tga->colourmaporigin);
	header[5]= LO(tga->colourmaplength);
	header[6]= HI(tga->colourmaplength);
	header[7]= LO(tga->colourmapdepth);
	header[8]= LO(tga->x_origin);
	header[9]= HI(tga->x_origin);
	header[10]= LO(tga->y_origin);
	header[11]= HI(tga->y_origin);
	header[12]= LO(tga->width);
	header[13]= HI(tga->width);
	header[14]= LO(tga->height);
	header[15]= HI(tga->height);
	header[16]= LO(tga->bitsperpixel);
	header[17]= LO(tga->imagedescriptor);

	n= fwrite(header, sizeof(unsigned char[18]), 1, out);

	free(header);

	return (n==1);
}


int tga_write(IMG *img, char *fname)
{
	TGA_HEADER tga;
	unsigned char *tmp, *data;
	FILE *out;
	size_t n;
	int i, y;
	int largeur;

	if(img->bits!=32 && img->bits!=24)
        {
		printf("\n-- invalid format '%s'\n", fname);
		return 0;
	}
	
	tga.idlength= 0;
	tga.colourmaptype= 0;
	tga.datatypecode= 2; /* rgb non compresse */
	tga.colourmaporigin= 0;
	tga.colourmaplength= 0;
	tga.colourmapdepth= 0;
	tga.x_origin= 0;
	tga.y_origin= 0;
	tga.width= img->largeur;
	tga.height= img->hauteur;
	tga.bitsperpixel= img->bits;
	tga.imagedescriptor= 0x20;

	out= fopen(fname, "wb");
	if(out==NULL)
	{
		printf("\n -- write error '%s'\n", fname);
		return 0;
	}

	n= ecrire_tga_entete(out, &tga);
	if(n!=1)
	{
		printf("\n -- write error '%s'\n", fname);
		return 0;
	}

	largeur= img->alignement;
	tmp= (unsigned char *) malloc(largeur);
	assert(tmp);

	data= img->data;
	for(y= 0; y < img->hauteur; y++)
	{	
		if(img->bits==24)
		{
			/* place les triplets rgb dans le bon ordre */
			for(i= 0; i<largeur; i+= 3)
			{
				tmp[i]=   data[i+2];
				tmp[i+1]= data[i+1];
				tmp[i+2]= data[i];
			}
		}
		else if(img->bits==32)
		{
			for(i= 0; i<largeur; i+= 4)
			{
				tmp[i]=   data[i+2];
				tmp[i+1]= data[i+1];
				tmp[i+2]= data[i];
				tmp[i+3]= data[i+3];
			}
		}
		
		n= fwrite(tmp, sizeof(unsigned char), largeur, out);
		if(n!= (size_t) largeur)
		{
                        printf("\n -- write error '%s'\n", fname);
			free(tmp);
			fclose(out);
			return 0;
		}
		
		data+= largeur;
	}

	free(tmp);
	fclose(out);
	
	return 1;
}

char *tga_basename(char *fn)
{
	char *ext;
	
	ext= strrchr(fn , '.');
	if(ext)
		if(strcmp(ext, ".tga")==0)
			*ext= 0;
	
	return fn;
}

char *num_tga_basename(char *fn)
{
    char *ext;
    char *dext;
    
    tga_basename(fn);
    ext= strrchr(fn, '.');
    if(ext)
    {
        for(dext= ext+1; *dext && isdigit(*dext); dext++)
            {;}

        if(*dext==0)
            *ext= 0;
    }
    
    return fn;
}

int is_tga(char *fn)
{
    char *ext;
    
    ext= strrchr(fn, '.');
    if(ext)
        if(strcmp(ext, ".tga")==0)
            return 1;
    
    return 0;
}

void checker_tga(IMG *img)
{
	int i, j, pix;
	
	pix= 0;
	for(j= 0; j<img->hauteur; j++)
	{
		for(i= 0; i<img->largeur; i++)
		{
			img->data[pix]= pix&0xFF;
			img->data[pix+1]= (pix>>8)&0xFF;
			img->data[pix+2]= (pix>>16)&0xFF;
			pix+= 3;
		}
	}
}


