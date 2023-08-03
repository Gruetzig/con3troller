#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/time.h>
#include <fcntl.h>

int connectToServer(char* ip, int port);
int exitSocket();
int initSocket();
int socketSetup();
int connectToServer(char* ip, int port);
bool getSockState();
int attemptHandshake(u64 timeomsec, char* resp);
ssize_t sendData(void* buf, size_t bufsize, int flags);
void disconnectfromServer();
void socketUnsetup();
