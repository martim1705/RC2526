#include "../headers/alarm_sigaction.h"

alarmStates alarmState; 

// define alarmHandler 
void alarmHandler(int signal) {
    alarmState.alarmEnabled = FALSE;
    alarmState.alarmCount++;
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
    alarmState.alarmEnabled = FALSE; 
    alarm(0);
}

void enableAlarm(int timeout) {
    alarmState.alarmEnabled = TRUE; 
    alarm(timeout); 
}