#ifndef _YA2D_PNG_H_
#define _YA2D_PNG_H_

#include <pspsdk.h>
#include <pspsdk.h>
#include <malloc.h>
#include "ya2d_texture.h"

ya2d_Texture* ya2d_loadPNGfromFile(char *filename, ya2d_Place place);
void _ya2d_png_read_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead);





#endif
