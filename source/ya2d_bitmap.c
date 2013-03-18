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
	int i;

	if(!(fd = sceIoOpen(filename, PSP_O_RDONLY, 0777)))
		goto exit_error;

	sceIoLseek(fd, 0x0, SEEK_SET);
	sceIoRead(fd, (void *)&BMPheader, sizeof(BMPheader));

	if(!(ya2d_isBMP(&BMPheader)))
		goto exit_close;

	sceIoClose(fd);
	return 1;
	
exit_close:
	sceIoClose(fd);
exit_error:
	return 0;
}
