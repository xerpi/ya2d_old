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

//Images
	extern unsigned char test_png_start[];
	extern u32 test_png_size;

	extern unsigned char test2_png_start[];
	extern u32 test2_png_size;

	extern unsigned char test3_png_start[];
	extern u32 test3_png_size;
	
	extern unsigned char jpegimg_jpg_start[];
	extern u32 jpegimg_jpg_size;
	

PSP_MODULE_INFO("ya2d", PSP_MODULE_USER, 1, 1);
PSP_HEAP_SIZE_MAX();

int main(int argc, char *argv[])
{
	SetupCallbacks();
	ya2d_init();

	ya2d_Texture *tex1, *tex2, *tex3, *jpeg;
	
	/*
	if(! (tex1 = ya2d_loadPNGfromFile("ms0:/test.png", YA2D_VRAM))) //non base 2 size
		ya2d_error("Error loading ms0:/test.png");
	if(! (tex2 = ya2d_loadPNGfromFile("ms0:/test2.png", YA2D_VRAM)))
		ya2d_error("Error loading ms0:/test2.png");
	if(! (tex3 = ya2d_loadPNGfromFile("ms0:/test3.png", YA2D_VRAM)))
		ya2d_error("Error loading ms0:/test3.png");
	if(! (jpeg = ya2d_loadJPEGfromFile("ms0:/jpegimg.jpg", YA2D_VRAM)))
		ya2d_error("Error loading ms0:/jpegimg.jpg");
	*/
	
	if(! (tex1 = ya2d_loadPNGfromBuffer(test_png_start, YA2D_VRAM)))
		ya2d_error("Error loading test.png, start: %i   size: %i", test_png_start, test_png_size);
	if(! (tex2 = ya2d_loadPNGfromBuffer(test2_png_start, YA2D_VRAM)))
		ya2d_error("Error loading test2.png");
	if(! (tex3 = ya2d_loadPNGfromBuffer(test3_png_start, YA2D_VRAM)))
		ya2d_error("Error loading test3.png");
	if(! (jpeg = ya2d_loadJPEGfromBuffer(jpegimg_jpg_start, jpegimg_jpg_size, YA2D_VRAM)))
		ya2d_error("Error loading jpegimg.jpg");

	ya2d_swizzleTexture(tex1);
	ya2d_swizzleTexture(tex2);
	ya2d_swizzleTexture(tex3);
	ya2d_swizzleTexture(jpeg);

	float angle = 0.0f;
	int x = 270, y = 40;
	while(1)
	{   
        ya2d_clearScreen(0xFFFFFFFF); //white
		printf("FPS: %.2f   angle: %f  ", ya2d_getFPS(), angle);
		printf("vram: %.2f KB", ((float)vlargestblock()/1024));


		ya2d_drawRotateTexture(jpeg, 20, 10, angle);
		ya2d_drawRotateTexture(tex1, 250, 60, angle);
		ya2d_drawRotateTexture(tex2, 20, 10, angle);
		ya2d_drawRotateTexture(tex3, x,y, angle);
		ya2d_drawRect(x, y, 64,64,0xFF0000FF);
		
		angle += 0.01f;
		ya2d_flipScreen();
		ya2d_updateConsole();
	}
	
    ya2d_freeTexture(tex1);
    ya2d_freeTexture(tex2);
    ya2d_freeTexture(tex3);
    ya2d_freeTexture(jpeg);
	ya2d_deinit();
	sceKernelExitGame();
	return 0;
}

