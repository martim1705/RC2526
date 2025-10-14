#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include "../macros/const.h"

int alarmEnabled = FALSE;
int alarmCount = 0; 



void alarmHandler(int signal);

void configAlarm();

void disableAlarm();

void enableAlarm(int time);