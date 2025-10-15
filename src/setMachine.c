#include <stdio.h>
#include <stdlib.h>
#include "../headers/stateMachine.h"
#include "../macros/const.h"




 

int change_state(unsigned char byte, int *current_state) {
    
    int nBytesBuf = 0;



    switch(*current_state) {
        case ST_START:
            if (byte == FLAG) {
                *current_state = ST_FLAG_RCV;
            } 
            break;
        case ST_FLAG_RCV:
        
            if (byte == A_SND) {
                *current_state = ST_A_RCV; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG_RCV; 
            } else {
                *current_state = ST_START; 
            }
            break; 
        case ST_A_RCV:
            if (byte == C_SND) {
                *current_state = ST_C_RCV;
            } else if (byte == FLAG) {
                *current_state = ST_FLAG_RCV; 
            } else {*current_state = ST_START;}
                break; 
        case ST_C_RCV:
            if (byte == BCC_SND) {
                *current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG_RCV; 
            } else {
                *current_state = ST_START; 
            }
                break; 
        case ST_BCC_OK:
            if (byte == FLAG) {
                *current_state = ST_STOP;
            } else {
                *current_state = ST_START;
            }
            break;
        case ST_STOP:
            break; 
        default:
            printf("Impossible state reached.");
            break; 
    }
    nBytesBuf += 1; 
    if (*current_state != 5)
    readByteSerialPort(&byte);

    printf("Total bytes received: %d\n", nBytesBuf);  
}