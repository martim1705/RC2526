#ifndef _APPLICATIONLAYER_H_
#define _APPLICATIONLAYER_H_

#include "../headers/applicationLayer.h"
#include "../headers/data_link.h"
#include "../headers/packet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//uses the data to create the LinkLayer struct 
void appConfig(const char *serialPort, const char *role, int baudrate, int timeout, int nretransmissions, const char* filename);
int sendFile(FILE* file); 
int readFile(FILE* file, long int fileSize);

#endif