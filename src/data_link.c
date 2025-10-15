#include "../headers/data_link.h"
#include "../headers/alarm_sigaction.h"
#include "../headers/read_noncanonical.h"
#include "../headers/write_noncanonical.h"
#include "../headers/frames.h"
#include "../headers/serial.h"
#include "../headers/stateMachine.h"
#include "../macros/const.h"
#include <stdio.h>

 
alarmStates alarmState; 

int llopen(LinkLayer parameters) {

    if (openSerialPort(parameters.serialPort, parameters.baudrate) < 0) {
            printf("Serial port opening error.\n");
            return -1; 
        };  

    printf("Serial port %s opened\n", parameters.serialPort);

    unsigned char *frame;
     
    
    if (!strcmp(parameters.role, "LlTx")) {
        
        
        if (create_SET(frame) != BUF_SIZE) {
            printf("SET Frame was incorrectly set up.\n");
            return -1;  
        } 

        configAlarm(alarmState);
        
        
        int nBytes = 0; 


        while (alarmState.alarmCount < parameters.nRetransmissions) 
        {
            if (!alarmState.alarmEnabled) {
                int bytes = writeBytesSerialPort(frame, BUF_SIZE);
            sleep(1);
            printf("%d bytes written to serial port\n", bytes);
            enableAlarm(parameters.timeout); // Set alarm to be triggered in 3s
            alarmState.alarmEnabled = TRUE;
        }
            // read byte
            
                
            
        unsigned char byte;

        if (readByteSerialPort(&byte) == 1) {
            nBytes += 1;
            printf("Byte read: 0x%02X\n", byte);
            printf("nBytes= %d\n", nBytes); 
            if ( nBytes == 5 && byte == FLAG) {
                printf("All bytes read.\n");
                disableAlarm(alarmState); 
                break; 
            }
        } else {
            printf("Byte not read.\n"); 
        }
            }
        
        } else if (!strcmp(parameters.role,"LlRx")) {
            
            // receive SET - first step with state machine implemented 
            int current_state = ; 
            


            // create UA frame - 2nd step 
            if (create_UA(frame) != BUF_SIZE) {
                printf("UA frame was incorrectly set up.\n");
                return -1; 
            }
            
            
            // send UA - third step 


        } else return -1; 

    return 0;
}
     
int llclose();




    


int llwrite(const unsigned char *buf, int bufSize) {
    if (bufSize < 0) {
        printf("bufSize must be equal to buffer size. Currently bufSize is equal to %d\n",bufSize);
        return -1; 
    }

    if (buf == NULL) {
        printf("buf was passed as a null pointer.\n");
        return -1; 
    }
}