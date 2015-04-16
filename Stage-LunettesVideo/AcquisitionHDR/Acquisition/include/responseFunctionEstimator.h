#ifndef _RESPONSE_FUNCTION_ESTIMATOR_H
#define _RESPONSE_FUNCTION_ESTIMATOR_H

void afficher_image(IplImage *src);
void allocate_vars(const int nimg, const int NbPt);
void luminance_from_color(const IplImage *src, IplImage *dst);
void lecture_multi_images(const int nimg, IplImage **images);
void tirage_point(const int NbPt, int **tab_point, const int width ,const int height);
void lecture_pixel_images(const int nimg, IplImage **images, const float *Te, int **Zr , int **Zg , int **Zb , float **Br, float **Bg, float **Bb, const int NbPt);
void lecture_pixel_images_luminance(const int nimg, IplImage **images, const float *Te, int **Zr , float **Br, const int NbPt);
void load_Te();
void save_G_func_channel(const char file[256], float gfunction[256]);
void load_G_func_channel(const char file[256], float gfunction[256]);
void lecture_pixel_supervisee(const int nimg, IplImage **images, const float *Te, int **Zr , float **Br, int *NbPoint);

#endif
