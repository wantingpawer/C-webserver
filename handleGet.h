#ifndef _HANDLEGET_H_
#define _HANDLEGET_H_
#include "common.h"

void handleGet(struct requestData rqData);
char* getFile(char *path);
void return404(struct requestData rqData);
#endif // _HANDLEGET_H_
