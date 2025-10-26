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

    int timeout = parameters.timeout;
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
    }
}

int llread(unsigned char *packet) { // validates I frames and puts data in packet.  
    if (packet == NULL) {
        printf("packet is NULL.\n"); 
        return -1; 
    }
    

    unsigned char expectedFrameNumber = 0; // might be 0 or 1 
    
    while(1) {

        int result = checkIFrame(A_SND, &expectedFrameNumber, packet); // verifies all the I frame, and returns number of data bytes, or any errors 

        if (result > 0) {
            if (expectedFrameNumber == frameNumber) { // in this case Ns is correct 
                // implement byte de-stuffing
            } else {
                continue; // ignore ... 
                // or send RR(Ns) 
            }
         }
    }
    return 0; 
}

// if expected Ns 
                    // parse frame to do the de-stuffing
                    // compute BCC2 
                    // if BCC2 correct 
                        //send rr(ns+1) 
                        //return frame
                    //else send REJ(Ns) 
                // else ignore, or send RR(Ns)