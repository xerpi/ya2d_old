#ifndef _YA2D_BITMAP_H_
#define _YA2D_BITMAP_H_

#include <pspsdk.h>
#include <malloc.h>
#include <pspiofilemgr.h>
#include "ya2d_texture.h"

//http://www.daubnet.com/en/file-format-bmp
//http://en.wikipedia.org/wiki/BMP_file_format
//http://www.tinaja.com/glib/expbmp.pdf


typedef struct
{
	uint8_t  signature[2];
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t dataOffset;
}__attribute__((packed)) ya2d_BMPfileHeader;

typedef struct
{
	uint32_t infoHeaderSize;
	uint32_t bitmapWidth;
	uint32_t bitmapHeight;
	uint16_t planes;
	uint16_t bitsPerPixel;
	uint32_t compression;
	uint32_t imageSize;
	uint32_t XpixelsPerMeter;
	uint32_t YpixelsPerMeter;
	uint32_t colorsUsed;
	uint32_t colorsImportant;
}__attribute__((packed)) ya2d_BMPfileInfoHeader;

typedef struct
{
	ya2d_BMPfileHeader     fileHeader;
	ya2d_BMPfileInfoHeader infoHeader;
}__attribute__((packed)) ya2d_BMPheader;



int ya2d_loadBMPfromFile(char *filename, ya2d_Texture *texp);
int ya2d_isBMP(ya2d_BMPheader *BMPheader);




#endif
