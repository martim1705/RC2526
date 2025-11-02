#include "../headers/applicationLayer.h"




int sendFile(FILE* file) {
    printf("Entered sendFile.\n"); 
    if (file == NULL) {
        printf("Null pointer given.\n"); 
        return -1; 
    }
    unsigned char data[MAX_PAYLOAD_SIZE]; 
    unsigned char packet[MAX_PAYLOAD_SIZE + 4]; 
    unsigned char seqNum = 0; 
    size_t bytesRead; 
    //printf("Before while loop.\n"); 
    while ((bytesRead = fread(data, 1, MAX_PAYLOAD_SIZE, file)) > 0) {
        //printf("Entered loop\n"); 
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
        
        const char * Txfilename = filename; 
        FILE *file = fopen(Txfilename, "rb");

        if (file == NULL) {
            perror("Error opening file");
            return;
        }
        
        long int fileSize = getFileSize(file); // returns the size of filename 
        
        if (fileSize < 0) {
            return; 
        }    
        unsigned char Cpacket[MAX_PAYLOAD_SIZE];
        int CpacketSize = buildControlPacket(Cpacket, Txfilename, fileSize, 1); // returns the size of the START control packet 
        
        printf("\nSTART PACKET SENT (hex): ");
        for (int i = 0; i < CpacketSize; i++)
            printf("%02X ", Cpacket[i]);
        printf("\n");
        
        if ( CpacketSize < 0) {
            printf("Could not contruct START Control packet.\n"); 
            return; 
        }
         
        if (llwrite(Cpacket, CpacketSize) < 0) { // sends the START packet 
            printf("START packet was not sent.\n"); 
            return; 
        }
         
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }
        
        if (sendFile(file) < 0) {
            printf("Could not send file.\n"); 
            fclose(file); 
            return; 
        }
        
        CpacketSize = buildControlPacket(Cpacket, Txfilename, fileSize,3); 
        
        if ( CpacketSize < 0) {
            printf("Could not contruct END Control packet.\n"); 
            return; 
        }

        llwrite(Cpacket, CpacketSize); 

        fclose(file); 
    } else { // receiver 
        const char *Rxfilename = filename; 
        unsigned char packet[MAX_PAYLOAD_SIZE + 4]; 
        int packetSize; 
        char txfilename[256];
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


        printf("START PACKET RECEIVED (%d bytes): ", packetSize);
for (int k = 0; k < packetSize; k++) {
    printf("%02X ", packet[k]);
}
printf("\n");


        if (readControlPacket(packet, &fileSize, packetSize, txfilename) < 0) {
            printf("Could not read START packet.\n"); 
            return; 
        }

        FILE *file = fopen(Rxfilename, "wb");
        if (file == NULL) {
            printf("Could not create file.\n");
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
                printf("END packet received.\n"); 
                break; 
            } else {
                printf("Unidentified packet.\n"); 
            }
        }
        fclose(file); 
        printf("Success\n"); 
    }
    
    llclose(parameters); 
}
