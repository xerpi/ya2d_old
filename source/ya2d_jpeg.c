#include "ya2d_jpeg.h"


ya2d_Texture* ya2d_loadJPEGfromFile(char *filename, ya2d_Place place)
{
	FILE       *fp;
	JSAMPARRAY jpegLine;
	uint8_t    *jpegLineP;
	int        jpegRowBytes, y, x;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	uint32_t color;
	ya2d_Texture *texp;

	if(!(fp = fopen(filename, "rb")))
		goto exit_error;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fp);
	jpeg_read_header(&cinfo, 1);

	jpegRowBytes = cinfo.image_width * 3;
	
	//Create texture
		texp = ya2d_createTexture(cinfo.image_width, cinfo.image_height, GU_PSM_8888, place);
	texp->hasAlpha   = 1;
	
	jpegLine = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * cinfo.image_height);
	jpegLine[0] = (JSAMPROW)malloc(jpegRowBytes);
	
	jpeg_start_decompress(&cinfo);
	y = 0;
	while(cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)jpegLine, 1);
		jpegLineP = (uint8_t *)(jpegLine[0]);
		for(x = 0; x < cinfo.image_width; x++, color = 0)
		{
			color |=  *(jpegLineP++);
			color |=  *(jpegLineP++)<<8;
			color |=  *(jpegLineP++)<<16;
			*(uint32_t *)(texp->data + (x*4) + texp->rowBytes * y) = color | 0xFF000000;
		}
		y++;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(jpegLine[0]); free(jpegLine);
	fclose(fp);
	return texp;
exit_error:
	return NULL;
}
