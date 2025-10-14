#include "../headers/data_link.h"
#include "../headers/alarm_sigaction.h"
#include "../headers/read_noncanonical.h"
#include "../headers/write_noncanonical.h"
#include "../headers/serial.h"
#include <stdio.h>

 
int llopen(LinkLayer parameters) {
    if (openSerialPort(parameters.serialPort, parameters.baudrate) < 0) {
            printf("Serial port opening error.\n");
            return -1; 
        };  
    printf("Serial port %s opened\n", parameters.serialPort);
    if (!strcmp(parameters.role, "LlTx")) {
        
    } else if (!strcmp(parameters.role,"LlRx")) {
        
    } else return -1; 

    return 0;
}
     
int llclose();










































// alarm MACROS 
    int alarmEnabled = FALSE;
    int alarmCount = 0; 


    // alarm handler code 
    void alarmHandler(int signal)
    {
        alarmEnabled = FALSE;
        alarmCount++;

        printf("Alarm #%d received\n", alarmCount);
    }


int llwrite(const unsigned char *buf, int bufSize) {
    if (bufSize < 0) {
        printf("bufSize must be equal to buffer size. Currently bufSize is equal to %d\n",bufSize);
        return -1; 
    }

    if (buf == NULL) {
        printf("buf was passed as a null pointer.\n");
        return -1; 
    }

    // se os argumento estiverem todos corretos entao 
    // configurar o alarme 
    // começar o alarme

    while (alarmCount < 4 && )
    

    // Set alarm function handler.
    // Install the function signal to be automatically invoked when the timer expires,
    // invoking in its turn the user function alarmHandler
    struct sigaction act = {0};
    act.sa_handler = &alarmHandler;
    if (sigaction(SIGALRM, &act, NULL) == -1) // configurar o alarme
    {
        perror("sigaction");
        exit(1);
    }

    printf("Alarm configured\n");

    int t = 3; 
    alarm(t); 



}