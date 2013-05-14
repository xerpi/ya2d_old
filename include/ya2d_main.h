#ifndef _YA2D_MAIN_H_
#define _YA2D_MAIN_H_

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
#include <malloc.h>
#include <math.h>
#include <stdarg.h>

#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdio.h>
#include <stdlib.h>
#include <psprtc.h>


#include "ya2d_globals.h"


#define ya2d_micros() sceKernelLibcClock()
#define ya2d_millis() (sceKernelLibcClock()/1000)

int ya2d_init();
int ya2d_deinit();

void ya2d_clearScreen(unsigned int color);
void ya2d_flipScreen();
inline float ya2d_getFPS();
void ya2d_updateConsole();
void ya2d_setupProjection();
void ya2d_error(char *t, ...);


#endif
