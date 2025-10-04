#include "../headers/read_noncanonical.h"
#include "../headers/serial.h"


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

// Read from serial port until the 'z' char is received.

// NOTE: This while() cycle is a simple example showing how to read from the serial port.
// It must be changed in order to respect the specifications of the protocol indicated in the Lab guide.

// TODO: Save the received bytes in a buffer array and print it at the end of the program.
int nBytesBuf = 0;

while (STOP == FALSE)
{
// Read one byte from serial port.
// NOTE: You must check how many bytes were actually read by reading the return value.
// In this example, we assume that the byte is always read, which may not be true.
unsigned char byte;
int bytes = readByteSerialPort(&byte);
nBytesBuf += bytes;

printf("Byte received: 0x%02X\n", byte);

if (nBytesBuf == 5 && byte == FLAG)
{
STOP = TRUE;
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

