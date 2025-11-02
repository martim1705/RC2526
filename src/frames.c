#include "../headers/frames.h"


 

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

// if returns >= 0 success
int checkIFrame(unsigned char expectedAddressField, unsigned char *expectedframeNumber, unsigned char *packet) {  // packet is where data will be stored 
    
    IFrameState state = IF_START;   
    unsigned char byte; 
    unsigned char realFrameNumber; // vai guardar o frame Number que está no I frame
    
    while (state != IF_STOP && state != IF_BCC1_BAD && state != IF_BCC2_BAD) { // fica a ler, até chegar ao estado final, ou bcc1 errado, ou bcc2 errado 
        
        
        int r = readByteSerialPort(&byte);
        
        if (r < 0) return -1; 

        if (r == 1) {
            //++totalBytes; 
            // mudar estado da maquina de estados pra I Frames
            state = updateIFrameState(byte, state, expectedAddressField, &realFrameNumber);

            
            if (state == IF_A_RCV || state == IF_C_RCV) {
                continue; 
            }

            
            if (state == IF_BCC1_OK) {
                if (realFrameNumber == *expectedframeNumber) {
                    
                    unsigned char confirmBCC2 = 0; 
                    int idx = 0;  
                    int escaped = 0; 
                    unsigned char prevByte = 0; 
                    int havePrev = 0; 
                
                    while (1) {
                        r = readByteSerialPort(&byte);
                        if (r < 0) return -1; 
                    
                        if (byte == FLAG) {
                            if (!havePrev) return -6; // no bcc2 received 
                            unsigned char bcc2 = prevByte; 
                            state = ((confirmBCC2 ^ bcc2) == 0) ? IF_BCC2_OK : IF_BCC2_BAD;
                            break;
                        }
                    
                        if (!escaped && byte == ESC) {
                            escaped = 1;
                            continue; 
                        }
                    
                        if (escaped) {
                            if (byte == 0x5E) byte = 0x7E; 
                            else if (byte == 0x5D) byte = 0x7D; 
                            else return -3; 
                            escaped = 0; 
                        }
                    
                        if (havePrev) {
                            if (idx >= MAX_PAYLOAD_SIZE + 4) return -4;
                            packet[idx++] = prevByte; 
                            confirmBCC2 ^= prevByte; 
                        }
                    
                        prevByte = byte; 
                        havePrev = 1;  
                    }
                
                    // Depois de ler payload e BCC2:
                    if (state == IF_BCC2_OK) { 
                        return idx; // sucesso
                    }
                    if (state == IF_BCC2_BAD) {
                        state = IF_STOP;
                        return -5;
                    } // erro de BCC2
                
                } else {
                    printf("Duplicated frame received! Expected %d, received %d\n", *expectedframeNumber, realFrameNumber);
                    return -2;
                }


                
            }

            if (state == IF_BCC1_BAD) {
                return -7; // retornar -1 significa que bcc1 foi incorretamente calculado!! Enviar REJ  
            }
            
        }
        
    } 
    printf("checkIFrame ended unexpectedly.\n");
    return -6;

}


int createIFrame(const unsigned char *data, int bufSize, unsigned char *frame, unsigned char ns) {   
    //if (bufSize > MAX_PAYLOAD_SIZE || bufSize < 0) {
    //    printf("The payload size is invalid.Must be less than %d bytes.\n", MAX_PAYLOAD_SIZE);
    //    return -1; 
    //}
    
    frame[0] = FLAG; 
    frame[1] = A_Tx; 
    frame[2] = ns; 
    frame[3] = A_Tx ^ ns; 
    
    unsigned char bcc2 = 0; 
    int idx = 4; 
    for (int i = 0; i < bufSize; i++) {
        if (data[i] == FLAG) {
            frame[idx++] = 0x7D;   
            frame[idx++] = 0x5E;

        } else if (data[i] == ESC) {
            frame[idx++] = 0x7D;  
            frame[idx++] = 0x5D;
        } else {
            frame[idx++] = data[i]; 
        }
        bcc2 ^= data[i];
    }

    if (bcc2 == FLAG) {
        frame[idx++] = 0x7D;   
        frame[idx++] = 0x5E; 

    } else if (bcc2 == ESC) {
        frame[idx++] = 0x7D;  
        frame[idx++] = 0x5D;

    } else {
        frame[idx++] = bcc2; 
    }
    
    frame[idx++] = FLAG; 

    return idx;     
} 



int createResponse(unsigned char *frame, unsigned char expectedFrameNumber, int code) { 
    
    frame[0] = FLAG;
    frame[1] = A_Rx; 
    
    if (code == -2 || code > 0) {  
        if (expectedFrameNumber) frame[2] = 0xAB; 
        else frame[2] = 0xAA;  
        
    } else if (code == -5) {
        if (expectedFrameNumber) frame[2] = 0x54; 
        else frame[2] = 0x55;  
        
    } else {
        printf("Invalid code for RR/REJ creation: %d\n", code);
        return 0; // error
    }

    frame[3] = frame[1] ^ frame[2];
    frame[4] = FLAG;

    printf("Created %s frame for Ns=%d\n",
           (code == -5 ? "REJ" : "RR"), expectedFrameNumber);

    
    return 5; // success
}

int sendResponse(unsigned char *frame) {

    int send = writeBytesSerialPort(frame, 5);
    if (send == 5) return 1;
    else if (send < 0) {
        printf("Error: could not write to serial port.\n");
        return 0;
    } else return 0;
}


int readResponse(unsigned char *frame) { // returns a number corresponding to rr or rej 
    if (frame == NULL) {
        printf("Passed argument in null pointer.\n"); 
        return 0;
    }
    
    int idx = 0;
    unsigned char byte;

    while (1) {
        int r = readByteSerialPort(&byte);
        
        if (r < 0) {
            printf("Byte could not be read.\n");
            return -1;
        }

        if (byte == FLAG) {
            if (idx == 0) {
                frame[idx++] = byte;
                continue;
            } else {
                frame[idx++] = byte;
                break;
            }
        }

        frame[idx++] = byte;  

    }

    if (idx < 5) {
        printf("Incomplete frame (only %d bytes read)\n", idx);
        return -1;
    }
     
    if (frame[0] != FLAG || frame[4] != FLAG) {
        return -1; 
    }

    unsigned char A = frame[1]; 
    unsigned char C = frame[2]; 
    unsigned char bcc1 = frame[3]; 

    if ((A ^ C) != bcc1) return -3;  // bcc1 incorrect 
    
    if (C == 0xAA) return 0; // rr(0) 
    if (C == 0xAB) return 1; // rr(1) 
    if (C == 0x54) return 2; // rej(0)
    if (C == 0x55) return 3; // rej(1)
     
    return -1; 
}


int checkFrame() {
    unsigned char byte; 
    int current_state = ST_START; // INITIAL STATE

    while (current_state != ST_STOP) { // While current_state is not the last
        
        int r = readByteSerialPort(&byte); 
        if (r == 1) { // while a SET byte is read  
            change_state(byte, &current_state); // stage changes 
        }
        else if (r < 0) { 
            perror("No byte was read in receiver serialPort.\n"); 
            return -1;
        }
    }
    return 0;
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