#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_LOG_SIZE 16384

int initLog(size_t logbufsize);
void stringLog(char *string);
char* getLog();
void exitLog();
void clearLog();