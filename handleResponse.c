#include "common.h"

char g_404responsefile[DEFAULT_BUFLEN];
char g_root[DEFAULT_BUFLEN];

void handleResponse(struct requestData rqData){
    //Get the requested path from the request
    char* tempRecvbuf = malloc(sizeof(rqData.recvbuf));
    strcpy(tempRecvbuf, rqData.recvbuf);

    //A temp variable is used here because strtok() modifies the original string passed into it
    strtok(tempRecvbuf, " "); char* filepath = strtok(NULL, " ");
    //char* type = determineContentType(filepath);

    /*
    TODO: MOVE MORE STUFF OUT OF METHOD SPECIFIC BRANCHES
    PERHAPS USE MALLOC/REALLOC FOR SENDBUF?
    */

    //Determine type of request and handle appropriately
    if(strncmp(rqData.recvbuf, "GET", 3) == 0){
        char* data = getFile(filepath);
        if(!data){
            printf("Invalid path\n\n");
            return404(rqData);
            free(data);
            pthread_exit(NULL);
        }
        char sendBuf[strlen(data) + 100];
            /*
            TODO: USE FUNCTION determineContentType() AND OTHERS TO DYNAMICALLY GENERATE HTTP HEADERS
            IN ORDER TO STOP HARD CODING THEM INTO THE PROGRAM
            */
        sprintf(sendBuf, "HTTP/1.1 200 OK\r\n"
                     //"Content-Type: text/html;\r\n"
                     "Content-Length: %I64d\r\n\r\n"
                     "%s\r\n\r\n", strlen(data), data);
        send(rqData.clientSocket, sendBuf, DEFAULT_BUFLEN, 0);
        printf("Sent:\n%s\n", sendBuf);
        free(data);
    }else if(strncmp(rqData.recvbuf, "HEAD", 4) == 0){
        size_t size = getFileSize(filepath);
        if(size == 0){
            printf("Invalid path\n\n");
            return404(rqData);
            pthread_exit(NULL);
        }

        //Create a buffer to hold the header
        char sendbuf[500];
        sprintf(sendbuf, "HTTP/1.1 200 OK\r\n"
                         //"Content-Type: text/html;\r\n"
                         "Content-Length: %I64d\r\n\r\n",
                         size);

        send(rqData.clientSocket, sendbuf, DEFAULT_BUFLEN, 0);
        printf("Sent:\n%s\n", sendbuf);
    }

    //Free all dynamically allocated memory and terminate the thread
    free(tempRecvbuf);
    //free(type);
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

    //Check if file is accessible, if not return nothing, if so open the file for reading
    if( access( path, F_OK ) == -1 ) return NULL;
    FILE* fptr = fopen(path, "r");

    //Get file size
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    //Allocate enough memory to hold the file
    char* result = malloc(size);
    char temp[size];

    //Copy the first line of the file to the result variable
    fgets(result, size, fptr);

    //Make sure ever line is read (can probably make this a little more efficient, works for now though)

    /*I don't know why you need to take 2 away from the size
    *Would make sense if you only had to take 1
    *I don't like this solution since I don't know how it works
    *But if you only take 1, it copies the last line twice and doesn't assign enough memory
    *So the program crashes when you try to free the memory*/
    while(strlen(result) < size - 2){
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

char* determineContentType(char* filepath){
    //Get the file extension
    char* tempFilepath = malloc(sizeof(*filepath));
    strcpy(tempFilepath, filepath);

    //A temp variable is used here because strtok() modifies the original string passed into it
    strtok(tempFilepath, "."); char* ext = strtok(NULL, ".");
    char* res = malloc(strlen(ext) + 5);
    strcpy(res, "text/");
    strcat(res, ext);

    printf("%s", res);
    free(tempFilepath);
    return res;
}
