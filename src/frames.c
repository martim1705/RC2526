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
                //ns = (control >> 6) & 0x01;
                //if (*frameNumber != ns) { // se a trama é duplicada, proceder a retornar erro!! 
                //    printf("Frame is duplicated!\n"); 
                //    return -2; 
                //}
                //else {
                unsigned char confirmBCC2 = 0; 
                int idx = 0;  
                int escaped = 0; // 0 = falso 
                
                while (1) {
                    r = readByteSerialPort(&byte); 
                    if (r < 0) return -1; 
                    if (byte == FLAG) {
                        if (confirmBCC2 == 0) { // confirmBCC2 == 0 porque (D1 XOR D2 XOR ...XOR DN) XOR BCC2 == 0
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
                    if (idx >= MAX_PAYLOAD_SIZE) {
                        printf("Payload size exceeded!\n");
                        return -4;
                    }
                    packet[idx++] = byte; 
                    confirmBCC2 ^= byte; 
                }

                ns = (control >> 6) & 0x01;


                if (state == IF_BCC2_OK) {
                    // send RR 
                    if (ns == *frameNumber) {
                        *frameNumber = !(*frameNumber);
                        return idx;
                    } // success
                    else continue;  // BCC2 is correct, but frame is a duplicate -> ignore 
                } 

                if (state == IF_BCC2_BAD) { // bcc2 is incorrect 
                    if (ns == *frameNumber) return -5; // frame is not a duplicate, but is rejected -> discard data & send REJ 
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


int createIFrame(unsigned char *data, int bufSize, unsigned char *frame, int frameSize) { // creates the IFrame . bufSize is the size of the data!! 
    if (bufSize > MAX_PAYLOAD_SIZE || bufSize < 0) {
        printf("The payload size is invalid.Must be less than %d bytes.\n", MAX_PAYLOAD_SIZE);
        return -1; 
    }
    
    frame[0] = FLAG; 
    frame[1] = A_SND; 
    frame[2] = C_SND; 
    frame[3] = A_SND ^ C_SND; 
    
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


int createResponse(unsigned char *frame, unsigned char Ns, int code) { // CONTINUE AT HOME!!!!
    
    frame[0] = FLAG;
    frame[1] = A_RCV; 
    
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