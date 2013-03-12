#include "ya2d.h"

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
        sceGuEnable(GU_SCISSOR_TEST);
        sceGuFrontFace(GU_CW);
        sceGuShadeModel(GU_SMOOTH);
        sceGuDisable(GU_TEXTURE_2D);
 
        sceGuAlphaFunc(GU_GREATER, 0, 255);
        sceGuDepthFunc(GU_LEQUAL);
        sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
        sceGuDisable(GU_CULL_FACE);
        sceGuDisable(GU_CLIP_PLANES);
        sceGuDisable(GU_DITHER);
        sceGuEnable(GU_ALPHA_TEST);
        sceGuEnable(GU_SCISSOR_TEST);
        sceGuEnable(GU_BLEND);
 
        //setup texture
            sceGuTexMode(GU_PSM_8888,0,0,0);
            sceGuTexFunc(GU_TFX_DECAL, GU_TCC_RGBA);//apply as decal
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
        sceDisplayWaitVblankStart();
        ya2d_fbp0 = sceGuSwapBuffers();
    }

    void ya2d_updateConsole()
    {
        pspDebugScreenSetOffset((int)ya2d_fbp0);
        pspDebugScreenClear();
        //pspDebugScreenSetXY(0,0);
    }

    void ya2d_drawRect(int x, int y, int w, int h, u32 color)
    {
		sceGuDisable(GU_TEXTURE_2D);
        ya2d_FastVertex *vertices = (ya2d_FastVertex *)sceGuGetMemory(5 * sizeof(ya2d_FastVertex));

        vertices[0] = (ya2d_FastVertex){color, x, y};
        vertices[1] = (ya2d_FastVertex){color, x+w, y};
        vertices[2] = (ya2d_FastVertex){color, x+w, y+h};
        vertices[3] = (ya2d_FastVertex){color, x, y+h};
        vertices[4] = vertices[0];

        sceGuDrawArray(GU_LINE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 5, 0, vertices);

        sceKernelDcacheWritebackRange(vertices, 5 * sizeof(ya2d_FastVertex));
    }

    void ya2d_drawFillRect(int x, int y, int w, int h, u32 color)
    {
		sceGuDisable(GU_TEXTURE_2D);
        ya2d_FastVertex *vertices = (ya2d_FastVertex *)sceGuGetMemory(4 * sizeof(ya2d_FastVertex));

        vertices[0] = (ya2d_FastVertex){color, x, y};
        vertices[1] = (ya2d_FastVertex){color, x, y+h};
        vertices[2] = (ya2d_FastVertex){color, x+w, y};
        vertices[3] = (ya2d_FastVertex){color, x+w, y+h};

        sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);

        sceKernelDcacheWritebackRange(vertices, 4 * sizeof(ya2d_FastVertex));
    }

    int ya2d_loadPNGfromFile(char *filename, ya2d_Texture *texp)
    {
        uint8_t      header[YA2D_PNG_SIG_LEN];
        png_structp  png_ptr  = NULL;
        png_infop    info_ptr = NULL;
        SceUID       fp;

        if(!(fp = sceIoOpen(filename, PSP_O_RDONLY, 0777)))
            goto exit_error;

        sceIoRead(fp, header, YA2D_PNG_SIG_LEN);
        if (png_sig_cmp((png_bytep)header, 0, YA2D_PNG_SIG_LEN))
            goto exit_close;

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(png_ptr == NULL)
            goto exit_close;

        info_ptr = png_create_info_struct(png_ptr);
        if(info_ptr == NULL)
            goto exit_destroy;

        if(setjmp(png_jmpbuf(png_ptr)))
            goto exit_destroy;

        //ya2d_freeTexture(texp);

        png_set_read_fn(png_ptr, (void *)&fp, (png_rw_ptr)_ya2d_png_read_fn);
        png_set_sig_bytes(png_ptr, YA2D_PNG_SIG_LEN);
        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &texp->imageWidth, &texp->imageHeight, &texp->bitDepth, &texp->colorType, NULL, NULL, NULL);

        texp->textureWidth  = (texp->imageWidth);
        texp->textureHeight = (texp->imageHeight);

        if(texp->bitDepth == 16)
        {
            png_set_strip_16(png_ptr);
            texp->bitDepth = 8;
        }
        else if(texp->bitDepth < 8) //1 byte per pixel component
        {
            png_set_packing(png_ptr);
            texp->bitDepth = 8;
        }

        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
        switch(texp->colorType)
        {
            case PNG_COLOR_TYPE_RGB_ALPHA:
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                texp->hasAlpha = 1;
                break;
            case PNG_COLOR_TYPE_PALETTE:
                png_set_palette_to_rgb(png_ptr);
                texp->hasAlpha = 0;
                break;
            case PNG_COLOR_TYPE_GRAY:
                if (texp->bitDepth < 8)
                {
                    png_set_expand_gray_1_2_4_to_8(png_ptr);
                    texp->bitDepth = 8;
                }
                break;
            default:
                texp->hasAlpha = 0;
                break;
        }
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
        png_read_update_info (png_ptr, info_ptr);

        texp->rowBytes = png_get_rowbytes(png_ptr, info_ptr);
        texp->dataLength = texp->rowBytes * texp->textureHeight;
        texp->data = (uint8_t *)memalign(16, texp->dataLength * 4);

        //memset(texp->data, 0x0, texp->dataLength);
        u32 *line = (u32*) malloc(texp->rowBytes);
        int x, y;
        for (y = 0; y < texp->imageHeight; y++)
        {
            png_read_row(png_ptr, (u8*) line, NULL);
            for (x = 0; x < texp->imageWidth; x++)
            {
                u32 color = line[x];
                texp->data[x + y * texp->textureWidth] =  color;
            }
        }

        free(line);

        png_read_end(png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        sceIoClose(fp);
        return 1;

    exit_destroy:
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    exit_close:
        sceIoClose(fp);
    exit_error:
        return 0;
    }


    void ya2d_freeTexture(ya2d_Texture *texp)
    {
        if(texp->data != NULL)
        {
            free(texp->data);
            texp->data = NULL;
        }
    }

    void _ya2d_png_read_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead)
    {
        SceUID *fp = (SceUID *)png_get_io_ptr(png_ptr);
        if(fp == NULL)
            return;
        uint32_t bytesReaded = sceIoRead((SceUID)*fp, (void*)buffer, bytesToRead);
        if(bytesReaded != bytesToRead)
            return;
    }

    void ya2d_drawTexture(ya2d_Texture *texp, int x, int y)
    {
        if(!texp->data) return;

		sceGuEnable(GU_TEXTURE_2D);
		
		if(texp->hasAlpha)
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
		else
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);

        sceGuTexImage(0, texp->textureWidth, texp->textureHeight, texp->textureWidth, texp->data);

        ya2d_TextureVertex *vertices = (ya2d_TextureVertex *)sceGuGetMemory(2 * sizeof(ya2d_TextureVertex));

        vertices[0] = (ya2d_TextureVertex){0, 0, x, y};
        vertices[1] = (ya2d_TextureVertex){texp->imageWidth, texp->imageHeight, x+texp->imageWidth, y+texp->imageHeight};

        sceGumDrawArray(GU_SPRITES, GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);

        sceKernelDcacheWritebackRange(vertices, 2 * sizeof(ya2d_TextureVertex));
    }

    void ya2d_error(char *error_txt)
    {
		ya2d_clearScreen(GU_RGB(0, 50, 255)); //BSoD
		printf(error_txt);
		printf("   -   Press HOME to exit.");
		ya2d_flipScreen();
		ya2d_updateConsole();
		while(1)
		{
			sceKernelDelayThread(1000);
		}
	}


