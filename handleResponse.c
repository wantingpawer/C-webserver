#include "common.h"

int handleGet(char *recvbuf, SOCKET ClientSocket){
    strtok(recvbuf, " ");
    char* filepath = strtok(NULL, " ");
    char* data = getFile(filepath);
    char sendbuf[5000];
    sprintf(sendbuf, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html;\r\n"
                     "Content-Length: %I64d\r\n\r\n"
                     "%s\r\n\r\n", strlen(data), data);
    int sendResult = send(ClientSocket, sendbuf, DEFAULT_BUFLEN, 0);
    printf("Sent:\n%s\n", sendbuf);
    free(data);
    return sendResult;
}

int handleHead(char *recvbuf, SOCKET ClientSocket){
    char sendbuf[] = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html;\r\n"
                     "Content-Length: 13\r\n\r\n";
    int sendResult = send(ClientSocket, sendbuf, DEFAULT_BUFLEN, 0);
    printf("Sent:\n%s\n", sendbuf);
    return sendResult;
}

char* getFile(char *path){
    for(int i = 1; i < strlen(path)+1; ++i) path[i-1] = path[i];
    if(strcmp(path, "") == 0) path = "index.html";
    FILE* fptr = fopen(path, "r");
    char* result = malloc(5000);
    fscanf(fptr, "%s", result);
    printf(result);
    return result;
}
