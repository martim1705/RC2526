// Example of how to write to the serial port in non-canonical mode
//
// Modified by: Eduardo Nuno Almeida [enalmeida@fe.up.pt]

#include "../headers/write_noncanonical.h"
#include "../macros/const.h"
#include "../headers/serial.h"
#include "../headers/alarm_sigaction.h"
#include <stdlib.h>
#include <stdio.h>
// alarm MACROS 
int alarmEnabled = FALSE;
int alarmCount = 0; 


// alarm handler code 
void alarmHandler(int signal)
{
    alarmEnabled = FALSE;
    alarmCount++;

    printf("Alarm #%d received\n", alarmCount);
} 



// ---------------------------------------------------
// MAIN
// ---------------------------------------------------
void run_sender(int argc, char *argv[])
{
    printf("Initiating run_sender().\n");
    // Open serial port device for reading and writing, and not as controlling tty
    // because we don't want to get killed if linenoise sends CTRL-C.
    //
    // NOTE: See the implementation of the serial port library in "serial_port/".
    const char *serialPort = argv[1];

    if (openSerialPort(serialPort, BAUDRATE) < 0)
    {
        perror("openSerialPort");
        exit(-1);
    }

    printf("Serial port %s opened\n", serialPort);




    // Set alarm function handler.
    // Install the function signal to be automatically invoked when the timer expires,
    // invoking in its turn the user function alarmHandler
    struct sigaction act = {0};
    act.sa_handler = &alarmHandler;
    if (sigaction(SIGALRM, &act, NULL) == -1) // configurar o alarme
    {
        perror("sigaction");
        exit(1);
    }

    printf("Alarm configured\n");





    // Create string to send
    unsigned char buf[BUF_SIZE] = {FLAG, A_SND, C_SND, BCC_SND, FLAG};
    int nBytes = 0;
     
    
    // In non-canonical mode, '\n' does not end the writing.
    // Test this condition by placing a '\n' in the middle of the buffer.
    // The whole buffer must be sent even with the '\n'.
    


    // Wait until all bytes have been written to the serial port
    //sleep(1);
 

    while (alarmCount < 4 && STOP == FALSE)
    {
        // number of incoming bytes that were read
         
        
        
        
        
        if (alarmEnabled == FALSE) {
            int bytes = writeBytesSerialPort(buf, BUF_SIZE);
            sleep(1);
            printf("%d bytes written to serial port\n", bytes);
            alarm(3); // Set alarm to be triggered in 3s
            alarmEnabled = TRUE;
        }
            // read byte
            
                
            
        unsigned char byte;
        if (readByteSerialPort(&byte) == 1) {
            nBytes += 1;
            printf("Byte read: 0x%02X\n", byte);
            printf("nBytes= %d\n", nBytes); 
            if ( nBytes == 5 && byte == FLAG) {
                printf("All bytes read.\n");
                STOP = TRUE;
                alarm(0);
                break; 
            }
        } else {
            printf("Byte not read.\n"); 
        }
    }

    printf("Ending program\n");
    


    // Close serial port
    if (closeSerialPort() < 0)
    {
        perror("closeSerialPort");
        exit(-1);
    }

    printf("Serial port %s closed\n", serialPort);

}