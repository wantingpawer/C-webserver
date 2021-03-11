#define DEFAULT_PORT "80"
#define MAX_THREADS 4
#define DEFAULT_BUFLEN 512

#include <stdio.h>
#include <winsock2.h>
#include <Ws2def.h>
#include <ws2tcpip.h>
#include <direct.h>
#include <pthread.h>

#include "handleGet.h"
#include "handleHead.h"
#include "startUp.h"
