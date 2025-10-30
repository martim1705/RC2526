
#include "../headers/applicationLayer.h"

#include <string.h>

#include <stdlib.h>
#include <stdio.h>


#define NTRANSMISSIONS 3
#define TIMEOUT 3 


int main(int argc, char *argv[]) {
    //./project 
    // utilizador escolhe opção (receiver(1)/sender(0))
    if (argc < 5) {
        printf("Usage: %s <serial port> <baudrate> <LlTx|LlRx> filename\n", argv[0]);
        return 1;
    }

    
    
    const char *serialPort = argv[1];
    const int baudrate = atoi(argv[2]);
    const char *role = argv[3];   
    const char *filename = argv[4]; 

// check role values
    if (strcmp(role, "LlRx") != 0 && strcmp(role, "LlTx") != 0) {
        printf("error in role. Must be \"LlTx\" or \"LlRx\".\n");
        exit(1); 
    } 
    
    // check baudrate values
    switch (baudrate)
    {
        case 1200: case 1800: case 2400: case 4800:
        case 9600: case 19200: case 38400: case 57600:
        case 115200:
            break; 
        
    default:
        fprintf(stderr, "Unsupported baud rate (must be one of 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200)\n");
        exit(1);
    }

    printf("Starting opening application-layer connection.\n"
           "Serial port: %s\n"
           "Role: %s\n"
           "Baudrate: %d\n"
           "Timeout: %d\n"
           "Retransmissions: %d\n"
           "Filename: %s\n", serialPort, role, 
           baudrate, TIMEOUT, NTRANSMISSIONS,filename);

    appConfig(serialPort, role, baudrate, TIMEOUT, NTRANSMISSIONS,filename);
    

    return 0; 
}
