#define DEFAULT_BUFLEN 2048
#define CONFIG_FILE "config.txt"

//
extern int g_max_thread;
extern char g_port[DEFAULT_BUFLEN];
extern char g_404responsefile[DEFAULT_BUFLEN];
extern char g_root[DEFAULT_BUFLEN];

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2def.h>
#include <ws2tcpip.h>
#include <direct.h>
#include <pthread.h>

#include "handleResponse.h"
#include "startUp.h"
