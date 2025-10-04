/*#include "../macros/const.h"
#include "../headers/alarm_sigaction.h"
#include "../headers/write_noncanonical.h"
#include "../headers/read_noncanonical.h" */


#include <stdlib.h>
#include <stdio.h>

int main() {
    // utilizador escolhe opção (receiver(1)/sender(2))
    int userOption;

    printf("Choose to execute as:\n");
    printf("Receiver(1)\n");
    printf("Sender(2)\n");
    scanf("%d", &userOption);
    printf("Number is %d", userOption);
    return 0; 
}
