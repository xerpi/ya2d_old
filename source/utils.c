#include "utils.h"

u32 frames;
u64 current_millis, last_millis, delta_millis;


void initFPS()
{
	current_millis = getMilliSeconds();
	last_millis = current_millis;
	frames = 0;
}

void FPS(float *fps_p)
{
	frames++;
	current_millis = getMilliSeconds();
	delta_millis = current_millis - last_millis;
	if(delta_millis >= 1000)
	{
		*fps_p = (float)frames/((float)delta_millis/1000.0f);
		frames = 0;
		last_millis = getMilliSeconds();
	}
}


/*callbacks*/

int exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

int CallbackThread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("exit_callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("callback_update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}
