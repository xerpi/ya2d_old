#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ya2d.h"

#define millis() (clock()/1000)

PSP_MODULE_INFO("ya2d", PSP_MODULE_USER, 1, 1);
PSP_HEAP_SIZE_MAX();

void initFPS();
void FPS(float *fps_p);

void drawRectangle(short x, short y, short w, short h, unsigned int color);

int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);

int main(int argc, char *argv[])
{
	SetupCallbacks();
	ya2d_init();

	ya2d_Texture tex;
	if(!ya2d_loadPNGfromFile("ms0:/test.png", &tex))
	{
		ya2d_error("Error loading ms0:/test.png");
	}

	while(1)
	{   
        ya2d_clearScreen(0);
		//printf("FPS: %f   frame: %i diffTime: %lu", fps, frame_count, diffTime);
		printf("image width: %i\n", tex.imageWidth);

		ya2d_drawFillRect(10, 10, 50, 20, GU_RGB(255,0,0));
		ya2d_drawRect(109, 140, 20, 80, GU_RGB(0,0,255));

		ya2d_drawTexture(&tex, 50, 10);

		ya2d_flipScreen();
		ya2d_updateConsole();
		//FPS(&fps);
	}
    ya2d_freeTexture(&tex);
	ya2d_deinit();
	sceKernelExitGame();
	return 0;
}



/*
void initFPS()
{
	currentTime = millis();
	lastTime = currentTime;
	frame_count = 0;
}

void FPS(float *fps_p)
{
	frame_count++;
	currentTime = millis();
	diffTime = https://github.com/xerpi/Kamera
	if(diffTime >= 1000)
	{
		*fps_p = (float)frame_count/(diffTime/1000.0f);
		frame_count = 0;
		lastTime = millis();
	}
}
*/

/*callbacks*/

int exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

int CallbackThread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("exit_callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("callback_update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}
