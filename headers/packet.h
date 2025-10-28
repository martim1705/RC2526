#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>
#include <stdlib.h>

long int openFile(const char *filename); // returns file size
int buildControlPacketunsigned(char *packet, const char *filename, long int fileSize, int type); // builds the START packet 
int sendControlPacket(unsigned char *packet, unsigned char *data, int dataSize); // sends the control packets

#endif