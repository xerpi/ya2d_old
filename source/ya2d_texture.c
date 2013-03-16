#include "ya2d_texture.h"

  int ya2d_loadPNGfromFile(char *filename, ya2d_Texture *texp)
    {
        uint8_t      header[YA2D_PNG_SIG_LEN];
        png_structp  png_ptr  = NULL;
        png_infop    info_ptr = NULL;
        SceUID       fp;
        png_bytep *rowPointers;
		int i;

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

        /* */

		texp->textureWidth  = next_pow2(texp->imageWidth);
		texp->textureHeight = next_pow2(texp->imageHeight);

		texp->centerX = (int)(texp->imageWidth/2);
		texp->centerY = (int)(texp->imageHeight/2);

        texp->rowBytes = texp->textureWidth * 4; //only if pow2 -> png_get_rowbytes(png_ptr, info_ptr);
        texp->dataLength = texp->rowBytes * texp->textureHeight;
        texp->data = (uint8_t *)memalign(16, texp->dataLength);
        uint8_t *d = (uint8_t *)memalign(16, texp->dataLength);
        memset(texp->data, 0x0, texp->dataLength);
		memset(d, 0x0, texp->dataLength);
		rowPointers = (png_bytep *)malloc(sizeof(png_bytep) * texp->imageHeight);
		
		for(i = 0; i < texp->imageHeight; i++)
			rowPointers[i] = (png_bytep)(d + i * texp->rowBytes);
			
		png_read_image(png_ptr, rowPointers);

        png_read_end(png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
		swizzle_fast(texp->data, d, texp->textureWidth * 4, texp->textureHeight);
		free(d);
        sceKernelDcacheWritebackRange(texp->data, texp->dataLength);
        free(rowPointers);
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

        sceGuTexMode (GU_PSM_8888, 0,0, 1);
		sceGuEnable(GU_TEXTURE_2D);
		sceGumPushMatrix();
		sceGumLoadIdentity();
		
		if(texp->hasAlpha)
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
		else
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);

        sceGuTexImage(0, texp->textureWidth, texp->textureHeight, texp->textureWidth, texp->data);

        ya2d_FloatTextureVertex *vertices = (ya2d_FloatTextureVertex *)sceGuGetMemory(2 * sizeof(ya2d_FloatTextureVertex));

        vertices[0] = (ya2d_FloatTextureVertex){0.0f, 0.0f, x, y, 0.0f};
        vertices[1] = (ya2d_FloatTextureVertex){1.0f, 1.0f, x+texp->textureWidth, y+texp->textureHeight, 0.0f};

        sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 2, 0, vertices);

        sceKernelDcacheWritebackRange(vertices, 2 * sizeof(ya2d_FloatTextureVertex));
        sceGumPopMatrix();
    }

void ya2d_drawRotateTexture(ya2d_Texture *texp, int x, int y, float angle)
{
sceGuDisable(GU_TEXTURE_2D);
		sceGumPushMatrix();
		sceGumLoadIdentity();
		ScePspFVector3 p = {(float)x, (float)y, 0.0f};
		
		ya2d_FloatVertex __attribute__((aligned(16))) vert[3] =
		{
			   {GU_RGB(255,0,0), 0.0f, -50.0f, 0.0f}, // Top, red
			   {GU_RGB(0,255,0), 50.0f, 50.0f, 0.0f}, // Right, green
			   {GU_RGB(0,0,255), -50.0f, 50.0f, 0.0f}, // Left, blue
		};
		sceGumTranslate(&p);
        sceGumRotateZ(angle);
        
        sceGumDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,vert);


		
		sceGumLoadIdentity();
		p.x += 51.0f;
		sceGumTranslate(&p);
        sceGumRotateZ(angle);
        
        sceGumDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,vert);

		sceGumPopMatrix();		

}
