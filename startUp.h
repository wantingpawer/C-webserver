#ifndef _STARTUP_H_
#define _STARTUP_H_
#include "common.h"

SOCKET webserverStartUp();

int loadConfigs();
void loadWhitelist(char* file);

struct requestData{
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET clientSocket;
};

struct whitelistList{
    int id;
    char url[2048];
    struct whitelistList *next;
};

#endif // _STARTUP_H_
