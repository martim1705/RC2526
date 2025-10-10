typedef struct {
char serialPort[50]; // Device /dev/ttySx (x = {0,1})
LinkLayerRole role; // Transmitter or receiver role
int baudRate; // Speed of the transmission
int timeout; // Retransmission timeout value in seconds
int nRetransmissions; // Number of retries in case of failure
} LinkLayer;

typedef enum{
LlTx, // Act as transmitter
LlRx, // Act as receiver
} LinkLayerRole;