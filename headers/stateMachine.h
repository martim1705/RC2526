#include "stdlib.h"
#include "stdio.h"


// Machine state states 
enum State {
    START,
    FLAG_RCV,
    A_RCV,
    C_RCV,
    BCC_OK,
    STOP
}; 