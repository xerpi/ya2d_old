#ifndef _YA2D_DRAW_H_
#define _YA2D_DRAW_H_

#include <pspgu.h>
#include <pspgum.h>
#include <vram.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <malloc.h>

#include "ya2d_main.h"
#include "ya2d_texture.h"
#include "ya2d_globals.h"

void ya2d_drawRect(int x, int y, int w, int h, u32 color);
void ya2d_drawFillRect(int x, int y, int w, int h, u32 color);


#endif
