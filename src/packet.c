#include "../headers/packet.h"

long int getFileSize(FILE *file) {
    

    if (fseek(file, 0L, SEEK_END)) {
        printf("Pointer error.\n");
        fclose(file);
        return -1; 
    }

    long int res = ftell(file); 
    
    if (res == -1L) {
        printf("ftell error.\n");
        fclose(file);
        return -1;
    }

    fclose(file);
     
    return res;  
}

int buildControlPacket(unsigned char *packet, const char *filename, long int fileSize, int type) {
    // type pode ser 1 (start) ou 3 (end) 
    if (type != 1 && type != 3) {
        printf("type value is invalid.\n"); 
        return -1; 
    }

    if (filename == NULL) {
        printf("filename is a null pointer.\n"); 
        return -1; 
    }
    
    int ind = 0; 

    packet[ind++] = type;
    packet[ind++] = 0; //size 
    packet[ind++] = sizeof(fileSize); 
    memcpy(&packet[ind], &fileSize, sizeof(fileSize)); 
    ind += sizeof(fileSize); 

    packet[ind++] = 1;
    packet[ind++] = strlen(filename); 
    memcpy(&packet[ind], filename, strlen(filename)); 
    ind += strlen(filename); 


    printf("Control packet built (%d bytes): type=%d, size=%ld, name=%s\n",
       ind, type, fileSize, filename);

    return ind; 
}

int buildDataPacket(unsigned char *packet, unsigned char *data, int dataSize, unsigned char seqNum) {
    
    if (packet == NULL || data == NULL) {
        printf("Null pointer passed to buildDataPacket.\n");
        return -1;
    }

    if (dataSize > MAX_PAYLOAD_SIZE) {
        printf("Payload too large (max = %d bytes).\n", MAX_PAYLOAD_SIZE);
        return -1;
    }   

    int ind = 0; 

    packet[ind++] = 2; 
    packet[ind++] = seqNum;
    packet[ind++] = (dataSize >> 8) & 0XFF; 
    packet[ind++] = dataSize & 0xFF; 

    memcpy(&packet[ind], data, dataSize);
    ind += dataSize;  

    return ind; 
}


int readControlPacket(unsigned char *packet, long int *fileSize, int packetSize, char *filename) {
    
    if (filename == NULL || packet == NULL || fileSize == NULL ) {printf("arguments passed are null pointer.\n");return -1;}
    int i = 1;
    while (i < packetSize) {
        unsigned char T = packet[i++]; // t1 t2
        unsigned char L = packet[i++]; // l1 l2 

        if (T == 0) {
            memcpy(fileSize, &packet[i], L); 
        } else if (T == 1) {
            memcpy(fileSize, &packet[i], L); 
            filename[L] = '\0'; 
        }
        i += L; 
    }

    return 1; 
}