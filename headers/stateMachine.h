#ifndef STATEMACHINE_H
#define STATEMACHINE_H


// Machine state states 
enum State {
    ST_START,
    ST_FLAG,
    ST_A_Rx,
    ST_A_Tx,
    ST_C_UA_Tx,
    ST_C_UA_Rx,
    ST_C_SET_Tx,
    ST_C_DISC_Rx,
    ST_C_DISC_Tx,
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
    IF_DATA_W,
    IF_DATA_ESC,
    IF_DATA_STUF,
    IF_STOP,
    IF_BCC2_BAD,
    IF_BCC2_OK,
    IF_ERROR_STUFFING,
    IF_DUPLICATE
} IFrameState;

int change_state(unsigned char byte, int *current_state);

IFrameState updateIFrameState(unsigned byte,enum State current_state, unsigned char expectedAddress, unsigned char frameNumber, unsigned char realFrameNumber);  
#endif