#include "../headers/frames.h"

 

int create_SET(unsigned char *frame) { // create a SET frame 
    
    frame[0] = FLAG;
    frame[1] = A_SND;
    frame[2] = C_SND;
    frame[3] = BCC_SND;
    frame[4] = FLAG;
    return 5;

}

int create_UA(unsigned char *frame) { // create a UA frame 
    
    frame[0] = FLAG; 
    frame[1] = A_RCV; 
    frame[2] = C_RCV;
    frame[3] = BCC_RCV; 
    frame[4] = FLAG; 
    return 5;
}

// if returns >= 0 success
int checkIFrame(unsigned char expectedAddressField, unsigned char *frameNumber, unsigned char *packet) {  // packet is where data will be stored 
    
    IFrameState state = IF_START; 
    unsigned char address; // used to calculate bcc2 and bcc1
    unsigned char control;  
    int totalBytes = 0; 
    int idx = 0; // Index to track data 
    unsigned char byte; 
    unsigned char ns;
    
    while (state != IF_STOP && state != IF_BCC1_BAD && state == IF_BCC2_BAD) { // fica a ler, até chegar ao estado final, ou bcc1 errado, ou bcc2 errado 
        
        
        int r = readByteSerialPort(&byte);
        
        if (r < 0) return -1; 

        if (r == 1) {
            ++totalBytes; 
            // mudar estado da maquina de estados pra I Frames
            state = updateIFrameState(state, &byte, expectedAddressField, frameNumber);

            if (state == IF_A_RCV && byte != FLAG) {
                address = byte;
                continue; 
            }

            if (state == IF_C_RCV) {
                control = byte;
                ns = (control >> 6) & 0x01; // check if ns is correct 
                continue;
            }
            
            if (state == IF_BCC1_OK) {
                if (frameNumber == ns) { // se a trama não é duplicada, proceder à leitura dos dados!! 

                }
            }

            if (state == IF_BCC1_BAD) {
                return -1; // retornar -1 significa que bcc1 foi incorretamente calculado! ! 
            }
            
        }
    }
    return totalBytes; 
}


int createIFrame(unsigned char *data, int bufSize) { // creates the IFrame bufSize is the size of the data!! 
    if (bufSize > MAX_PAYLOAD_SIZE) {
        printf("The size of the array is more than the max payload size.\n");
        return -1; 
    }
    unsigned char frame[5 + 2 * (MAX_PAYLOAD_SIZE + 1)]; // worst case possible all bytes need byte stuffing 
    
    frame[0] = FLAG; 
    frame[1] = A_SND; 
    frame[2] = C_SND; 
    frame[3] = A_SND ^ C_SND; 
    
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
    frame[1] = A_SND; 
    if (Ns) frame[2] = 0xAB; 
    else frame[2] = 0xAA; 
    frame[3] = frame[2] ^ frame[1]; 
    frame[4] = FLAG; 
}