#include "../headers/frames.h"



int create_SET(unsigned char *frame) {
    
    frame[0] = FLAG;
    frame[1] = A_SND;
    frame[2] = C_SND;
    frame[3] = BCC_SND;
    frame[4] = FLAG;
    return 5;

}

int create_UA(unsigned char *frame) {
    
    frame[0] = FLAG; 
    frame[1] = A_RCV; 
    frame[2] = C_RCV;
    frame[3] = BCC_RCV; 
    frame[4] = FLAG; 
    return 5;
}

int create_DISC_Tx(unsigned char *frame) {
    frame[0] = FLAG; 
    frame[1] = A_SND; 
    frame[2] = C_DISC;
    frame[3] = BCC_DISC_TX; 
    frame[4] = FLAG;
}

int create_DISC_Rx(unsigned char *frame) {
    frame[0] = FLAG; 
    frame[1] = A_RCV; 
    frame[2] = C_DISC;
    frame[3] = BCC_DISC_RX; 
    frame[4] = FLAG;
}