#ifndef _HANDLEGET_H_
#define _HANDLEGET_H_
#include "common.h"

int handleGet(char *recvbuf, SOCKET ClientSocket);
char* getFile(char *path);

#endif // _HANDLEGET_H_
