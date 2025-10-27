#ifndef _FRAMES_H_
#define _FRAMES_H_

#include "../macros/const.h"
#include "stateMachine.h"
#include "serial.h"

int create_SET(unsigned char *frame); 
int create_UA(unsigned char *frame);  
int checkIFrame(unsigned char expectedAddressField, unsigned char *frameNuber, unsigned char *packet);
int createIFrame(unsigned char *data, int bufSize, unsigned char *frame);
int createResponse(unsigned char *frame, unsigned char Ns, int code); 
int sendResponse(unsigned char *frame);
#endif