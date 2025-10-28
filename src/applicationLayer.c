#include "../headers/applicationLayer.h"


unsigned char Cpacket[264];

int sendFile(FILE* file) {
    if (file == NULL) {
        printf("null pointer given.\n"); 
        return -1; 
    }
    unsigned char data[MAX_PAYLOAD_SIZE]; 
    unsigned char packet[MAX_PAYLOAD_SIZE + 4]; 
    unsigned char seqNum = 0; 
    size_t bytesRead; 

    while ((bytesRead = fread(data, 1, MAX_PAYLOAD_SIZE, file)) > 0) {
        int packetSize = buildDataPacket(packet, data, bytesRead, seqNum); 
        if (packetSize < 0) {
            printf("Data packet building error.\n");
            return -1;
        }

        int res = llwrite(packet, packetSize); 
        if (res < 0) {
            printf("Packet could not be sent.\n"); 
            return -1; 
        }

        seqNum = (seqNum + 1) % 256; 

    }

    return 0; 
}

void appConfig(const char *serialPort, const char* role, int baudrate, int timeout, int nretransmissions, const char* filename) {
    
    if (filename == NULL || serialPort == NULL || role == NULL) {
        printf("filename, serialPort, or role parameters were passed as NULL.\n");
        return; 
    }

    LinkLayer parameters; 
    strcpy(parameters.serialPort, serialPort);
    parameters.baudrate = baudrate; 
    parameters.nRetransmissions = nretransmissions; 
    parameters.timeout = timeout; 
    if (!strcmp(role, "LlTx")) {
        parameters.role = LlTx; 
    } else {
        parameters.role = LlRx; 
    } 
    

    if (llopen(parameters) < 0) {
        printf("Error calling llopen().\n"); 
        return; 
    }

    if (parameters.role == LlTx) {
        
        FILE *file = fopen(filename, "rb");
        
        long int fileSize = getFileSize(filename); // returns the size of filename 
        
        if (fileSize < 0) {
            return; 
        }    
        
        int CpacketSize = buildControlPacketunsigned(Cpacket, filename, fileSize, 1); // returns the size of the START control packet 
        
        if ( CpacketSize < 0) {
            printf("Could not contruct START Control packet.\n"); 
            return; 
        }

        llwrite(Cpacket, CpacketSize); // sends the START packet 
         
        
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }

        if (sendFile(file) < 0) {
            printf("Could not send file.\n"); 
            fclose(file); 
            return; 
        }

        CpacketSize = buildControlPacketunsigned(Cpacket, filename, fileSize,3); 
        
        if ( CpacketSize < 0) {
            printf("Could not contruct END Control packet.\n"); 
            return; 
        }

        llwrite(Cpacket, CpacketSize); 

        fclose(file); 
    } else { // receiver 

        unsigned char packet[MAX_PAYLOAD_SIZE + 4]; 
        int packetSize; 
        char filename[256];
        long int fileSize = 0; 
        
        packetSize = llread(packet); 

        if (packetSize < 0) {
            printf("START packet was not read.\n");
            return;  
        }

        if (packet[0] != 1) {
            printf("Expected START packet.\n"); 
            return; 
        }

        if (readControlPacket(packet, &fileSize, packetSize, filename) < 0) {
            printf("Coulr not read START packet.\n"); 
            return; 
        }

        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Could not create file named: %s", filename);
            return;
        }  
        
        while (1) {
            packetSize = llread(packet); 
            
            if (packetSize < 0) {
                printf("Error reading packet.\n"); 
                fclose(file); 
                return; 
            }

            unsigned char C = packet[0]; 

            if (C == 2) { // DATA PACKET 
                int dataSize = (packet[2] << 8) | packet[3]; 
                fwrite(&packet[4], 1, dataSize, file); 
            }

            else if (C == 3) {
                pritnf("END packet received.\n"); 
                break; 
            } else {
                printf("unidentified packet.\n"); 
            }
        }
        fclose(filename); 
        printf("Success"); 
    }
    

}
