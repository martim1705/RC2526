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


IFrameState updateIFrameState(unsigned byte,enum State current_state, unsigned char expectedAddress, unsigned char expectedframeNumber) {
    switch(current_state) {
        case IF_START:
            if (byte == FLAG) {
                return IF_FLAG_RCV;
            } else {
                return IF_START;
            }
        
        case IF_FLAG_RCV:
            if (byte == FLAG) {
                return IF_FLAG_RCV; 
            }
            else return IF_A_RCV;
            
        case IF_A_RCV: 
                return IF_C_RCV;

        
        case IF_C_RCV: {
            unsigned char bcc1 = expectedAddress ^ expectedframeNumber;
            if (bcc1 == byte)
                return IF_BCC1_OK;
            else
                return IF_BCC1_BAD;
        } 

        case IF_BCC1_OK:
            if (byte == FLAG) {
                return IF_STOP; 
            } else if (byte == ESC) {
                return IF_DATA_ESC; 
            } else {
                return IF_DATA;
            }
        
        case IF_DATA: 
            
            if (byte == FLAG) {
                return IF_STOP; 
            } else if (byte == ESC) {
                return IF_DATA_ESC; 
            } else {
                return IF_DATA;
            }

        case IF_DATA_ESC:
            return IF_DATA; 

        default:
            return IF_START; 
    }

}