#ifndef _YA2D_H_
#define _YA2D_H_

#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <pspgum.h>
#include <vram.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <png.h>
#include <malloc.h>
#include <pspkernel.h>

#ifndef GU_RGB
	#define GU_RGB(r,g,b) GU_RGBA(r,g,b,255)
#endif

#ifndef printf
	#define printf pspDebugScreenPrintf
#endif

#define YA2D_BUF_WIDTH  512
#define YA2D_SCR_WIDTH  480
#define YA2D_SCR_HEIGHT 272

#define YA2D_PNG_SIG_LEN 8

typedef struct
{
	unsigned int color;
	short x, y, z;
} ya2d_FastVertex;

typedef struct
{
    unsigned short u, v;
    short x, y, z;
} ya2d_TextureVertex;

typedef struct
{
	uint32_t imageWidth, imageHeight;
	uint32_t textureWidth, textureHeight;
	uint32_t centerX, centerY;
    uint32_t rowBytes, dataLength;
	uint8_t __attribute__((aligned(16))) *data;
	int bitDepth, colorType;
	uint8_t hasAlpha;
}ya2d_Texture;


int ya2d_init();
int ya2d_deinit();

void ya2d_clearScreen(unsigned int color);
void ya2d_flipScreen();
void ya2d_updateConsole();
void ya2d_setupProjection();

void ya2d_drawRect(int x, int y, int w, int h, u32 color);
void ya2d_drawFillRect(int x, int y, int w, int h, u32 color);

int ya2d_loadPNGfromFile(char *filename, ya2d_Texture *texp);
void ya2d_drawTexture(ya2d_Texture *texp, int x, int y);
void ya2d_freeTexture(ya2d_Texture *texp);
void _ya2d_png_read_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead);

void ya2d_error(char *error_txt);


#endif
