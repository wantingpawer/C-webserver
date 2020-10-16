#include "common.h"

//If the code results a linker error, make sure ws2_32.lib has been included as a library to be linked

int main(int argc, char *argv[]){

    //Starts up web server and assigns result to listen socket, as well as declares client socket
    SOCKET listenSocket = webserverStartUp();
    struct requestData rqData;

    while(1){
        //If accepting the connection fails
        if((rqData.clientSocket = accept(listenSocket, NULL, NULL)) == INVALID_SOCKET){
            //Print an error
            printf("Accept failed: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return -1;
        }else{
            //Otherwise print that the connection was accepted
            printf("Accepted connection\n");
            int recvResult;

            //Receive data in recvResult and print how much was received
            recvResult = recv(rqData.clientSocket, rqData.recvbuf, DEFAULT_BUFLEN, 0);
            if(recvResult > 0){
                printf("Received %d bytes\n", recvResult);
            }

            //Print what was received and handle specific requests
            printf("%s\n", rqData.recvbuf);
            if(strncmp(rqData.recvbuf, "GET", 3) == 0){
                handleGet(rqData);
            }else if(strncmp(rqData.recvbuf, "HEAD", 4) == 0){
                handleHead(rqData);
            }
        }
    }
    return 0;
}
