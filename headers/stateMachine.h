#include "stdlib.h"
#include "stdio.h"


// Machine state states 
enum State {
    ST_START,
    ST_FLAG_RCV,
    ST_A_RCV,
    ST_C_RCV,
    ST_BCC_OK,
    ST_STOP
}; 