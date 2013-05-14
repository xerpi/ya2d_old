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


typedef struct
{
	unsigned int select   : 3;
	unsigned int start    : 1;
	unsigned int up       : 1;
	unsigned int right    : 1;
	unsigned int down     : 1;
	unsigned int left     : 1;
	unsigned int ltrigger : 1;
	unsigned int rtrigger : 3;
	unsigned int triangle : 1;
	unsigned int circle   : 1;
	unsigned int cross    : 1;
	unsigned int square   : 1;
	unsigned int home     : 1;
	unsigned int hold     : 1;
	unsigned int wlan_up  : 1;
	unsigned int remote   : 3;
	unsigned int screen   : 1;
	unsigned int volup    : 1;
	unsigned int voldown  : 1;
	unsigned int disc     : 1;
	unsigned int ms       : 1;
}__attribute__ ((packed)) ya2d_Buttons_t;

struct ya2d_Controls_t
{
	ya2d_Buttons_t*  held;
	ya2d_Buttons_t*  pressed;
	ya2d_Buttons_t*  released;
	unsigned char*   analogx;
	unsigned char*   analogy;
};

struct ya2d_Controls_t ya2d_Controls;


#define ya2d_micros() sceKernelLibcClock()
#define ya2d_millis() (sceKernelLibcClock()/1000)

int ya2d_init();
int ya2d_deinit();

void ya2d_clearScreen(unsigned int color);
void ya2d_flipScreen();
void ya2d_readControls();
inline float ya2d_getFPS();
void ya2d_updateConsole();
void ya2d_setupProjection();
void ya2d_error(char *t, ...);


#endif
