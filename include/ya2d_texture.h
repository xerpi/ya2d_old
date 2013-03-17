#ifndef _YA2D_TEXTURE_H_
#define _YA2D_TEXTURE_H_


#include <pspgu.h>
#include <pspgum.h>
#include <vram.h>
#include <png.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <malloc.h>

#include "ya2d_main.h"
#include "ya2d_utils.h"
#include "ya2d_globals.h"

#define YA2D_TEXTURE_SLICE 32

typedef struct
{
	unsigned int color;
	short x, y, z;
} ya2d_FastVertex;

typedef struct
{
	unsigned int color;
	float x, y, z;
} ya2d_FloatVertex;

typedef struct
{
    unsigned short u, v;
    short x, y, z;
} ya2d_TextureVertex;

typedef struct
{
    float u, v;
    float x, y, z;
} ya2d_FloatTextureVertex;

typedef struct
{
	uint32_t imageWidth, imageHeight;
	uint32_t textureWidth, textureHeight;
	uint32_t centerX, centerY;
    uint32_t rowBytes, dataLength;
	uint8_t __attribute__((aligned(16))) *data;
	int bitDepth, colorType;
	uint8_t hasAlpha, isSwizzled;
}ya2d_Texture;

int ya2d_loadPNGfromFile(char *filename, ya2d_Texture *texp);
void ya2d_swizzleTexture(ya2d_Texture *texp);
void ya2d_drawTexture(ya2d_Texture *texp, int x, int y);
void ya2d_drawRotateTexture(ya2d_Texture *texp, int x, int y, float angle);
void ya2d_freeTexture(ya2d_Texture *texp);
void _ya2d_png_read_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead);

#endif
