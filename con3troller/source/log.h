#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_LOG_SIZE 2048

int initLog(size_t logbufsize);
void stringLog(char *string);
char* getLog();
void exitLog();