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
    //unsigned char address; 
    unsigned char control;  
    //int totalBytes = 0;  
    unsigned char byte; 
    unsigned char ns;
    
    while (state != IF_STOP && state != IF_BCC1_BAD && state != IF_BCC2_BAD) { // fica a ler, até chegar ao estado final, ou bcc1 errado, ou bcc2 errado 
        
        
        int r = readByteSerialPort(&byte);
        
        if (r < 0) return -1; 

        if (r == 1) {
            //++totalBytes; 
            // mudar estado da maquina de estados pra I Frames
            state = updateIFrameState(state, &byte, expectedAddressField, frameNumber);

            if (state == IF_A_RCV) {
                //address = byte;
                continue; 
            }

            if (state == IF_C_RCV) {
                control = byte; 
                continue;
            }
            
            if (state == IF_BCC1_OK) {
                ns = (control >> 6) & 0x01;
                if (*frameNumber == ns) { // se a trama é duplicada, proceder a retornar erro!! 
                    printf("Frame is duplicated!\n"); 
                    return -2; 
                }
                else {
                    unsigned char confirmBCC2 = 0; 
                    int idx = 0;  
                    int escaped = 0; // 0 = falso 
                    
                    while (1) {
                        r = readByteSerialPort(&byte); 
                        if (r < 0) return -1; 

                        if (byte == FLAG) {
                            if (confirmBCC2 == 0) {
                                state = IF_BCC2_OK; 
                                break; 
                            } else {
                                state = IF_BCC2_BAD; 
                                break; 
                            }
                        }

                        if (!escaped && byte == ESC) {
                            escaped = 1;
                            continue; 
                        }

                        if (escaped) {
                            if (byte == 0x5E) byte = 0x7E; 
                            else if (byte == 0x5D) byte = 0x7D; 
                            else {
                                printf("byte stuffing wrong.\n"); 
                                return -3; 
                            }
                            escaped = 0; 
                        }
                        if (idx >= MAX_PAYLOAD_SIZE) {
                            printf("Payload size exceeded!\n");
                            return -4;
                        }
                        packet[idx++] = byte; 
                        confirmBCC2 ^= byte; 
                    }

                    if (state == IF_BCC2_OK) {
                        *frameNumber = !(*frameNumber); 
                        return idx; 
                    } else if (state == IF_BCC2_BAD) {
                        printf("BCC2 is wrong.\n"); 
                        return -5; 
                    }
                }
            } 

            if (state == IF_BCC1_BAD) {
                return -7; // retornar -1 significa que bcc1 foi incorretamente calculado! ! 
            }
            
        }
        
    } 
    printf("checkIFrame ended unexpectedly.\n");
    return -6;

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