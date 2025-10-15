#ifndef ALARM_SIGACTION_H
#define ALARM_SIGACTION_H

#include <signal.h>


extern volatile sig_atomic_t alarmEnabled;
extern volatile sig_atomic_t alarmCount;

void configAlarm();
void enableAlarm(int seconds);
void disableAlarm();

#endif 
