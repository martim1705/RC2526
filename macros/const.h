// FLAG byte
#define FLAG 0x7E
// Address bytes 
#define A_SND 0x03 
#define A_RCV 0x01

//Control bytes 
#define C_SND 0x03
#define C_RCV 0x07
#define BCC_SND A_SND ^C_SND
#define BCC_RCV A_RCV ^C_RCV

#define FALSE 0
#define TRUE 1


#define _POSIX_SOURCE 1 // POSIX compliant source

#define BAUDRATE 38400
#define BUF_SIZE 5