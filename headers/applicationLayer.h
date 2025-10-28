#ifndef _APPLICATIONLAYER_H_
#define _APPLICATIONLAYER_H_


//uses the data to create the LinkLayer struct 
void appConfig(const char *serialPort, const char *role, int baudrate, int timeout, int nretransmissions);


#endif