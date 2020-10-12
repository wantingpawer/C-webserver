#include "common.h"

//If the code results a linker error, make sure ws2_32.lib has been included as a library to be linked

int main(int argc, char *argv[]){

    SOCKET listenSocket = webserverStartUp();
    SOCKET ClientSocket;

    while(1){
        if((ClientSocket = accept(listenSocket, NULL, NULL)) == INVALID_SOCKET){
            printf("Accept failed: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return -1;
        }else{
            printf("Accepted connection\n");
            int recvResult, sendResult;
            char recvbuf[DEFAULT_BUFLEN];

            recvResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if(recvResult > 0){
                printf("Received %d bytes\n", recvResult);
            }
            printf("%d %d\n", strncmp(recvbuf, "HEAD", 4), strncmp(recvbuf, "GET", 3));
            printf("%s\n", recvbuf);
            if(strncmp(recvbuf, "GET", 3) == 0){
                sendResult = handleGet(recvbuf, ClientSocket);
            }else if(strncmp(recvbuf, "HEAD", 4) == 0){
                sendResult = handleHead(recvbuf, ClientSocket);
            }

            if(sendResult == SOCKET_ERROR){
                printf("Send failed: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return -1;
            }
            printf("Sent %d bytes\n", sendResult);
        }
    }
    return 0;
}
