#ifndef _PA_video
#define _PA_video
#include "PA9.h"
extern void PA_on_irqvideo();
extern void PA_InitInterruptHandlervideo();
extern void PA_Initvideo();
extern void PA_video(int numimages, char *nomdufichier);
extern int PA_maxfilevideo(char *nomdufichier);
#endif
