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

// if returns >= 0 success
int checkIFrame(unsigned char expectedAddressField, unsigned char *expectedframeNumber, unsigned char *packet) {  // packet is where data will be stored 
    
    IFrameState state = IF_START;   
    unsigned char byte; 
    unsigned char realFrameNumber; // 
    
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
                if (realFrameNumber != *expectedframeNumber) {
                    printf("Frame duplicada recebida! Esperava %d, recebeu %d\n", expectedAddressField, realFrameNumber);
                    return -2; // frame duplicada
                }
                //ns = (control >> 6) & 0x01;
                //if (*frameNumber != ns) { // se a trama é duplicada, proceder a retornar erro!! 
                //    printf("Frame is duplicated!\n"); 
                //    return -2; 
                //}
                //else {
                unsigned char confirmBCC2 = 0; 
                int idx = 0;  
                int escaped = 0; // 0 = falso 
                unsigned char prevByte = 0; 
                int havePrev = 0; 

                while (1) {
                    r = readByteSerialPort(&byte);
                     

                    if (r < 0) return -1; 
                    if (byte == FLAG) {
                        if (!havePrev) {
                            return -6; // no bcc2 received 
                        }

                        unsigned char bcc2 = prevByte; 
                        if ((confirmBCC2 ^ bcc2) == 0) { // confirmBCC2 == 0 porque (D1 XOR D2 XOR ...XOR DN) XOR BCC2 == 0
                            state = IF_BCC2_OK;  
                        } else {
                            state = IF_BCC2_BAD;  
                        }
                        break;
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
                    if (havePrev) {
                        if (idx >= MAX_PAYLOAD_SIZE) {
                            printf("Payload size exceeded!\n");
                            return -4;
                        }
                        packet[idx++] = prevByte; 
                        confirmBCC2 ^= prevByte; 
                    }
                    
                    prevByte = byte; 
                    havePrev = 1;  
                }



                if (state == IF_BCC2_OK ) {
                    if (*expectedframeNumber == realFrameNumber) {
                        *expectedframeNumber = !(*expectedframeNumber);
                        return idx;
                    }
                    else return -2;  // BCC2 is correct, but frame is a duplicate -> send rr(ns) 

                } 
                
                if (state == IF_BCC2_BAD) { // bcc2 is incorrect 
                    if (realFrameNumber == *expectedframeNumber) return -5; // frame is not a duplicate, but is rejected -> discard data & send REJ 
                    else return -2;// bcc2 is incorrect and frame is duplicate -> discard data & send RR 
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
    if (bufSize > MAX_PAYLOAD_SIZE || bufSize < 0) {
        printf("The payload size is invalid.Must be less than %d bytes.\n", MAX_PAYLOAD_SIZE);
        return -1; 
    }
    
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



int createResponse(unsigned char *frame, unsigned char Ns, int code) { 
    
    frame[0] = FLAG;
    frame[1] = A_Rx; 
    
    if (code == -2 || code > 0) {  
        if (Ns) frame[2] = 0xAB; 
        else frame[2] = 0xAA;  
        
    } else if (code == -5) {
        if (Ns) frame[2] = 0x54; 
        else frame[2] = 0x55;  
         
    } else {
        printf("Invalid code for RR/REJ creation: %d\n", code);
        return 0; // error
    }
    
    frame[3] = frame[1] ^ frame[2];
    frame[4] = FLAG;

    printf("Created %s frame for Ns=%d\n",
           (code == -5 ? "REJ" : "RR"), Ns);


    return 5; // success
}

int sendResponse(unsigned char *frame) {

    int send = writeBytesSerialPort(frame, 5); 
    if (send == 5) return 1;
    else if (send < 0) {
        printf("Error: could not write to serial port.\n");
        return 0;
    } else return 0; //error 
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
     
    if (frame[0] != FLAG && frame[4] != FLAG) {
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
            printf("Byte read: 0x%02X\n", byte);
        }
        else if (r < 0) { 
            perror("No byte was read in receiver serialPort."); 
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