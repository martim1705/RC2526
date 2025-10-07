#include "../headers/read_noncanonical.h"
#include "../headers/serial.h"
#include "../headers/stateMachine.h"

// ---------------------------------------------------
// MAIN function
// ---------------------------------------------------
void run_receiver(int argc, char *argv[])
{


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
int current_state = ST_START;


while (current_state != ST_STOP) {

    nBytesBuf += 1; 
    if (readByteSerialPort(&byte) < 0) {
        printf("Byte coul not be read");
        exit(0);
    }
    
    printf("byte = 0x%02X\n", byte);
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
                    current_state = ST_STOP;
            } else {
                current_state = ST_START;
            }
            break;
        default:
            printf("Impossible state reached.");
            break; 
    }
}

printf("Total bytes received: %d\n", nBytesBuf);



unsigned char buf[BUF_SIZE] = {FLAG, A_RCV, C_RCV , BCC_RCV, FLAG};

writeBytesSerialPort(buf, BUF_SIZE);
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

