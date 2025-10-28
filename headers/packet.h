#ifndef _PACKET_H_
#define _PACKET_H_

#include "../macros/const.h"

#include <stdio.h>
#include <stdlib.h>

long int getFileSize(FILE* filename); // returns file size
int buildControlPacket(unsigned char *packet, const char *filename, long int fileSize, int type); // builds the START packet 
int sendControlPacket(unsigned char *packet, unsigned char *data, int dataSize); // sends the control packets
int buildDataPacket(unsigned char *packet, unsigned char *data, int dataSize, unsigned char seqNum); // builds the data packet 
int readControlPacket(unsigned char *packet, long int *fileSize, int packetSize, char *filename); 
#endif