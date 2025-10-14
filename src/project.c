#include "../macros/const.h"
//#include "../headers/alarm_sigaction.h"
#include "../headers/write_noncanonical.h"
#include "../headers/read_noncanonical.h"
#include "../headers/applicationLayer.h"
#include "../headers/data_link.h" 
#include <string.h>

#include <stdlib.h>
#include <stdio.h>


#define NTRANSMISSIONS 3
#define TIMEOUT 3 


int main(int argc, char *argv[]) {
    //./project 
    // utilizador escolhe opção (receiver(1)/sender(2))
    if (argc < 4) {
        printf("Usage: %s <serial port> <baudrate> <LlTx|LlRx>\n", argv[0]);
        return 1;
    }

    LinkLayer parameters;
    
    
    const char* serialPort[50] = argv[1];
    const int baudrate = toi(argv[2]);
    const char *role = argv[3];     
// check role values
    if (strcmp(role, "LlRx") || strcmp(role, "LlTx")) {
        printf("error in role. Must be \"LlTx\" or \"LlRx\".\n");
        exit(1); 
    } 
    
    // check baudrate values
    switch (baudrate)
    {
        CASE_BAUDRATE(1200);
        CASE_BAUDRATE(1800);
        CASE_BAUDRATE(2400);
        CASE_BAUDRATE(4800);
        CASE_BAUDRATE(9600);
        CASE_BAUDRATE(19200);
        CASE_BAUDRATE(38400);
        CASE_BAUDRATE(57600);
        CASE_BAUDRATE(115200);
    default:
        fprintf(stderr, "Unsupported baud rate (must be one of 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200)\n");
        exit(1);
    }

    printf("Starting opening application-layer connection.\n"
           "Serial port: %s\n"
           "Role: %s\n"
           "Baudrate: %d\n"
           "Timeout: %d\n"
           "Retransmissions: %d\n", serialPort, role, 
           baudrate, TIMEOUT, NTRANSMISSIONS);

    appConfig(serialPort, role, baudrate, TIMEOUT, NTRANSMISSIONS);
    // if (!strcmp(serialPort, "/dev/pts/3")) run_receiver(argc, argv); // llread()
    // else if (!strcmp(serialPort, "/dev/pts/2")) run_sender(argc, argv); // llwrite()

    return 0; 
}
