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
int createIFrame(const unsigned char *data, int bufSize, unsigned char *frame, unsigned char ns);
int createResponse(unsigned char *frame, unsigned char Ns, int code); 
int sendResponse(unsigned char *frame);
int sendIFrame(unsigned char *frame, int bufSize);
int readResponse(unsigned char *frame);
int checkFrame(unsigned char *frame);
#endif