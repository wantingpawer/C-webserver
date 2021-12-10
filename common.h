#define DEFAULT_BUFLEN 2048
#define MAX_HEAD 128
#define CONFIG_FILE "config.txt"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2def.h>
#include <ws2tcpip.h>
#include <direct.h>
#include <pthread.h>

#include "handleResponse.h"
#include "startUp.h"
#include "global.h"
