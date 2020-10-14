#ifndef _HANDLEHEAD_H_
#define _HANDLEHEAD_H_
#include "common.h"

int handleHead(char *recvbuf, SOCKET ClientSocket);
size_t getFileSize(char *path);

#endif // _HANDLEHEAD_H_
