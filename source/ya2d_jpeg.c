#include "ya2d_jpeg.h"


ya2d_Texture* ya2d_loadJPEGfromFile(char *filename, ya2d_Place place)
{
	FILE       *fp;
	JSAMPARRAY jpegLine;
	uint8_t    *jpegLineP;
	int        jpegRowBytes, y, x;
	struct jpeg_decompress_struct jinfo;
	struct jpeg_error_mgr jerr;
	uint32_t color;
	ya2d_Texture *texp;

	if(!(fp = fopen(filename, "rb")))
		goto exit_error;

	jinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&jinfo);
	jpeg_stdio_src(&jinfo, fp);
	jpeg_read_header(&jinfo, 1);

	jpegRowBytes = jinfo.image_width * 3;
	
	//Create texture
		texp = ya2d_createTexture(jinfo.image_width, jinfo.image_height, GU_PSM_8888, place);
	texp->hasAlpha   = 1;
	
	jpegLine = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * jinfo.image_height);
	jpegLine[0] = (JSAMPROW)malloc(jpegRowBytes);
	
	jpeg_start_decompress(&jinfo);
	y = 0;
	while(jinfo.output_scanline < jinfo.output_height)
	{
		jpeg_read_scanlines(&jinfo, (JSAMPARRAY)jpegLine, 1);
		jpegLineP = (uint8_t *)(jpegLine[0]);
		for(x = 0; x < jinfo.image_width; x++, color = 0)
		{
			color |=  *(jpegLineP++);
			color |=  *(jpegLineP++)<<8;
			color |=  *(jpegLineP++)<<16;
			*(uint32_t *)(texp->data + (x*4) + texp->rowBytes * y) = color | 0xFF000000;
		}
		y++;
	}
	
	jpeg_finish_decompress(&jinfo);
	jpeg_destroy_decompress(&jinfo);
	free(jpegLine[0]); free(jpegLine);
	fclose(fp);
	return texp;
exit_error:
	return NULL;
}


ya2d_Texture* ya2d_loadJPEGfromBuffer(void* buffer, u32 buffer_size, ya2d_Place place)
{
	FILE       *fp;
	JSAMPARRAY jpegLine;
	uint8_t    *jpegLineP;
	int        jpegRowBytes, y, x;
	struct jpeg_decompress_struct jinfo;
	struct jpeg_error_mgr jerr;
	uint32_t color;
	ya2d_Texture *texp;

	if(buffer == NULL)
		goto exit_error;

	jinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&jinfo);
	
	
	jpeg_mem_src(&jinfo, buffer, buffer_size);
	jpeg_read_header(&jinfo, 1);

	jpegRowBytes = jinfo.image_width * 3;
	
	//Create texture
		texp = ya2d_createTexture(jinfo.image_width, jinfo.image_height, GU_PSM_8888, place);
	texp->hasAlpha   = 1;
	
	jpegLine = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * jinfo.image_height);
	jpegLine[0] = (JSAMPROW)malloc(jpegRowBytes);
	
	jpeg_start_decompress(&jinfo);
	y = 0;
	while(jinfo.output_scanline < jinfo.output_height)
	{
		jpeg_read_scanlines(&jinfo, (JSAMPARRAY)jpegLine, 1);
		jpegLineP = (uint8_t *)(jpegLine[0]);
		for(x = 0; x < jinfo.image_width; x++, color = 0)
		{
			color |=  *(jpegLineP++);
			color |=  *(jpegLineP++)<<8;
			color |=  *(jpegLineP++)<<16;
			*(uint32_t *)(texp->data + (x*4) + texp->rowBytes * y) = color | 0xFF000000;
		}
		y++;
	}
	
	jpeg_finish_decompress(&jinfo);
	jpeg_destroy_decompress(&jinfo);
	free(jpegLine[0]); free(jpegLine);
	return texp;
exit_error:
	return NULL;
}
