#ifndef _STARTUP_H_
#define _STARTUP_H_
#include "common.h"

SOCKET webserverStartUp();
SOCKET backendSocket();

int loadConfigs();
void loadWhitelist(char* file);

struct requestData{
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET clientSocket;
};

struct whitelistList{
    char url[2048];
    struct whitelistList *next;
};

#endif // _STARTUP_H_
