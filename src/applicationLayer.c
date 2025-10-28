#include "../headers/applicationLayer.h"
#include "../headers/data_link.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void appConfig(const char *serialPort, const char* role, int baudrate, int timeout, int nretransmissions, const char* filename) {
    
    if (filename == NULL | serialPort == NULL || role == NULL) {
        printf("filename, serialPort, or role parameters were passed as NULL.\n");
        return; 
    }

    LinkLayer parameters; 
    strcpy(parameters.serialPort, serialPort);
    parameters.baudrate = baudrate; 
    parameters.nRetransmissions = nretransmissions; 
    parameters.timeout = timeout; 
    if (!strcmp(role, "LlTx")) {
        parameters.role = LlTx; 
    } else {
        parameters.role = LlRx; 
    }

    

    if (llopen(parameters) < 0) {
        printf("Error calling llopen().\n"); 
        return; 
    }
}


