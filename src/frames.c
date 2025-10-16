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

int create_Iframe(unsigned char *frame) { // function to write the I FRAME - THIS IS USED FOR TESTING 
    frame[0] = FLAG; 
    frame[1] = A_SND; 
    frame[2] = C_SND; 
    frame[3] = A_SND ^ C_SND; 
    frame[4] = 0x67; 
    frame[5] = 0x76; 
    frame[6] = frame[4] ^ frame[5]; 
    frame[7] = FLAG; 
    return 8; 
}