#ifndef _UTILS_H_
#define _UTILS_H_

#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdio.h>
#include <stdlib.h>
#include <psprtc.h>


int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);



#endif
