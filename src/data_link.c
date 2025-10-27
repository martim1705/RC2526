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

    unsigned char *frame; // used to create UA or SET 
     
    
    if (parameters.role == LlTx) { // if its transmitter
        
        int timeout = parameters.timeout;
        int nRetransmissions = parameters.nRetransmissions; 
        
        if (create_SET(frame) != BUF_SIZE) {
            printf("SET Frame was incorrectly set up.\n");
            return -1;  
        } 

        configAlarm();
        
        
        int nBytes = 0; 


        while (alarmCount < nRetransmissions) 
        {
            if (!alarmEnabled) {
                int bytes = writeBytesSerialPort(frame, BUF_SIZE);
            sleep(1);
            printf("%d bytes written to serial port\n", bytes);
            
            enableAlarm(timeout); // Set alarm to be triggered in 3s
            //alarmEnabled = TRUE;
        }
            // read byte
            
                
            
        unsigned char byte;

        if (readByteSerialPort(&byte) == 1) {
            nBytes += 1;
            printf("Byte read: 0x%02X\n", byte);
            printf("nBytes= %d\n", nBytes); 
            if ( nBytes == 5 && byte == FLAG) {
                printf("All bytes read.\n");
                disableAlarm(); 
                break; 
            }
        } else {
            printf("Byte not read.\n"); 
        }
            }
        
        } else if (parameters.role == LlRx) { // if its receiver
            
            // receive SET - first step with state machine implemented 
            
            unsigned char byte; 
            int current_state = ST_START; // INITIAL STATE

            while (current_state != ST_STOP) { // While current_state is not the last
                
                int r = readByteSerialPort(&byte); 
                if (r == 1) { // while a SET byte is read  

                    change_state(byte, &current_state); // stage changes 
                }
                else if (r < 0) { 
                    perror("No byte was read in receiver serialPort."); 
                    break; 
                }
            }

            printf("SET frame received and read. Starting UA frame creation.\n"); 
            // create UA frame - 2nd step 
            if (create_UA(frame) != BUF_SIZE) {
                printf("UA frame was incorrectly set up.\n");
                return -1; 
            }
            
            
            // send UA - third step 
            if (writeBytesSerialPort(frame, BUF_SIZE) < 0) {
                printf("Bytes could not be sent by sender."); 
                exit(1); 
            }
            printf("Bytes written!");

            sleep(1);

        } else return -1; 

    return 0;
}
     
int llclose();




    


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