#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include "../macros/const.h"
#include "data_link.h"
 
typedef struct {
    int alarmEnabled; 
    int alarmCount; 
} alarmStates; 


void alarmHandler(int signal, alarmStates alarmState);

void configAlarm(alarmStates alarmState);

void disableAlarm(alarmStates alarmState);

void enableAlarm(LinkLayer layer);