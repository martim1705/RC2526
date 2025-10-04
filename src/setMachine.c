#include "stateMachine.h"
#include "../macros/const.h"

/*
 as receiver 
#define FLAG 0X7E
#define A 0x01
#define C 0x07
const unsigned char BCC = A ^ C;
BCC_SND

*/


int main() {
    // nxt_state é byte - CORRIGIR ISSO 
    int current_state = START; 
    // int nxt_state = 0; 
    switch (current_state)
    {
    case START:
        if (nxt_state == FLAG_RCV) current_state = FLAG_RCV; 
        
        break; 
        
    
    case FLAG_RCV:
        if (nxt_state == A_RCV) current_state = A_RCV;
        else {
            current_state = START; 
        } 
        break; 

    case A_RCV:
        if (nxt_state == C_RCV) current_state = C_RCV;
        else if (nxt_state == FLAG_RCV) current_state = FLAG_RCV; 
        else {current_state = START;} 
        break;

    case C_RCV:
        if (nxt_state == BCC) current_state = BCC_OK; 
        else if (nxt_state == FLAG_RCV) current_state = FLAG_RCV; 
        else {current_state = START;}
        break;
    case BCC_OK:
        if (nxt_state == FLAG_RCV) current_state = STOP; 
        else {current_state = START;}
        break;
    case STOP:
        printf("Message read successfully.\n");
        break; 
    default:
        printf("Message was not read.\n");
        break; 
    }
    return 0; 
}