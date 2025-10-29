// FLAG byte
#define FLAG 0x7E
// Address bytes 
#define A_Tx 0x03 
#define A_Rx 0x01

//Control bytes 
#define C_SET 0x03
#define C_UA 0x07
#define C_DISC 0x0B
#define BCC_SET (A_Tx ^C_SET)
#define BCC_UA_Rx (A_Rx ^C_UA)
#define BCC_UA_Tx (A_Tx ^C_UA)
#define BCC_DISC_Tx (A_Tx ^C_DISC)
#define BCC_DISC_Rx (A_Rx ^C_DISC)

#define FALSE 0
#define TRUE 1


#define ESC 0x7D

#define _POSIX_SOURCE 1 // POSIX compliant source

#define BAUDRATE 38400
#define BUF_SIZE 5

#define MAX_PAYLOAD_SIZE 1000