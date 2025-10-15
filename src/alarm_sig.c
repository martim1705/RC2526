#include "../headers/alarm_sigaction.h"
#include <string.h>
#include <unistd.h>   // alarm()
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t alarmEnabled = 0;  
volatile sig_atomic_t alarmCount   = 0;

 

// define alarmHandler 
void alarmHandler(int signal) {
    alarmEnabled = 0;
    alarmCount++;
}

    // Set alarm function handler.
    // Install the function signal to be automatically invoked when the timer expires,
    // invoking in its turn the user function alarmHandler

void configAlarm() {
 
    struct sigaction act = {0};
    act.sa_handler = &alarmHandler;
    if (sigaction(SIGALRM, &act, NULL) == -1) // configurar o alarme
    {
        perror("sigaction");
        exit(1);
    }
}

void disableAlarm() {
    alarmEnabled = 0; 
    alarm(0);
}

void enableAlarm(int timeout) {
    alarmEnabled = 1; 
    alarm(timeout); 
}