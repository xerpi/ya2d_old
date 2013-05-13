#ifndef _YA2D_JPEG_H_
#define _YA2D_JPEG_H_

#include <pspsdk.h>
#include <malloc.h>
#include <pspiofilemgr.h>
#include <stdio.h>
#include <jpeglib.h>
#include "ya2d_texture.h"


int ya2d_loadJPEGfromFile(char *filename, ya2d_Texture *texp);



#endif