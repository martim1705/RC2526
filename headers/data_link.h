#ifndef DATA_LINK_H_
#define DATA_LINK_H_

#include "../headers/alarm_sigaction.h"
#include "../headers/frames.h"
#include "../headers/serial.h"
#include "../headers/stateMachine.h"
#include "../macros/const.h"

#include <stdlib.h>
#include <stdio.h>

typedef enum{
LlTx, // Act as transmitter
LlRx, // Act as receiver
} LinkLayerRole;


typedef struct {
    char serialPort[50]; // Device /dev/ttySx {x = {0,1}}
    LinkLayerRole role; //Transmitter or receiver
    int baudrate;  // Speed of the transmission
    int timeout; // Retransmission timeout value in seconds 
    int nRetransmissions; //Number of retries in case of failure 
} LinkLayer;



int llopen(LinkLayer connectionParameters);

int llwrite(const unsigned char *buf, int bufSize); 

int llread(unsigned char *packet); 

int llclose(); 



#endif 