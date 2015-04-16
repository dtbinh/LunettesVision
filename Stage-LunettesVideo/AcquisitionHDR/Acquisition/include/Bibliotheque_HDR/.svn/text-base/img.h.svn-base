#ifndef _IMG_H
#define _IMG_H

typedef unsigned char PIX;

typedef struct _img
{
	int largeur;
	int hauteur;
	int channels;
	int palette_n;

        int alignement;
	int bits;
	
        int alignementf;
	int bitsf;
	
        PIX *palette;
	PIX *data;
	float *dataf;
} IMG;


#define LO(a) (unsigned char) ((a) & 0xFF)
#define HI(a) (unsigned char) ((a)>>8 & 0xFF)

#define int_HIHI(a) (unsigned char) ((a)>>24 & 0xFF)
#define int_HILO(a) (unsigned char) ((a)>>16 & 0xFF)
#define int_LOHI(a) (unsigned char) ((a)>>8 & 0xFF)
#define int_LOLO(a) (unsigned char) ((a) & 0xFF)

#define LOHI(l, h) (((int)(h)<<8) + (int)(l))
#define LOHI_int(ll, lh, hl, hh) (((int)(hh)<<24) + ((int)(hl)<<16) + ((int)(lh)<<8) + (int)(ll))

extern PIX color_clamp(float c);
extern IMG *new_img_datafloat(int w, int h);
extern IMG *new_img_data32(int w, int h);
extern IMG *new_img_data24(int w, int h);
extern IMG *new_img_palette256(int w, int h);
extern IMG *new_img_nodata(int w, int h);
extern void free_img(IMG *img);
extern void clear_img(IMG *img);
extern IMG *img_blit(IMG *src, int x, int y, int w, int h);
extern IMG *img_cpy(IMG *dst, IMG *src);

extern float *img_get_channel_datafloat(IMG *img, int channel);
extern PIX *img_get_channel_data(IMG *img, int channel);

extern void blit24(unsigned char *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    unsigned char *dst, int dst_w, int dst_h);
extern void blit24_alpha(unsigned char *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    unsigned char *dst, int dst_w, int dst_h);
extern void blitfloat(float *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    float *dst, int dst_w, int dst_h);

// conversion rgbfloat depuis rgb24 
IMG *img_rgbfloat_rgb24(IMG *dstf, IMG *src24);

// conversion directe depuis camera
extern IMG *img_rgb24_raw(IMG *dst, PIX *raw, int w, int h);

// + sous-echantillonnage 2x2
extern IMG *img_rgb24_raw_sub22(IMG *dst, PIX *raw, int w, int h);
extern IMG *img_rgb24_raw_sub22_fast(IMG *dst, PIX *raw, int w, int h);

//
extern void img_palette(IMG *img, int i, int r, int g, int b, int a);
extern IMG *img_rgb24_256(IMG *in);


#endif
