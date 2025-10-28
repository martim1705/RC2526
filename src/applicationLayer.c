#include "../headers/applicationLayer.h"


unsigned char packet[264];

void appConfig(const char *serialPort, const char* role, int baudrate, int timeout, int nretransmissions, const char* filename) {
    
    if (filename == NULL || serialPort == NULL || role == NULL) {
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

    if (parameters.role == "LlTx") {

        long int fileSize = openFile(filename);
        
        if (fileSize < 0) {
            return; 
        }    
        
        int packetSize = buildControlPacketunsigned(packet, filename, fileSize, 1);
        
        if ( packetSize < 0) {
            printf("Could not contruct Control packet.\n"); 
            return; 
        }

        llwrite(packet, packetSize);  
    } else {
        
    }
    

}


