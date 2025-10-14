#ifndef _DATA_LINK_H_
#define _DATA_LINK_H_

typedef struct {
    char serialPort[50]; // Device /dev/ttySx {x = {0,1}}
    LinkLayerRole role; //Transmitter or receiver
    int baudrate;  // Speed of the transmission
    int timeout; // Retransmission timeout value in seconds 
    int nRetransmissions; //Number of retries in case of failure 
} LinkLayer;

typedef enum{
LlTx, // Act as transmitter
LlRx, // Act as receiver
} LinkLayerRole;

int llopen(LinkLayer connectionParameters);

int llwrite(const unsigned char *buf, int bufSize); 

int llread(unsigned char *packet); 

int llclose(); 
#endif _DATA_LINK_H_