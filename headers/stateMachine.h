#ifndef STATEMACHINE_H
#define STATEMACHINE_H
// Machine state states 
enum State {
    ST_START,
    ST_FLAG_RCV,
    ST_A_RCV,
    ST_C_RCV,
    ST_BCC_OK,
    ST_STOP
}; 
#endif