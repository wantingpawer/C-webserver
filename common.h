#define DEFAULT_BUFLEN 2048
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

//Global variables - treated as constants outside of startup.c
extern int g_max_thread;
extern char g_port[DEFAULT_BUFLEN];
extern char g_404responsefile[DEFAULT_BUFLEN];
extern char g_root[DEFAULT_BUFLEN];

//These variables link to the whitelist, which is a custom made singly linked list purpose built for this project
extern bool g_usingwhitelist;
extern struct whitelistList *g_whitelist;
extern struct whitelistList *g_whitelistend;
