#ifndef _TGA_H
#define _TGA_H

#include "img.h"

typedef struct
{
	unsigned char idlength;
	unsigned char  colourmaptype;
	unsigned char  datatypecode;
	short int colourmaporigin;
	short int colourmaplength;
	unsigned char  colourmapdepth;
	short int x_origin;
	short int y_origin;
	short int width;
	short int height;
	unsigned char  bitsperpixel;
	unsigned char  imagedescriptor;
} TGA_HEADER; // 18B

extern int tga_write(IMG *img, char *fname);
extern IMG *tga_read(char *fname);

extern char *tga_basename(char *fn);
extern char *num_tga_basename(char *fn);
extern int is_tga(char *fn);

extern void checker_tga(IMG *img);

#endif
