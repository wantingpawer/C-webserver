#include "common.h"

char g_404responsefile[DEFAULT_BUFLEN];
char g_root[DEFAULT_BUFLEN];

void handleGet(struct requestData rqData){
    //Get the requested path from the request
    strtok(rqData.recvbuf, " ");
    char* filepath = strtok(NULL, " ");

    //Get the data and the file size
    char* data = getFile(filepath);
    if (!data){
        printf("Invalid path\n\n");
        return404(rqData);
        free(data);
        pthread_exit(NULL);
        return;
    }

    //Assign a buffer with the size of the file + headers
    char sendbuf[strlen(data) + 100];
    sprintf(sendbuf, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html;\r\n"
                     "Content-Length: %I64d\r\n\r\n"
                     "%s\r\n\r\n", strlen(data), data);

    send(rqData.clientSocket, sendbuf, DEFAULT_BUFLEN, 0);
    printf("Sent:\n%s\n", sendbuf);
    free(data);
    pthread_exit(NULL);
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
    pthread_exit(NULL);
}

void return404(struct requestData rqData){
    char* data = getFile(g_404responsefile);
    char sendbuf[getFileSize(g_404responsefile) + 100];
    if (!data){
        printf("WARNING: Please make a \"404.html\" file for users to receive upon getting a 404 error\n\n");
        sprintf(sendbuf, "HTTP/1.1 404 NOT FOUND\r\n"
                "Content-Length: 0\r\n\r\n"
                "\r\n\r\n");
    }else{
        sprintf(sendbuf, "HTTP/1.1 404 NOT FOUND\r\n"
                     "Content-Type: text/html;\r\n"
                     "Content-Length: %I64d\r\n\r\n"
                     "%s\r\n\r\n", getFileSize(g_404responsefile), data);
    }

    send(rqData.clientSocket, sendbuf, DEFAULT_BUFLEN, 0);
    printf("Sent:\n%s\n", sendbuf);
    free(data);
}

char* getFile(char *path){
    //Get rid of the first part of the path if it's a "/"
    if(strncmp(path, "/", 1) == 0) for(int i = 1; i < strlen(path)+1; ++i) path[i-1] = path[i];

    //If the path is empty set it to the root path defined in config.txt
    if(strcmp(path, "") == 0) path = g_root;

    if( access( path, F_OK ) == -1 ) return 0;
    FILE* fptr = fopen(path, "r");

    //Get file size
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    //Allocate enough memory to hold the file
    char* result = malloc(size);
    char temp[size];
    fgets(result, size, fptr);

    //Make sure ever line is read (can probably make this a little more efficient, works for now though)
    while(strlen(result) < size - 1){
        fgets(temp, size, fptr);
        strcat(result, temp);
    }
    return result;
}

size_t getFileSize(char *path){
    //Get rid of the first part of the path if it's a "/"
    if(strncmp(path, "/", 1) == 0) for(int i = 1; i < strlen(path)+1; ++i) path[i-1] = path[i];

    //If the path is empty set it to the root path defined in config.txt
    if(strcmp(path, "") == 0) path = g_root;
    if( access( path, F_OK ) == -1 ) return 0;
    //Get and return the file size
    FILE* fptr = fopen(path, "r");
    fseek(fptr, 0L, SEEK_END);
    return ftell(fptr);
}
