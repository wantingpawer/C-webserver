#ifndef _HANDLERESPONSE_H_
#define _HANDLERESPONSE_H_
#include "common.h"

void handleResponse(struct requestData rqData);

bool checkFileWhitelistPrescence(char* path);

char* getFile(char* path);
void return404(struct requestData rqData);
size_t getFileSize(char* path);

char* determineContentType(char* filepath);

#endif // _HANDLERESPONSE_H_
