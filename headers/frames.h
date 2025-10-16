#ifndef _FRAMES_H_
#define _FRAMES_H_
#include "../macros/const.h"

int create_SET(unsigned char *frame); 
int create_UA(unsigned char *frame); 
int create_Iframe(unsigned char *frame); 
int checkIFrame(unsigned char *frame)

#endif