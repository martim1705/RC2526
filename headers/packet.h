#ifndef _PACKET_H_
#define _PACKET_H_

long int openFile(const char *filename); // returns file size
int buildControlPacket(); // builds the START packet 
int sendControlPacket(); // sends the control packets

#endif