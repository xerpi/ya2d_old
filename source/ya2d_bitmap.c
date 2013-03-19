#include "ya2d_bitmap.h"


int ya2d_isBMP(ya2d_BMPheader *BMPheader)
{
	if(BMPheader->fileHeader.signature[0] == 'B' &&
	   BMPheader->fileHeader.signature[1] == 'M')
	{
		return 1;
	}
	return 0;
}

int ya2d_loadBMPfromFile(char *filename, ya2d_Texture *texp)
{
	SceUID       fd;
	ya2d_BMPheader BMPheader;
	uint8_t *imageBuffer;
	int x, y;
	int imgRowBytes, imgSize;

	if(!(fd = sceIoOpen(filename, PSP_O_RDONLY, 0777)))
		goto exit_error;

	sceIoLseek(fd, 0x0, SEEK_SET);
	sceIoRead(fd, (void *)&BMPheader, sizeof(BMPheader));

	if(!(ya2d_isBMP(&BMPheader)))
		goto exit_close;

	//ya2d_error("bitsPixel: %i  w: %i  h: %i",BMPheader.infoHeader.bitsPerPixel,BMPheader.infoHeader.bitmapWidth,BMPheader.infoHeader.bitmapHeight);

	texp->bitDepth = BMPheader.infoHeader.bitsPerPixel;

	texp->imageWidth  = BMPheader.infoHeader.bitmapWidth;
	texp->imageHeight = BMPheader.infoHeader.bitmapHeight;

	texp->textureWidth  = next_pow2(BMPheader.infoHeader.bitmapWidth);
	texp->textureHeight = next_pow2(BMPheader.infoHeader.bitmapHeight);

	texp->centerX = texp->imageWidth/2;
	texp->centerY = texp->imageHeight/2;
	
	texp->hasAlpha   = 1;
	texp->isSwizzled = 0;

	switch(texp->bitDepth)
	{
	case 4: //4 or 32?
		texp->texPSM = GU_PSM_8888;
		texp->rowBytes = texp->textureWidth * 4;
		imgRowBytes = texp->imageWidth * 4;
		break;
	case 2: //2 or 16?
		texp->texPSM = GU_PSM_4444;
		texp->rowBytes = texp->textureWidth * 2;
		imgRowBytes = texp->imageWidth * 2;
		ya2d_error("16 bit BMP");
		break;
	default:
		ya2d_error("BMP bitDepth: %i", texp->bitDepth);
		imgRowBytes = 0;
		break;
	}

	texp->dataLength = texp->rowBytes * texp->textureHeight;
	imgSize = imgRowBytes * BMPheader.infoHeader.bitmapHeight;
	
	texp->data  = (uint8_t *)memalign(16, texp->dataLength);
	imageBuffer = (uint8_t *)memalign(16, imgSize);
	memset(texp->data, 0xff, texp->dataLength);
	
	sceIoLseek(fd, BMPheader.fileHeader.dataOffset, SEEK_SET);
	sceIoRead(fd, imageBuffer, imgSize);

	for(y = 0; y < BMPheader.infoHeader.bitmapHeight; y++)
	{
		for(x = 0; x < imgRowBytes; x++)
		{
			if(texp->bitDepth == 4)
			{
				*(uint8_t *)(texp->data + x + texp->rowBytes * y) = *(uint8_t *)(imageBuffer + x + imgRowBytes * y);
			}
			else
			{
				ya2d_error("bmp lawlz");
			}
		}
	}

	free(imageBuffer);
	sceIoClose(fd);
	return 1;
	
exit_close:
	sceIoClose(fd);
exit_error:
	return 0;
}
