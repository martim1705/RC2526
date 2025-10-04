#include "stateMachine.h"


// as receiver 
#define FLAG 0X7E
#define A 0x01
#define C 0x07
const unsigned char BCC = A ^ C;


// Machine state states 
enum State {
    Start,
    FLAG_RCV,
    A_RCV,
    C_RCV,
    BCC_OK,
    STOP
}; 


int main() {
     
    return 0; 
}