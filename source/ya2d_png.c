#include "ya2d_png.h"


	ya2d_Texture* ya2d_loadPNGfromFile(char *filename, ya2d_Place place)
	{
        uint8_t       header[YA2D_PNG_SIG_LEN];
        png_structp   png_ptr  = NULL;
        png_infop     info_ptr = NULL;
        FILE          *fp;
        png_bytep     *rowPointers;
		int           i;
		u32           width, height;
		int           bitDepth, colorType;
		ya2d_Texture* texp;

        if(!(fp = fopen(filename, "rb")))
            goto exit_error;

        fread(header, 1, YA2D_PNG_SIG_LEN, fp);
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

        //png_set_read_fn(png_ptr, (void *)&fd, (png_rw_ptr)_ya2d_png_read_fn);
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, YA2D_PNG_SIG_LEN);
        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);
		
        if(bitDepth == 16)
        {
            png_set_strip_16(png_ptr);
            bitDepth = 8;
        }
        else if(bitDepth < 8) //1 byte per pixel component
        {
            png_set_packing(png_ptr);
            bitDepth = 8;
        }
        
        //Create texture
			texp = ya2d_createTexture(width, height, GU_PSM_8888, place);

        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
        switch(colorType)
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
                if (bitDepth < 8)
                {
                    png_set_expand_gray_1_2_4_to_8(png_ptr);
                    texp->bitDepth = 8;
                }
                break;
            default:
                texp->hasAlpha = 0;
                break;
        }
		if(!texp->hasAlpha) //PSP only supports 16 or 32 bits pixels (not 0xRRGGBB)
		{
			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
			texp->hasAlpha = 1;
		}
        png_read_update_info (png_ptr, info_ptr);

        /* */
        
		rowPointers = (png_bytep *)malloc(sizeof(png_bytep) * texp->imageHeight);
		
		for(i = 0; i < texp->imageHeight; i++)
			rowPointers[i] = (png_bytep)(texp->data + i * texp->rowBytes);
			
		png_read_image(png_ptr, rowPointers);
        png_read_end(png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        free(rowPointers);
        fclose(fp);
        texp->isSwizzled = 0;
        return texp;

    exit_destroy:
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    exit_close:
        fclose(fp);
    exit_error:
        return NULL;
    }


	ya2d_Texture* ya2d_loadPNGfromBuffer(void *buffer, u32 buffer_size, ya2d_Place place)
	{
        uint8_t       header[YA2D_PNG_SIG_LEN];
        png_structp   png_ptr  = NULL;
        png_infop     info_ptr = NULL;
        png_bytep     *rowPointers;
		int           i;
		u32           read_address = (u32) buffer;
		u32           width, height;
		int           bitDepth, colorType;
		ya2d_Texture* texp;

        if(buffer == NULL || buffer_size == 0)
            goto exit_error;

        memcpy(header, buffer, YA2D_PNG_SIG_LEN);
        read_address += YA2D_PNG_SIG_LEN;
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

        png_set_read_fn(png_ptr, (void *)&read_address, (png_rw_ptr)_ya2d_png_read_buffer_fn);
        png_set_sig_bytes(png_ptr, YA2D_PNG_SIG_LEN);
        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);
		
        if(bitDepth == 16)
        {
            png_set_strip_16(png_ptr);
            bitDepth = 8;
        }
        else if(bitDepth < 8) //1 byte per pixel component
        {
            png_set_packing(png_ptr);
            bitDepth = 8;
        }
        
        //Create texture
			texp = ya2d_createTexture(width, height, GU_PSM_8888, place);

        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
        switch(colorType)
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
                if (bitDepth < 8)
                {
                    png_set_expand_gray_1_2_4_to_8(png_ptr);
                    texp->bitDepth = 8;
                }
                break;
            default:
                texp->hasAlpha = 0;
                break;
        }
		if(!texp->hasAlpha) //PSP only supports 16 or 32 bits pixels (not 0xRRGGBB)
		{
			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
			texp->hasAlpha = 1;
		}
        png_read_update_info (png_ptr, info_ptr);

        /* */
        
		rowPointers = (png_bytep *)malloc(sizeof(png_bytep) * texp->imageHeight);
		
		for(i = 0; i < texp->imageHeight; i++)
			rowPointers[i] = (png_bytep)(texp->data + i * texp->rowBytes);
			
		png_read_image(png_ptr, rowPointers);
        png_read_end(png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        free(rowPointers);
        texp->isSwizzled = 0;
        return texp;

    exit_destroy:
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    exit_close:
    exit_error:
        return NULL;
    }

	void _ya2d_png_read_file_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead)
    {
        SceUID *fd = (SceUID *)png_get_io_ptr(png_ptr);
		sceIoRead((SceUID)*fd, (void*)buffer, bytesToRead);
    }
    
	void _ya2d_png_read_buffer_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead)
    {
        u32 *addr = (u32 *)png_get_io_ptr(png_ptr);
		memcpy((void *)buffer, (void *)(*addr), bytesToRead);
		*addr += bytesToRead;
    }

