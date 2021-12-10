#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//Global variables shared between threads
extern pthread_mutex_t mutex;
extern int currentThreads;

//Global variables - treated as constants outside of startup.c
extern int g_max_thread;
extern char g_port[DEFAULT_BUFLEN];
extern char g_404responsefile[DEFAULT_BUFLEN];
extern char g_root[DEFAULT_BUFLEN];
extern char g_backendport[DEFAULT_BUFLEN];
extern SOCKET g_backendsock;

extern bool g_usingposttable;
extern struct posttable *g_posttable;

//These variables link to the whitelist, which is a custom made singly linked list purpose built for this project
extern bool g_usingwhitelist;
extern struct whitelistList *g_whitelist;



#endif // _GLOBAL_H_
