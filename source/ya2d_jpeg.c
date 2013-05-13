#include "ya2d_jpeg.h"


int ya2d_loadJPEGfromFile(char *filename, ya2d_Texture *texp)
{
	FILE       *fp;
	JSAMPARRAY jpegLine;
	uint8_t    *jpegLineP;
	int        jpegRowBytes, y, x;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	uint32_t color;

	if(!(fp = fopen(filename, "rb")))
		goto exit_error;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fp);
	jpeg_read_header(&cinfo, 1);

	jpegRowBytes = cinfo.image_width * 3;

	texp->imageWidth = cinfo.image_width;
	texp->imageHeight = cinfo.image_height;
	texp->textureWidth  = next_pow2(texp->imageWidth);
	texp->textureHeight = next_pow2(texp->imageHeight);
	texp->centerX = (int)(texp->imageWidth/2);
	texp->centerY = (int)(texp->imageHeight/2);

	texp->texPSM = GU_PSM_8888;
	texp->isSwizzled = 0;
	texp->hasAlpha   = 1;
	
	texp->rowBytes   = texp->textureWidth * 4;
	texp->dataLength = texp->rowBytes * texp->textureHeight;
	texp->data = (uint8_t *)memalign(16, texp->dataLength);
	memset(texp->data, 0x0, texp->dataLength);

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
	return 1;
exit_error:
	return 0;
}
