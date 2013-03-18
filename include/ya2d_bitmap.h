#ifndef _YA2D_BITMAP_H_
#define _YA2D_BITMAP_H_

//http://www.daubnet.com/en/file-format-bmp
//http://en.wikipedia.org/wiki/BMP_file_format
//http://www.tinaja.com/glib/expbmp.pdf

#include <pspsdk.h>
#include <malloc.h>
#include "ya2d_texture.h"


typedef struct
{
	uint8_t  signature[2];
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t dataOffset;
}__attribute__((packed)) ya2d_BITMAPFILEHEADER;

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
	

}__attribute__((packed)) ya2d_BITMAPFILEINFOHEADER;









#endif
