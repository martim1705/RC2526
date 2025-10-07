#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include "../macros/const.h"
// alarm 
#include <signal.h>

// void alarmHandler(int signal);
void run_sender(int argc, char *argv[]); 