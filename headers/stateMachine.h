#ifndef STATEMACHINE_H
#define STATEMACHINE_H


// Machine state states 
enum State {
    ST_START,
    ST_FLAG,
    ST_A_RCV,
    ST_A_SND,
    ST_C_RCV,
    ST_C_SND,
    ST_C_DISC_RCV,
    ST_C_DISC_SND,
    ST_BCC_OK,
    ST_STOP
};

int change_state(unsigned char byte, int *current_state);

#endif