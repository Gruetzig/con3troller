#include "log.h"

char *lb; //log buffer
size_t lbsize = 0; //size of log buffer
char dbuf[] = "";
int sus = 0;

int initLog(size_t logbufsize) {
    lb = malloc(logbufsize);
    memset(lb, 0, logbufsize);
    if (lb == NULL) {
        return -1; //no memory
    }
    lbsize = logbufsize;
    return 0;
}

void stringLog(char *string) {
    if (sus) {
        strcat(lb, "\n");
    } else {
        sus = 1;
    }
    strcat(lb, string);
}

char* getLog() {
    if (!lbsize) {
        return dbuf;
    }
    return lb;
}

void exitLog() {
    free(lb);
    lbsize = 0;
}