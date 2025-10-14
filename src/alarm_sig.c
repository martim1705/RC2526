#include "../headers/alarm_sigaction.h"

// define alarmHandler 
void alarmHandler(int signal, alarmStates alarmState) {
    alarmState.alarmEnabled = FALSE;
    alarmState.alarmCount++;

    printf("Alarm #%d received\n", alarmState.alarmCount);
}

    // Set alarm function handler.
    // Install the function signal to be automatically invoked when the timer expires,
    // invoking in its turn the user function alarmHandler

void configAlarm(alarmStates alarmState) {

    alarmState.alarmCount = FALSE; 
    alarmState.alarmEnabled = 1; 

    struct sigaction act = {0};
    act.sa_handler = &alarmHandler;
    if (sigaction(SIGALRM, &act, NULL) == -1) // configurar o alarme
    {
        perror("sigaction");
        exit(1);
    }

    printf("Alarm configured\n");
}

void disableAlarm() {
    alarm(0);
}

enableAlarm(int time) {
    alarm(time); 
}