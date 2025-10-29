#include "../headers/data_link.h"
#include "../headers/alarm_sigaction.h"
#include "../headers/read_noncanonical.h"
#include "../headers/write_noncanonical.h"
#include "../headers/frames.h"
#include "../headers/serial.h"
#include "../headers/stateMachine.h"
#include "../macros/const.h"
#include <stdio.h>

LinkLayer parameters; 
unsigned char frameNumber;
unsigned char ns = 0x00; // 0x00 or 0x80

int llopen(LinkLayer parameters) { // NOT TESTED

    

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

        while (alarmCount < nRetransmissions) {
            if (!alarmEnabled) {
                int bytes = writeBytesSerialPort(frame, BUF_SIZE);  // Send SET frame
                printf("%d bytes written to serial port\n", bytes);
                sleep(1);
                
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

        sleep(1);

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
                sleep(1);
                
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
        sleep(1);

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
    unsigned char Iframe[5 + 2 * (MAX_PAYLOAD_SIZE + 1)]; //

    int createFrame = createIFrame(buf, bufSize, Iframe, ns); 
    
    if (createFrame < 0) {
        printf("I Frame was not created.\n");
        return -1;  
    } 

    // config alarm 
    int timeout = parameters.timeout;
    int nRetransmissions = parameters.nRetransmissions;
    int nBytes = 0;
    
    configAlarm(); 

    while (alarmCount < nRetransmissions) {
        if (!alarmEnabled) {
            int bytes = writeBytesSerialPort(Iframe, createFrame);
            sleep(1);
            printf("%d bytes written to serial port\n", bytes);
            
            enableAlarm(timeout); // Set alarm to be triggered in timeout seconds
            alarmEnabled = TRUE;
        }

        // read if it is RR or REJ 
        unsigned char byte; 
        int read = readByteSerialPort(&byte); 
        // ver se e rej ou rr 
        // usar readResponse() -> identifica que resposta foi dada pelo recetor. a partir do output, age de maneira diferente 
        
    }
}

int llread(unsigned char *packet) { // validates I frames and puts data in packet.  
    if (packet == NULL) {
        printf("packet is NULL.\n"); 
        return -1; 
    }
    

    unsigned char expectedFrameNumber = 0; // might be 0 or 1 
    unsigned char response[5]; 
    while(1) {

        int result = checkIFrame(A_SND, &expectedFrameNumber, packet); // verifies all the I frame, and returns number of data bytes, or any errors 
        
        // >= 0 sucesso, packet contem o payload (1000 bytes) 
        // -1 leitura errada na porta série 
        // -2 frame duplicada 
        // -3 byte stuffing não foi concluido com sucesso 
        // -4 tamanho do payload é > 1000 bytes 
        // -5 bcc2 está errado 
        // -6 a função deixou de funcionar inesperadamente 
        // -7 bcc1 está errado, máquina de estados atingiu estado "mau"
        
        if (result > 0) { // send rr(Ns+1)
            createResponse(response, expectedFrameNumber, result);
            sendResponse(response); 
        } else if (result == -1) {
            printf("Byte could not be read correctly from the serial port.\n"); 
        } else if (result == -2) { // send RR(Ns+1)  
            printf("Frame sent is duplicated.\n"); 
            createResponse(response, expectedFrameNumber, result); // created rr frame 
            sendResponse(response); // send rr response  
        } else if (result == -3) {
            printf("Byte Stuffing detected an error.\n");
        } else if (result == -4) {
            printf("Size of data is too much for the current payload size limit.\n"); 
        } else if (result == -5) { // implement rejection REJ(Ns)
            printf("BCC2 is incorrect.\n");
        } else if (result == -6) {
            printf("I Frame could not be processed.\n"); 
        } else { 
            printf("BCC1 is incorrect.\n");
            continue;  
        }

    }
    return 0; 
}

/*int timeout = parameters.timeout;
    int nRetransmissions = parameters.nRetransmissions;

    configAlarm();

    int nBytes = 0;

    while (alarmCount < nRetransmissions) {
        if (!alarmEnabled) {
            int bytes = writeBytesSerialPort(&buf, bufSize);
            sleep(1);
            printf("%d bytes written to serial port\n", bytes);
            
            enableAlarm(timeout); // Set alarm to be triggered in timeout seconds
            alarmEnabled = TRUE;
        }

        unsigned char byte;

        if (readByteSerialPort(&byte) == 1) { // read supervision byte
            nBytes += 1;
            printf("Byte read: 0x%02X\n", byte);
            printf("nBytes= %d\n", nBytes); 
            if ( nBytes == bufSize && byte == FLAG) {
                printf("All bytes read.\n");
                disableAlarm(); 
                break; 
            }
        }
    }*/