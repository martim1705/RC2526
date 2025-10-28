#ifndef _FRAMES_H_
#define _FRAMES_H_

#include "../macros/const.h"
#include "stateMachine.h"
#include "serial.h"

int create_SET(unsigned char *frame); 
int create_UA_Tx(unsigned char *frame);
int create_UA_Rx(unsigned char *frame);
int create_DISC_Tx(unsigned char *frame);
int create_DISC_Rx(unsigned char *frame);
int checkIFrame(unsigned char expectedAddressField, unsigned char *frameNuber, unsigned char *packet);
int createIFrame(unsigned char *data, int bufSize);
int createRR(unsigned char *frame, unsigned char Ns); 
#endif