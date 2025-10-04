#include "../headers/stateMachine.h"
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
    int current_state = ST_START; 
    int nxt_state = 0; 
    switch (current_state)
    {
    case ST_START:
        if (nxt_state == ST_FLAG_RCV) current_state = ST_FLAG_RCV; 
        
        break; 
        
    
    case ST_FLAG_RCV:
        if (nxt_state == A_RCV) current_state = A_RCV;
        else {
            current_state = ST_START; 
        } 
        break; 

    case ST_A_RCV:
        if (nxt_state == C_RCV) current_state = C_RCV;
        else if (nxt_state == ST_FLAG_RCV) current_state = ST_FLAG_RCV; 
        else {current_state = ST_START;} 
        break;

    case ST_C_RCV:
        if (nxt_state == ST_BCC_OK) current_state = ST_BCC_OK; 
        else if (nxt_state == ST_FLAG_RCV) current_state = ST_FLAG_RCV; 
        else {current_state = ST_START;}
        break;
    case ST_BCC_OK:
        if (nxt_state == ST_FLAG_RCV) current_state = ST_STOP; 
        else {current_state = ST_START;}
        break;
    case ST_STOP:
        printf("Message read successfully.\n");
        break; 
    default:
        printf("Message was not read.\n");
        break; 
    }
    return 0; 
}