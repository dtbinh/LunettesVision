#ifndef _HDR_H
#define _HDR_H

#include "img.h"

extern int hdr_write(IMG *img, char *fname);
extern IMG *hdr_read(char *fname);
extern char *hdr_basename(char *fn);
extern int is_hdr(char *fn);

extern void img_hdr_normalise(IMG *hdr, float max);
extern void img_hdr_preview(IMG *hdr, float compression, float saturation);

#endif

