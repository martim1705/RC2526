/*#include "../headers/stateMachine.h"
#include "../macros/const.h"
#include "../headers/serial.h"



int main() {
    
    unsigned char byte; 
    int current_state = ST_START;
    
     
    while (current_state != ST_STOP)
    {
        if (readByteSerialPort(&byte) < 0) {
            printf("Byte coul not be read");
            exit(0);
        }
        switch(current_state) {
            case ST_START:
                if (byte == FLAG) {
                    current_state = ST_FLAG_RCV;
                } 
                break;
            case ST_FLAG_RCV:
                if (byte == A_RCV) {
                    current_state = ST_A_RCV; 
                } else if (byte == FLAG) {
                    current_state = ST_FLAG_RCV; 
                } else {
                    current_state = ST_START; 
                }
                break; 
            case ST_A_RCV:
                if (byte == C_RCV) {
                    current_state = ST_C_RCV;
                } else if (byte == FLAG) {
                    current_state = ST_FLAG_RCV; 
                } else {current_state = ST_START;}
                    break; 
            case ST_C_RCV:
                if (byte == BCC_RCV) {
                    current_state = ST_BCC_OK; 
                } else if (byte == FLAG) {
                    current_state = ST_FLAG_RCV; 
                } else {
                    current_state = ST_START; 
                }
                break; 
            case ST_BCC_OK:
                if (byte == FLAG) {
                        current_state = ST_STOP;
                } else {
                    current_state = ST_START;
                }
                break;
            default:
                printf("Impossible state reached.");
                break; 
        }
    }

    return 0; 
} */