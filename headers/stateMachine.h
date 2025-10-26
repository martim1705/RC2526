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

typedef enum {
    IF_START, 
    IF_FLAG_RCV,
    IF_A_RCV,
    IF_C_RCV, 
    IF_BCC1_BAD,
    IF_BCC1_OK,
    IF_DATA,
    IF_DATA_ESC,
    IF_STOP,
    IF_BCC2_BAD,
    IF_BCC2_OK,
    IF_ERROR_STUFFING
} IFrameState;

int change_state(unsigned char byte, int *current_state);

IFrameState updateIFrameState(IFrameState state, unsigned char byte, unsigned char expectedAddress, unsigned char frameNumber);  
#endif