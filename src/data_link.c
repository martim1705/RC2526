#include "../headers/data_link.h"
#include "../headers/alarm_sigaction.h"
#include <stdio.h>

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