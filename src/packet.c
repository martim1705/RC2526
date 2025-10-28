long int openFile(const char *filename) {
    
    FILE *file = fopen(filename, "r"); 

    if (file == NULL) {
        printf("%s was not successfully opened.\n", filename);
        return -1;  
    }

    if (fseek(file, 0L, SEEK_END)) {
        printf("Pointer error.\n");
        fclose(file);
        return -1; 
    }

    long int res = ftell(file); 
    
    if (res == -1) {
        printf("ftell error.\n");
        fclose(file);
        return -1;
    }

    fclose(file);
     
    return res;  
}