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

int checkIFrame(unsigned char *frame, int frameSize) { // checks if frame with length frameSize is a I frame 
    if (frameSize < 6) return 0; 
    if (frame[0] != FLAG || frame[frameSize-1] != FLAG) return 0; 
    
    unsigned char A = frame[1]; 
    unsigned char C = frame[2];
    unsigned char BCC1 = frame[3];

    if (A != 0x03 && A != 0x01) return 0; 

    if ((C & 0x01) != 0) return 0; 

    if (BCC1 != (A ^ C)) return 0; 


    unsigned char c_BCC2 = 0x00; 
    for (int i = 4; i < (frameSize -2); i++) {
        c_BCC2 ^= frame[i]; 
    }

    unsigned char BCC2 = frame[frameSize-2]; 
    if (BCC2 != c_BCC2) return 0; 

    return 1; 

}