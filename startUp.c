#include "common.h"

int g_max_thread;
char g_port[DEFAULT_BUFLEN];
char g_404responsefile[DEFAULT_BUFLEN];
char g_root[DEFAULT_BUFLEN];

SOCKET webserverStartUp(){
    WSADATA wsa;
    SOCKET listenSocket;

    //Starts up the winsock in version 2.2 and checks for any errors
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        printf("Error: %d\n", WSAGetLastError());
        exit(-1);
    }
    printf("Initialised\n");

    //Load configuration options from the config file (defined in the preprocessor variable CONFIG_FILE)
    if(loadConfigs() == 0) printf("Configs loaded from %s\n", CONFIG_FILE);
    else{
        printf("Error loading configs from %s\n", CONFIG_FILE);
        exit(-1);
    }

    //Creates the address info structures to be used in getaddrinfo();
    struct addrinfo *result = NULL, hints;

    //Zeros out the memory in hints and defines it's values
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, g_port, &hints, &result) != 0){
        printf("getaddrinfo failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(-1);
    }

    //Creates the socket to listen with and checks for any errors
    if((listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET){
        printf("Couldn't create socket: %d\n", WSAGetLastError());
        exit(-1);
    }
    printf("Socket created\n");

    if((bind(listenSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR){
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(-1);
    }

    freeaddrinfo(result);
    printf("Socket bound\n");

    if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(-1);
    }

    printf("Listening on socket\n");

    return listenSocket;
}

int loadConfigs(){
    //Open up the config file for reading from
    FILE* fptr = fopen(CONFIG_FILE, "r");

    //Get the size of the config file
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    char temp[size];
    //Until the end of the file is reached, load all the config options into their corresponding global variables
    while(ftell(fptr) < size - 1){
        fgets(temp, size, fptr);
        char* setting = strtok(temp, "=");
        char* value = strtok(strtok(NULL, "="), "\n"); //The 2nd strtok gets rid of the "\n", this was the easiest solution I could think of

        //TODO: find a better solution for this
        if(strcmp("port", setting) == 0) strcpy(g_port, value);
        else if(strcmp("max_thread", setting) == 0) g_max_thread = atoi(value);
        else if(strcmp("404responsefile", setting) == 0) strcpy(g_404responsefile, value);
        else if(strcmp("root", setting) == 0) strcpy(g_root, value);
    }
    fclose(fptr);
    return 0;
}
