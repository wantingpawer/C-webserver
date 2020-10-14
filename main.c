#include "common.h"

//If the code results a linker error, make sure ws2_32.lib has been included as a library to be linked

int main(int argc, char *argv[]){

    //Starts up web server and assigns result to listen socket, as well as declares client socket
    SOCKET listenSocket = webserverStartUp();
    SOCKET ClientSocket;

    while(1){
        //If accepting the connection fails
        if((ClientSocket = accept(listenSocket, NULL, NULL)) == INVALID_SOCKET){
            //Print an error
            printf("Accept failed: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return -1;
        }else{
            //Otherwise print that the connection was accepted
            printf("Accepted connection\n");
            int recvResult, sendResult;
            char recvbuf[DEFAULT_BUFLEN];

            //Receive data in recvResult and print how much was received
            recvResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if(recvResult > 0){
                printf("Received %d bytes\n", recvResult);
            }

            //Print what was received and handle specific requests
            printf("%s\n", recvbuf);
            if(strncmp(recvbuf, "GET", 3) == 0){
                sendResult = handleGet(recvbuf, ClientSocket);
            }else if(strncmp(recvbuf, "HEAD", 4) == 0){
                sendResult = handleHead(recvbuf, ClientSocket);
            }

            //If there is an error, print it and quit
            if(sendResult == SOCKET_ERROR){
                printf("Send failed: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return -1;
            }

            //Print bytes sent
            printf("Sent %d bytes\n", sendResult);
        }
    }
    return 0;
}
