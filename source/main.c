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
#include "utils.h"

u32 frames;
u64 current_millis, last_millis, delta_millis;

PSP_MODULE_INFO("ya2d", PSP_MODULE_USER, 1, 1);
PSP_HEAP_SIZE_MAX();

int main(int argc, char *argv[])
{
	SetupCallbacks();
	ya2d_init();
	initFPS();

	ya2d_Texture tex1, tex2, tex3, bmp;
	
	if(!ya2d_loadPNGfromFile("ms0:/test.png", &tex1)) //non base 2 size
		ya2d_error("Error loading ms0:/test.png");
	if(!ya2d_loadPNGfromFile("ms0:/test2.png", &tex2))
		ya2d_error("Error loading ms0:/test2.png");
	if(!ya2d_loadPNGfromFile("ms0:/test3.png", &tex3))
		ya2d_error("Error loading ms0:/test3.png");

	if(!ya2d_loadBMPfromFile("ms0:/bmptest.bmp", &bmp))
		ya2d_error("Error loading ms0:/bmptest.bmp");
		
	float angle = 0.0f, fps = 0.0f;
	ya2d_swizzleTexture(&tex3);
	while(1)
	{   
        ya2d_clearScreen(0xFFFFFFFF); //white
		printf("FPS: %.2f   angle: %f\n", fps, angle);
		printf("frames: %2i, current: %llu, last: %llu  delta: %llu", frames, current_millis, last_millis, delta_millis);

		ya2d_drawFillRect(40, 140, 50, 20, GU_RGB(255,0,0));
		ya2d_drawRect(109, 140, 20, 80, GU_RGB(0,0,255));
		
		//ya2d_drawRotateTexture(&tex3, 50, 60, angle);
		ya2d_drawTextureFast(&tex3, 150, 10);
		//ya2d_drawRotateTexture(&tex1, 240, 60, angle+3.0f);	

		angle += 0.15f;
		ya2d_flipScreen();
		ya2d_updateConsole();
		FPS(&fps);
	}
    ya2d_freeTexture(&tex1);
    ya2d_freeTexture(&tex2);
    ya2d_freeTexture(&tex3);
    ya2d_freeTexture(&bmp);
	ya2d_deinit();
	sceKernelExitGame();
	return 0;
}

