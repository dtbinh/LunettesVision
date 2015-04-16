#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "img.h"

PIX color_clamp(float c)
{
    if(c < 0.f)
        return 0;
    else if(c > 255.5f)
        return 255;
    else
        return (PIX) (c + .5f);
}


IMG *new_img_datafloat(int w, int h)
{
    IMG *img;

    img= (IMG *) malloc(sizeof(IMG));
    assert(img);

    img->largeur= w;
    img->hauteur= h;
    img->alignement= 0;
    img->bits= 0;
    img->alignementf= w * sizeof(float [3]);
    img->bitsf= sizeof(float [3]) * 8;
    img->channels= 3;

    img->palette= NULL;
    img->palette_n= 0;

    img->data= NULL;

    img->dataf= (float *) calloc(w * h, sizeof(float [3]));
    assert(img->dataf != NULL);

    return img;
}

IMG *new_img_datafloat_channels(int w, int h, int channels)
{
    IMG *img;

    img= (IMG *) malloc(sizeof(IMG));
    assert(img);

    img->largeur= w;
    img->hauteur= h;
    img->alignement= 0;
    img->bits= 0;
    img->alignementf= w * sizeof(float) * channels;
    img->bitsf= sizeof(float) * 8 * channels;
    img->channels= channels;
    
    img->palette= NULL;
    img->palette_n= 0;

    img->data= NULL;

    img->dataf= (float *) calloc(w * h, sizeof(float) * channels);
    assert(img->dataf != NULL);

    return img;
}

float *img_get_channel_datafloat(IMG *img, int channel)
{
    assert(img != NULL);
    assert(img->dataf!=NULL);
    assert(img->channels != 0);
    assert(channel >= 0 && channel < img->channels);
    
    return &img->dataf[channel];
}

PIX *img_get_channel_data(IMG *img, int channel)
{
    assert(img != NULL);
    assert(img->dataf!=NULL);
    assert(img->channels != 0);
    assert(channel >= 0 && channel < img->channels);
    
    return &img->data[channel];
}

IMG *new_img_data32(int w, int h)
{
    IMG *img;

    img= (IMG *) malloc(sizeof(IMG));
    assert(img);

    img->largeur= w;
    img->hauteur= h;
    img->alignement= w * sizeof(PIX[4]);
    img->bits= 32;
    img->alignementf= 0;
    img->bitsf= 0;
    img->channels= 4;

    img->palette= NULL;
    img->palette_n= 0;

    img->data= (unsigned char *) calloc(w * h, sizeof(unsigned char[4]));
    assert(img->data);

    img->dataf= NULL;

    return img;
}

IMG *new_img_data24(int w, int h)
{
    IMG *img;

    img= (IMG *) malloc(sizeof(IMG));
    assert(img);

    img->largeur= w;
    img->hauteur= h;
    img->alignement= w * sizeof(PIX[3]);
    img->bits= 24;
    img->alignementf= 0;
    img->bitsf= 0;
    img->channels= 3;

    img->palette= NULL;
    img->palette_n= 0;

    img->data= (unsigned char *) calloc(w * h, sizeof(unsigned char[3]));
    assert(img->data);

    img->dataf= NULL;

    return img;
}

IMG *new_img_palette256(int w, int h)
{
    IMG *img;

    img= (IMG *) malloc(sizeof(IMG));
    assert(img);

    img->largeur= w;
    img->hauteur= h;
    img->alignement= w;
    img->bits= 0;
    img->alignementf= 0;
    img->bitsf= 0;
    img->channels= 1;

    img->palette= (unsigned char *) calloc(256, sizeof(unsigned char[4]));
    assert(img->palette);
    img->palette_n= 256;

    img->data= (unsigned char *) calloc(w * h, sizeof(unsigned char));
    assert(img->data);

    img->dataf= NULL;

    return img;
}

// fixe une couleur de la palette d'une image 256 couleurs
void img_palette(IMG *img, int i, int r, int g, int b, int a)
{
    assert(img!=NULL);
    assert(img->palette!=NULL);

    if(img->palette_n > 0 && i < img->palette_n)
    {
        i= sizeof(unsigned char[4]) * i;
        img->palette[i]=    (unsigned char) r;
        img->palette[i +1]= (unsigned char) g;
        img->palette[i +2]= (unsigned char) b;
        img->palette[i +3]= (unsigned char) a;
    }
}


IMG *img_rgb24_256(IMG *in)
{
    IMG *img= new_img_data24(in->largeur, in->hauteur);
    int i;
    int size;

    if(in->palette_n > 0)
    {
        size= in->largeur * in->hauteur;
        for(i= 0; i<size; i++)
        {
            img->data[3*i]=    in->palette[4*in->data[i]];
            img->data[3*i +1]= in->palette[4*in->data[i] +1];
            img->data[3*i +2]= in->palette[4*in->data[i] +2];
        }
    }

    return img;
}


IMG *new_img_nodata(int w, int h)
{
    IMG *img;

    img= (IMG *) malloc(sizeof(IMG));
    assert(img);

    img->largeur= w;
    img->hauteur= h;
    img->alignement= 0;
    img->bits= 0;
    img->alignementf= 0;
    img->bitsf= 0;
    img->channels= 0;


    img->data= NULL;
    img->dataf= NULL;
    img->palette= NULL;
    img->palette_n= 0;

    return img;
}

void free_img(IMG *img)
{
    if(img!=NULL)
    {
        if(img->data != NULL)
            free(img->data);

        if(img->dataf != NULL)
            free(img->dataf);

        if(img->palette != NULL)
            free(img->palette);

        free(img);
    }
}


void clear_img(IMG *img)
{
    int n;
    
    if(img==NULL)
        return;

    n= img->channels*img->largeur*img->hauteur;
    if(img->palette_n==0)
    {
        if(img->data != NULL)
            memset(img->data, 0, sizeof(PIX) * n);
        if(img->dataf != NULL)
            memset(img->dataf, 0, sizeof(float) * n);
    }

    else if(img->palette_n==256)
    {
        if(img->data != NULL)
            memset(img->data, 0, sizeof(PIX) * n);
    }
}


void blit(unsigned char *src, int width, int height, 
    int sx, int sy, 
    int sw, int sh, 
    unsigned char *dst)
{
    int x, y;

    for(y= 0; y<sh; y++)
        for(x= 0; x<sw; x++)
            dst[y*sw + x]= src[(sy+y)*width + sx+x];
}

void blit24(unsigned char *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    unsigned char *dst, int dst_w, int dst_h)
{
    int x, y;
    int spix, dpix;

    for(y= 0; y < sh; y++)
    {
        dpix= 3*(y*dst_w);
        spix= 3*((sy+y)*src_w + sx);

        for(x= 0; x < sw; x++)
        {
            dst[dpix]= src[spix];
            dst[dpix +1]= src[spix +1];
            dst[dpix +2]= src[spix +2];

            dpix+= 3;
            spix+= 3;
        }
    }
}


void blit32(unsigned char *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    unsigned char *dst, int dst_w, int dst_h)
{
    int x, y;
    int spix, dpix;

    for(y= 0; y < sh; y++)
    {
        dpix= 4*(y*dst_w);
        spix= 4*((sy+y)*src_w + sx);

        for(x= 0; x < sw; x++)
        {
            dst[dpix]= src[spix];
            dst[dpix +1]= src[spix +1];
            dst[dpix +2]= src[spix +2];
            dst[dpix +3]= src[spix +3];

            dpix+= 4;
            spix+= 4;
        }
    }
}


void blit24_alpha(unsigned char *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    unsigned char *dst, int dst_w, int dst_h)
{
    int x, y;
    int spix, dpix;

    for(y= 0; y < sh; y++)
    {
        dpix= 3*(y*dst_w);
        spix= 4*((sy+y)*src_w + sx);

        for(x= 0; x < sw; x++)
        {
            dst[dpix]= src[spix];
            dst[dpix +1]= src[spix +1];
            dst[dpix +2]= src[spix +2];

            dpix+= 3;
            spix+= 4;
        }
    }
}

void blitfloat(float *src, int src_w, int src_h, 
    int sx, int sy, int sw, int sh, 
    float *dst, int dst_w, int dst_h)
{
    int x, y;
    int spix, dpix;

    for(y= 0; y < sh; y++)
    {
        dpix= 3*(y*dst_w);
        spix= 3*((sy+y)*src_w + sx);

        for(x= 0; x < sw; x++)
        {
            dst[dpix]= src[spix];
            dst[dpix +1]= src[spix +1];
            dst[dpix +2]= src[spix +2];

            dpix+= 3;
            spix+= 3;
        }
    }
}


IMG *img_blit(IMG *src, int x, int y, int w, int h)
{
    IMG *img= NULL;

    if(src->palette_n > 0)
    {
        img= new_img_palette256(w, h);

        memcpy(img->palette, src->palette, sizeof(unsigned char [4]) * 256);
        blit(src->data, src->largeur, src->hauteur, 
            x, y, w, h, 
            img->data);
    }
    else
    {
        if(src->bits==24)
        {
            img= new_img_data24(w, h);
            blit24(src->data, src->largeur, src->hauteur, 
                x, y, w, h, 
                img->data, img->largeur, img->hauteur);
        }

        else if(src->bits==32)
        {
            img= new_img_data32(w, h);
            blit32(src->data, src->largeur, src->hauteur, 
                x, y, w, h, 
                img->data, img->largeur, img->hauteur);
        }

        else if(src->bits==sizeof(float [3]) * 8)
        {
            img= new_img_datafloat(w, h);
            blitfloat(src->dataf, src->largeur, src->hauteur, 
                x, y, w, h, 
                img->dataf, img->largeur, img->hauteur);
        }
    }

    return img;
}


IMG *img_cpy(IMG *dst, IMG *src)
{
    if(src->palette_n > 0)
    {
        if(dst==NULL)
            dst=new_img_palette256(src->largeur, src->hauteur);
        assert(dst->palette!=NULL);

        memcpy(dst->palette, src->palette, sizeof(unsigned char [4]) * 256);
        blit(src->data, src->largeur, src->hauteur, 
            0, 0, src->largeur, src->hauteur, 
            dst->data);
    }
    else
    {
        if(src->bits==24 && src->bitsf==0)
        {
            if(dst==NULL)
                dst=new_img_data24(src->largeur, src->hauteur);

            blit24(src->data, src->largeur, src->hauteur, 
                0, 0, src->largeur, src->hauteur, 
                dst->data, dst->largeur, dst->hauteur);
        }

        else if(src->bits==32  && src->bitsf==0)
        {
            if(dst==NULL)
                dst=new_img_data32(src->largeur, src->hauteur);

            blit32(src->data, src->largeur, src->hauteur, 
                0, 0, src->largeur, src->hauteur, 
                dst->data, dst->largeur, dst->hauteur);
        }

        else if(src->bitsf==sizeof(float [3]) * 8 && src->bits==0)
        {
            if(dst==NULL)
                dst=new_img_datafloat(src->largeur, src->hauteur);

            blitfloat(src->dataf, src->largeur, src->hauteur, 
                0, 0, src->largeur, src->hauteur, 
                dst->dataf, dst->largeur, dst->hauteur);
        }
        else
            printf("%s: not implemented !\n", __FUNCTION__);
    }

    return dst;
}


IMG *img_rgbfloat_rgb24(IMG *dstf, IMG *src24)
{
    int x, y;
    int spix, dpix;

    assert(src24->bits==24);
    if(dstf==NULL)
        dstf= new_img_datafloat(src24->largeur, src24->hauteur);
    assert(dstf!=NULL);
    assert(dstf->channels==3);
    if(dstf->dataf==NULL)
    {
            dstf->dataf= (float *) malloc(sizeof(float [3]) * dstf->largeur*dstf->hauteur);
            assert(dstf->dataf!=NULL);
            dstf->bitsf= sizeof(float [3]) * 8;
            dstf->alignementf= dstf->largeur * sizeof(float [3]);
    }

    for(y= 0; y < src24->hauteur; y++)
    {
        dpix= 3*(y*dstf->largeur);
        spix= 3*(y*src24->largeur);

        for(x= 0; x < src24->largeur; x++)
        {
            dstf->dataf[dpix]= (float) src24->data[spix];
            dstf->dataf[dpix +1]= (float) src24->data[spix +1];
            dstf->dataf[dpix +2]= (float) src24->data[spix +2];

            dpix+= 3;
            spix+= 3;
        }
    }

    return dstf;
}

IMG *img_rgb24_raw(IMG *dst, PIX *raw, int w, int h)
{
    int x, y;
    PIX *rpix;
    PIX *dpix;

    for(y= 0; y < h; y++)
    {
        dpix= &dst->data[y * dst->largeur * 3];
        rpix= &raw[y * w * 3];

        for(x= 0; x < w; x++)
        {
            dpix[0]= rpix[2];
            dpix[1]= rpix[0];
            dpix[2]= rpix[1];

            dpix+= 3;
            rpix+= 3;
        }
    }

    return dst;
}

IMG *img_rgb24_raw_sub22_fast(IMG *dst, PIX *raw, int w, int h)
{
    int x, y;
    PIX *rpix;
    PIX *dpix;

    h/= 2;
    for(y= 0; y < h; y++)
    {
        dpix= &dst->data[y * dst->largeur * 3];
        rpix= &raw[2*y * w * 3];

        for(x= 0; x < w; x+= 2)
        {
            dpix[0]= rpix[2];
            dpix[1]= rpix[1];
            dpix[2]= rpix[0];

            dpix+= 3;
            rpix+= 2*3;
        }
    }

    return dst;
}


// sous echantillonne par blocs 2x2
IMG *img_rgb24_raw_sub22(IMG *dst, PIX *raw, int w, int h)
{
    unsigned int rm, rm0, rm1, rmw0, rmw1, rmin, rmax;
    int x, y;
    PIX *rpix;
    PIX *dpix;

    h/= 2;
    for(y= 0; y < h; y++)
    {
        dpix= &dst->data[y * dst->largeur * sizeof(PIX[3])];
        rpix= &raw[2*y * w * sizeof(PIX[3])];

        for(x= 0; x < w; x+= 2)
        {
            // filtre median / moyenne sur R
            rm0= (unsigned int) rpix[0];
            rm1= (unsigned int) rpix[3];
            rmw0= (unsigned int) rpix[3*w];
            rmw1= (unsigned int) rpix[3*w +3];

            rm= rm0 + rm1 + rmw0 + rmw1;

            rmin= rm0;
            rmax= rm0;

            if(rm1 < rmin)
                    rmin= rm1;
            else if(rm1 > rmax)
                    rmax= rm1;

            if(rmw0 < rmin)
                    rmin= rmw0;
            else if(rmw0 > rmax)
                    rmax= rmw0;

            if(rmw1 < rmin)
                    rmin= rmw1;
            else if(rmw1 > rmax)
                    rmax= rmw1;

            dpix[2]= (rm - rmin - rmax) / 2;

            // filtre median / moyenne sur G
            rm0= (unsigned int) rpix[0 +1];
            rm1= (unsigned int) rpix[3 +1];
            rmw0= (unsigned int) rpix[3*w +1];
            rmw1= (unsigned int) rpix[3*w +3 +1];

            rm= rm0 + rm1 + rmw0 + rmw1;

            rmin= rm0;
            rmax= rm0;

            if(rm1 < rmin)
                    rmin= rm1;
            else if(rm1 > rmax)
                    rmax= rm1;

            if(rmw0 < rmin)
                    rmin= rmw0;
            else if(rmw0 > rmax)
                    rmax= rmw0;

            if(rmw1 < rmin)
                    rmin= rmw1;
            else if(rmw1 > rmax)
                    rmax= rmw1;

            dpix[1]= (rm - rmin - rmax) / 2;

            // filtre median / moyenne sur B
            rm0= (unsigned int) rpix[0 +2];
            rm1= (unsigned int) rpix[3 +2];
            rmw0= (unsigned int) rpix[3*w +2];
            rmw1= (unsigned int) rpix[3*w +3 +2];

            rm= rm0 + rm1 + rmw0 + rmw1;

            rmin= rm0;
            rmax= rm0;

            if(rm1 < rmin)
                    rmin= rm1;
            else if(rm1 > rmax)
                    rmax= rm1;

            if(rmw0 < rmin)
                    rmin= rmw0;
            else if(rmw0 > rmax)
                    rmax= rmw0;

            if(rmw1 < rmin)
                    rmin= rmw1;
            else if(rmw1 > rmax)
                    rmax= rmw1;

            dpix[0]= (rm - rmin - rmax) / 2;

            dpix+= 3;
            rpix+= 2*3;
        }
    }

    return dst;
}

