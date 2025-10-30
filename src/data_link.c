#include "../headers/data_link.h"



LinkLayer parameters; 
unsigned char frameNumber;
unsigned char ns = 0x00; // 0x00 or 0x80

int llopen(LinkLayer connectionParameters) { // NOT TESTED

    //printf("a = %d\n b = %d\n", parameters.timeout, parameters.nRetransmissions);
    parameters = connectionParameters; 

    if (openSerialPort(parameters.serialPort, parameters.baudrate) < 0) {
            printf("Serial port opening error.\n");
            return -1; 
        };
    printf("Serial port %s opened\n", parameters.serialPort);

    // Array used to create SET or UA frames
    unsigned char frame[BUF_SIZE];

    if (parameters.role == LlTx) { // if its transmitter
        
        // Create SET frame
        if (create_SET(frame) != BUF_SIZE) {
            printf("SET Frame was incorrectly set up.\n");
            return -1;  
        } 

        // Send SET frame and read UA frame
        configAlarm();
        int timeout = parameters.timeout;
        int nRetransmissions = parameters.nRetransmissions; 
        //printf("timeout: %d",timeout);
        while (alarmCount < nRetransmissions) {
            if (!alarmEnabled) {
                int bytes = writeBytesSerialPort(frame, BUF_SIZE);  // Send SET frame
                printf("%d bytes written to serial port\n", bytes);
                //sleep(1);
                
                enableAlarm(timeout);                               // Set alarm to be triggered in 3s

                if (checkFrame() == 0) {                            // Read UA frame
                    printf("UA frame received from Receiver.\n");
                    break;
                } else {
                    printf("UA frame was incorrectly set up.\n");
                }
            }
        } 

    } else if (parameters.role == LlRx) { // if its receiver
        
        // Receive SET frame
        if (checkFrame() < 0) {
            printf("UA frame was incorrectly set up.\n");
            return -1;
        } else {
            printf("SET frame received and read. Starting UA frame creation.\n");
        }
        
        // Create UA frame
        if (create_UA_Rx(frame) != BUF_SIZE) {
            printf("UA frame was incorrectly set up.\n");
            return -1; 
        }
         
        // Send UA frame
        if (writeBytesSerialPort(frame, BUF_SIZE) < 0) {
            printf("Bytes could not be sent by sender."); 
            exit(1); 
        } else {
            printf("Bytes written!");
        }

        //sleep(1);

    } else return -1; 
    return 0;
}
     
int llclose() { // NOT TESTED

    // Array used to create DISC or UA frames
    unsigned char frame[BUF_SIZE];

    if (parameters.role == LlTx) { // if its transmitter

        // Create transmitter DISC frame
        if (create_DISC_Tx(frame) != BUF_SIZE) {
            printf("DISC Frame was incorrectly set up.\n");
            return -1;  
        }

        // Send transmitter DISC frame and read reveiver DISC frame
        configAlarm();
        int timeout = parameters.timeout;
        int nRetransmissions = parameters.nRetransmissions; 

        while (alarmCount < nRetransmissions) {
            if (!alarmEnabled) {
                int bytes = writeBytesSerialPort(frame, BUF_SIZE);  // Send transmitter DISC frame
                printf("%d bytes written to serial port\n", bytes);
                // sleep(1);
                
                enableAlarm(timeout);                               // Set alarm to be triggered in 3s

                if (checkFrame() == 0) {                            // Read reveiver DISC frame
                    printf("DISC frame received from Receiver.\n");
                    break;
                } else {
                    printf("UA frame was incorrectly set up.\n");
                }
            } 
        }

        // Create UA frame
        if (create_UA_Tx(frame) != BUF_SIZE) {
            printf("UA Frame was incorrectly set up.\n");
            return -1;
        }

        // Send UA frame
        if (writeBytesSerialPort(frame, BUF_SIZE) < 0) {
            printf("Bytes could not be sent by sender."); 
            exit(1); 
        }

    } else if (parameters.role == LlRx) { // if its receiver
        
        // Read transmitter DISC frame
        if (checkFrame() < 0) {
            printf("Transmitter DISC frame was incorrectly set up.\n");
            return -1;
        } else {
            printf("Transmitter DISC frame was received.\n");
        }

        // Create receiver DISC frame
        if (create_DISC_Rx(frame) != BUF_SIZE) {
            printf("Receiver DISC frame was incorrectly set up.\n");
            return -1; 
        }
        
        // Send receiver DISC frame
        if (writeBytesSerialPort(frame, BUF_SIZE) < 0) {
            printf("Bytes could not be sent by receiver."); 
            exit(1); 
        }
        // sleep(1);

        // Read UA frame
        if (checkFrame() < 0) {
            printf("UA frame was incorrectly set up.\n");
            return -1;
        } else {
            printf("UA received, all Done\n");
        }

    } else {
        return -1;
    }
    return 0;
}



int llwrite(const unsigned char *buf, int bufSize) { // NOT TESTED
    
    if (bufSize < 0 || buf == NULL) {
        printf("NULL pointer passed or impossible bufsize value passed.\n");
        return -1; 
    }
    unsigned char Iframe[5 + 2 * (MAX_PAYLOAD_SIZE + 1)]; 

    int createFrame = createIFrame(buf, bufSize, Iframe, ns); 
    
    if (createFrame < 0) {
        printf("I Frame was not created.\n");
        return -1;  
    } 
    printf("Created I Frame successfully with %d bytes.\n", createFrame); 
    // config alarm 
    int timeout = parameters.timeout;
    int nRetransmissions = parameters.nRetransmissions;
    printf("parameters.timeout = %d, parameters.nRetransmissions = %d\n",
       parameters.timeout, parameters.nRetransmissions);

    // int nBytes = 0;
    
    configAlarm(); 

    while (alarmCount < nRetransmissions) {
        // printf("entered loop.\n");
        // printf("alarmCount = %d\nnRetransmissions = %d\n ", alarmCount, nRetransmissions);
        // p rintf("alarmEnabled = %d\n", alarmEnabled); 
        if (alarmEnabled) alarmEnabled = !alarmEnabled; 
        if (alarmEnabled == 0) {
            int bytes = writeBytesSerialPort(Iframe, createFrame);
            // sleep(1);
            printf("%d bytes written to serial port\n", bytes);
            
            enableAlarm(timeout); // Set alarm to be triggered in timeout seconds
            alarmEnabled = !alarmEnabled;
        }

        // read if it is RR or REJ 
        unsigned char response[5]; 
        int res = readResponse(response); 
        printf("result of the response is %d\n", res); 
        if (res == -1) {
            return -1; 
        } else if (res == 0 || res == 1) { // rr(0) or rr(1) 
            ns = ns ^ 0x80;
            alarmEnabled = FALSE; 
            return bufSize; 
        } else if (res == 2 || res == 3) { // rej(0)
            alarmEnabled = FALSE; 
            alarmCount++; 
            continue; 
        } else { // -3 
            printf("Timout or invalid response - retransmitting.\n"); 
            alarmCount++; 
            alarmEnabled = FALSE; 
            continue; 
        }
        
    }
    return -1; // error
}

int llread(unsigned char *packet) { // validates I frames and puts data in packet.  
    if (packet == NULL) {
        printf("packet is NULL.\n"); 
        return -1; 
    }
    

    unsigned char expectedFrameNumber = 0; // might be 0 or 1 
    unsigned char response[5]; 
    while(1) {

        int result = checkIFrame(A_Tx, &expectedFrameNumber, packet); // verifies all the I frame, and returns number of data bytes, or any errors 
        //printf("%d\n", result); 
        // >= 0 sucesso, packet contem o payload (1000 bytes) 
        // -1 leitura errada na porta série 
        // -2 frame duplicada 
        // -3 byte stuffing não foi concluido com sucesso 
        // -4 tamanho do payload é > 1000 bytes 
        // -5 bcc2 está errado 
        // -6 a função deixou de funcionar inesperadamente 
        // -7 bcc1 está errado, máquina de estados atingiu estado "mau"
        
        if (result >= 0) { // send rr(Ns+1)
            //printf("%d\n", expectedFrameNumber); 
            
            //createResponse(response, expectedFrameNumber, result);
            if (createResponse(response, expectedFrameNumber, result) > 0) {
                if (sendResponse(response) < 0) {
                    printf("It was not possible to send the response.\n"); 
                    return -1;
                }
                return 5; 
            } 
            else {
                printf("Resposta não foi criada com sucesso\n"); 
            }
        } else if (result == -1) {
            printf("Byte could not be read correctly from the serial port.\n"); 
        } else if (result == -2) { // send RR(Ns+1)  
            printf("Frame sent is duplicated.\n"); 
            createResponse(response, expectedFrameNumber, result); // created rr frame 
            sendResponse(response); // send rr response  
        } else if (result == -3) {
            printf("Byte Stuffing detected an error.\n");
            return -1;
        } else if (result == -4) {
            printf("Size of data is too much for the current payload size limit.\n"); 
            return -1;
        } else if (result == -5) { // implement rejection REJ(Ns)
            printf("BCC2 is incorrect.\n");
            createResponse(response, expectedFrameNumber, result); 
            sendResponse(response); 
            return -1;
        } else if (result == -6) {
            printf("I Frame could not be processed.\n"); 
            return -1;
        } else { 
            printf("BCC1 is incorrect.\n");
            continue;  
        }

    }
    return 0; 
}
