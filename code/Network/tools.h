#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
 
#define MAXLINE 100
#define LISTENQ 1024
int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);