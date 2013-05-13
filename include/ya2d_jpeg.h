#ifndef _YA2D_JPEG_H_
#define _YA2D_JPEG_H_

#include <pspsdk.h>
#include <malloc.h>
#include <pspiofilemgr.h>
#include <stdio.h>
#include <jpeglib.h>
#include "ya2d_texture.h"


ya2d_Texture* ya2d_loadJPEGfromFile(char *filename, ya2d_Place place);
ya2d_Texture* ya2d_loadJPEGfromBuffer(void *buffer, u32 buffer_size, ya2d_Place place);


#endif
