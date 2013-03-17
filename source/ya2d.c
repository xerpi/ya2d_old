#include "ya2d_main.h"

/* Variables */
    //GU
        unsigned int __attribute__((aligned(16))) ya2d_guList[262144];
        void *ya2d_fbp0 = NULL, *ya2d_fbp1 = NULL, *ya2d_zbp = NULL;
    //Frame counting
        u32    ya2d_frameCount;
        time_t ya2d_currentTime, ya2d_lastTime, ya2d_diffTime;
    //ya2d
        u8 ya2d_inited = 0;

/* Functions */


    int ya2d_init()
    {
        if(ya2d_inited) return 1;

        if(ya2d_fbp0 == NULL) ya2d_fbp0 = getStaticVramBuffer(YA2D_BUF_WIDTH,YA2D_SCR_HEIGHT,GU_PSM_8888);
        if(ya2d_fbp1 == NULL) ya2d_fbp1 = getStaticVramBuffer(YA2D_BUF_WIDTH,YA2D_SCR_HEIGHT,GU_PSM_8888);
        if(ya2d_zbp  == NULL) ya2d_zbp  = getStaticVramBuffer(YA2D_BUF_WIDTH,YA2D_SCR_HEIGHT,GU_PSM_4444);

		sceGuInit();
        sceGuStart(GU_DIRECT, ya2d_guList);
        sceGuDrawBuffer(GU_PSM_8888, ya2d_fbp0, YA2D_BUF_WIDTH);
        sceGuDispBuffer(YA2D_SCR_WIDTH, YA2D_SCR_HEIGHT, ya2d_fbp1, YA2D_BUF_WIDTH);
        sceGuDepthBuffer(ya2d_zbp, YA2D_BUF_WIDTH);
        sceGuOffset(2048 - (YA2D_SCR_WIDTH/2), 2048 - (YA2D_SCR_HEIGHT/2));
        sceGuViewport(2048, 2048, YA2D_SCR_WIDTH, YA2D_SCR_HEIGHT);
        sceGuDepthRange(65535, 0);
 
        sceGuScissor(0, 0, YA2D_SCR_WIDTH, YA2D_SCR_HEIGHT);

		sceGuEnable(GU_ALPHA_TEST);
		sceGuEnable(GU_SCISSOR_TEST);
		sceGuEnable(GU_BLEND);

		sceGuFrontFace(GU_CW);
		sceGuShadeModel(GU_SMOOTH);
		sceGuDisable(GU_TEXTURE_2D);

		sceGuAlphaFunc(GU_GREATER, 0, 255);
        sceGuDepthFunc(GU_LEQUAL);
        sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);		
 
        //setup texture
            sceGuTexMode(GU_PSM_8888,0,0,1);
            sceGuTexFunc(GU_TFX_DECAL, GU_TCC_RGBA);//apply as decal
            //sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
            sceGuTexFilter(GU_LINEAR, GU_LINEAR);       //linear good quality
            sceGuTexScale(1.0f,1.0f); //no scaling
            sceGuTexOffset(0.0f,0.0f);
 
        //sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
        sceGuFinish();
 
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuDisplay(GU_TRUE);
       
        ya2d_setupProjection();
        //Debug console
            pspDebugScreenInit();
            ya2d_updateConsole();

        ya2d_inited = 1;
        sceKernelDcacheWritebackAll();
        return 1;
    }


    int ya2d_deinit()
    {
        sceGuTerm();
        ya2d_inited = 0;
        return 1;
    }

	void ya2d_setupProjection()
	{
		sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumOrtho(0, YA2D_SCR_WIDTH, YA2D_SCR_HEIGHT, 0, -1, 1); //2D
		sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();
		sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
	}
	
    void ya2d_clearScreen(unsigned int color)
    {
        sceGuStart(GU_DIRECT, ya2d_guList);
        sceGuClearColor(color);
        sceGuClearDepth(color);
        sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
		ya2d_setupProjection();
    }

    void ya2d_flipScreen()
    {
        sceGuFinish();
        sceGuSync(0,0);
       // sceDisplayWaitVblankStart();
        ya2d_fbp0 = sceGuSwapBuffers();
        
    }

    void ya2d_updateConsole()
    {
        pspDebugScreenSetOffset((int)ya2d_fbp0);
        //pspDebugScreenClear(); //Reduces FPS to 30
        pspDebugScreenSetXY(0,0);
    }


