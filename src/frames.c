#include "../headers/frames.h"
#include "../headers/serial.h"
#include <stdio.h>

 

int create_SET(unsigned char *frame) { // create a SET frame 
    
    frame[0] = FLAG;
    frame[1] = A_Tx;
    frame[2] = C_SET;
    frame[3] = BCC_SET;
    frame[4] = FLAG;
    return 5;
}

int create_UA_Rx(unsigned char *frame) { // create a UA frame 
    
    frame[0] = FLAG; 
    frame[1] = A_Rx; 
    frame[2] = C_UA;
    frame[3] = BCC_UA_Rx; 
    frame[4] = FLAG; 
    return 5;
}

int create_UA_Tx(unsigned char *frame) { // create a UA frame 
    
    frame[0] = FLAG; 
    frame[1] = A_Tx; 
    frame[2] = C_UA;
    frame[3] = BCC_UA_Tx; 
    frame[4] = FLAG; 
    return 5;
}

int create_DISC_Tx(unsigned char *frame) {

    frame[0] = FLAG; 
    frame[1] = A_Tx; 
    frame[2] = C_DISC;
    frame[3] = BCC_DISC_Tx; 
    frame[4] = FLAG;
    return 5;
}

int create_DISC_Rx(unsigned char *frame) {

    frame[0] = FLAG; 
    frame[1] = A_Rx; 
    frame[2] = C_DISC;
    frame[3] = BCC_DISC_Rx; 
    frame[4] = FLAG;
    return 5;
}

int checkIFrame(unsigned char expectedAddressField, unsigned char *frameNumber, unsigned char *packet) {  
    
    IFrameState state = IF_START; 
    //unsigned char  confirmBCC = 0; // used to calculate bcc2              variable not being used yet
    int totalBytes = 0; 
    //int idx = 0;                                                          variable not being used yet
    unsigned char byte;

    while (state != IF_STOP) {
        
        int r = readByteSerialPort(&byte);
        
        if (r < 0) return -1; 

        if (r == 1) {
            ++totalBytes; 
            // mudar estado da maquina de estados pra I Frames
            state = updateIFrameState(state, byte, expectedAddressField, *frameNumber);

            if (state == IF_BCC1_OK) {
                // check NS 
                // if expected Ns 
                    // parse frame to do the de-stuffing
                    // compute BCC2 
                    // if BCC2 correct 
                        //send rr(ns+1) 
                        //return frame
                    //else send REJ(Ns) 
                // else ignore, or send RR(Ns)  
            }
        }
    }
    return 0;
}


int createIFrame(unsigned char *data, int bufSize) { // creates the IFrame bufSize is the size of the data!! 
    if (bufSize > MAX_PAYLOAD_SIZE) {
        printf("The size of the array is more than the max payload size.\n");
        return -1; 
    }
    unsigned char frame[5 + 2 * (MAX_PAYLOAD_SIZE + 1)]; // worst case possible all bytes need byte stuffing
    
    frame[0] = FLAG; 
    frame[1] = A_Tx; 
    frame[2] = C_SET; 
    frame[3] = A_Tx ^ C_SET; 
    
    unsigned char bcc2 = 0x00; 

    for (int i = 0; i < bufSize; i++) {
        bcc2 ^= data[i]; 
        frame[4 + i] = data[i]; 
    }

    frame[4 + bufSize] = bcc2; 
    frame[5+bufSize] = FLAG; 

    return bufSize + 6;     
} 


int createRR(unsigned char *frame, unsigned char Ns) { // CONTINUE AT HOME!!!!
    frame[0] = FLAG; 
    frame[1] = A_Tx; 
    if (Ns) frame[2] = 0xAB; 
    else frame[2] = 0xAA; 
    frame[3] = frame[2] ^ frame[1]; 
    frame[4] = FLAG;
    return 5;
}