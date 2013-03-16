#ifndef _YA2D_GLOBALS_H_
#define _YA2D_GLOBALS_H_

#ifndef GU_RGB
	#define GU_RGB(r,g,b) GU_RGBA(r,g,b,255)
#endif

#ifndef printf
	#define printf pspDebugScreenPrintf
#endif

#define YA2D_BUF_WIDTH  512
#define YA2D_SCR_WIDTH  480
#define YA2D_SCR_HEIGHT 272

#define YA2D_PNG_SIG_LEN 8


#endif
