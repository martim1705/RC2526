#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include "../macros/const.h"
 
typedef struct {
    int alarmEnabled; 
    int alarmCount; 
} alarmStates; 


void alarmHandler(int signal);

void configAlarm();

void disableAlarm();

void enableAlarm(int timeout);