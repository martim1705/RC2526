#include <stdio.h>
#include <stdlib.h>
#include "../headers/stateMachine.h"
#include "../macros/const.h"
#include "../headers/serial.h"





 

int change_state(unsigned char byte, int *current_state) {

    switch(*current_state) {
        case ST_START:
            if (byte == FLAG) {
                *current_state = ST_FLAG;
            }
            break;
        case ST_FLAG:
        
            if (byte == A_Tx) {
                *current_state = ST_A_Tx; 
            } else if (byte == A_Rx){
                *current_state = ST_A_Rx;
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
            } else {
                *current_state = ST_START; 
            }
            break; 
        case ST_A_Tx:
            if (byte == C_SET) {
                *current_state = ST_C_SET_Tx;
            } else if (byte == C_UA) {
                *current_state = ST_C_UA_Tx;
            } else if (byte == C_DISC) {
                *current_state = ST_C_DISC_Tx;
            } else if (byte == FLAG){
                *current_state = ST_FLAG;
            } else {*current_state = ST_START;}
                break;
        case ST_A_Rx:
            if (byte == C_UA) {
                *current_state = ST_C_UA_Rx;
            } else if (byte == C_DISC) {
                *current_state = ST_C_DISC_Rx;
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
            } else {*current_state = ST_START;}
                break;
        case ST_C_SET_Tx:
            if (byte == BCC_SET_Tx) {
                *current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
            } else {
                *current_state = ST_START; 
            }
                break;
        case ST_C_UA_Tx:
            if (byte == BCC_UA_Tx) {
                *current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
            } else {
                *current_state = ST_START; 
            }
                break;
        case ST_C_UA_Rx:
            if (byte == BCC_UA_Rx) {
                *current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
            } else {
                *current_state = ST_START; 
            }
                break;
        case ST_C_DISC_Rx:
            if (byte == BCC_DISC_Rx) {
                *current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
            } else {
                *current_state = ST_START; 
            }
                break; 
        case ST_C_DISC_Tx:
            if (byte == BCC_DISC_Tx) {
                *current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                *current_state = ST_FLAG; 
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
    return 0;
}


IFrameState updateIFrameState(unsigned byte,enum State current_state, unsigned char expectedAddress, unsigned char frameNumber) {
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
            unsigned char bcc1 = expectedAddress ^ frameNumber;
            if (bcc1 == byte)
                return IF_BCC1_OK;
            else
                return IF_START;
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