#include "common.h"

void handleGet(struct requestData rqData){
    //Get the requested path from the request
    strtok(rqData.recvbuf, " ");
    char* filepath = strtok(NULL, " ");

    //Get the data and the file size
    char* data = getFile(filepath);
    size_t size = getFileSize(filepath);
    if(strcmp(data, "") == 0 || size == 0){
            printf("Invalid path\n\n");
            return;
    }

    //Assign a buffer with the size of the file + headers
    char sendbuf[size + 100];
    sprintf(sendbuf, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html;\r\n"
                     "Content-Length: %I64d\r\n\r\n"
                     "%s\r\n\r\n", strlen(data), data);

    send(rqData.clientSocket, sendbuf, DEFAULT_BUFLEN, 0);
    printf("Sent:\n%s\n", sendbuf);
    free(data);
}

void handleHead(struct requestData rqData){
    //Get the requested path from the request
    strtok(rqData.recvbuf, " ");
    char* filepath = strtok(NULL, " ");
    size_t size = getFileSize(filepath);
    if(size == 0){
        printf("Invalid path\n\n");
        return;
    }

    //Create a buffer to hold the header
    char sendbuf[500];
    sprintf(sendbuf, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html;\r\n"
                     "Content-Length: %I64d\r\n\r\n",
                     size);

    send(rqData.clientSocket, sendbuf, DEFAULT_BUFLEN, 0);
    printf("Sent:\n%s\n", sendbuf);
}

char* getFile(char *path){
    //Get rid of the first part of the path
    for(int i = 1; i < strlen(path)+1; ++i) path[i-1] = path[i];

    //If the path is empty set it to index.html
    /*
    TODO: MAKE THE DEFAULT PATH CONFIGURABLE
    */
    if(strcmp(path, "") == 0) path = "index.html";
    if( access( path, F_OK ) == -1 ) return 0;
    FILE* fptr = fopen(path, "r");

    //Get file size
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    //Allocate enough memory to hold the file
    char* result = malloc(size);
    fscanf(fptr, "%s", result);
    return result;
}

size_t getFileSize(char *path){
    //Get rid of the first part of the path
    for(int i = 1; i < strlen(path)+1; ++i) path[i-1] = path[i];
    //If the path is empty replace it with "index.html"
    /*
    TODO: MAKE THE DEFAULT PATH CONFIGURABLE
    */
    if(strcmp(path, "") == 0) path = "index.html";
    if( access( path, F_OK ) == -1 ) return 0;
    //Get and return the file size
    FILE* fptr = fopen(path, "r");
    fseek(fptr, 0L, SEEK_END);
    return ftell(fptr);
}
