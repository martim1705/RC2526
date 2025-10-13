#include "../macros/const.h"
//#include "../headers/alarm_sigaction.h"
#include "../headers/write_noncanonical.h"
#include "../headers/read_noncanonical.h" 
#include <string.h>

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    //./project 
    // utilizador escolhe opção (receiver(1)/sender(2))
    if (argc < 2) {
        printf("Incorrect program usage\n"
        "Usage: %s <SerialPort>\n"
        "Example: %s /dev/ttyS0\n",
        argv[0],
        argv[0]);
        exit(1);
    }

    if (!strcmp(argv[1], "/dev/pts/3")) run_receiver(argc, argv);
    else if (!strcmp(argv[1], "/dev/pts/2")) run_sender(argc, argv);

    return 0; 
}
