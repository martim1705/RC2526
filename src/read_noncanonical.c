#include "../headers/read_noncanonical.h"
#include "../headers/serial.h"
#include "../headers/stateMachine.h"
#include <stdlib.h>
#include <stdio.h>
// ---------------------------------------------------
// MAIN function
// ---------------------------------------------------
void run_receiver(int argc, char *argv[])
{

printf("Initiating run_receiver().\n");
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

// state machine implementation 

int nBytesBuf = 0;


unsigned char byte; 

while (readByteSerialPort(&byte) == 0) { 
}
int current_state = ST_START;

while (current_state != ST_STOP) {

     
    
    printf("byte = 0x%02X\n", byte);
    printf("current_state= %d\n", current_state);
    switch(current_state) {
         
        case ST_START:
            if (byte == FLAG) {
                current_state = ST_FLAG_RCV;
                 
            } 
            break;
        case ST_FLAG_RCV:
        
            if (byte == A_SND) {
                current_state = ST_A_RCV; 
            } else if (byte == FLAG) {
                current_state = ST_FLAG_RCV; 
            } else {
                current_state = ST_START; 
            }
            break; 
        case ST_A_RCV:
            if (byte == C_SND) {
                current_state = ST_C_RCV;
            } else if (byte == FLAG) {
                current_state = ST_FLAG_RCV; 
            } else {current_state = ST_START;}
                break; 
        case ST_C_RCV:
            if (byte == BCC_SND) {
                current_state = ST_BCC_OK; 
            } else if (byte == FLAG) {
                current_state = ST_FLAG_RCV; 
            } else {
                current_state = ST_START; 
            }
            break; 
        case ST_BCC_OK:
            if (byte == FLAG) {
                    printf("byte == 0x7E\n");
                    current_state = ST_STOP;
            } else {
                current_state = ST_START;
            }
            break;
        case ST_STOP:
            break; 
        default:
            printf("Impossible state reached.");
            break; 
    }
    nBytesBuf += 1; 
    if (current_state != 5)
    readByteSerialPort(&byte);
}

printf("Total bytes received: %d\n", nBytesBuf);



unsigned char buf[BUF_SIZE] = {FLAG, A_RCV, C_RCV , BCC_RCV, FLAG};

if (writeBytesSerialPort(buf, BUF_SIZE) < 0) {
    printf("Bytes could not be sent by sender."); 
    exit(1); 
}
printf("Bytes written!");

sleep(1);

// Close serial port
if (closeSerialPort() < 0)
{
perror("closeSerialPort");
exit(-1);
}

printf("Serial port %s closed\n", serialPort);


}


