#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"


int parseIP(char* out) {
    FILE *ipf = fopen("sdmc:/con3troller/ip.txt", "r");
    if (ipf == NULL) {
        return -1;
    }
    char *ip = malloc(20 * sizeof(char));
    if (ip == NULL) {
        fclose(ipf);
        return -2;
    }
    fread(ip, 1, 19, ipf);
    ip[19] = '\0';
    size_t iplen = strlen(ip);
    ip = realloc(ip, (iplen + 1) * sizeof(char));
    if (ip == NULL) {
        fclose(ipf);
        return -2;
    }
    ip[iplen] = '\0';
    fclose(ipf);
    strcpy(out, ip);
    free(ip);
    return 0;
}

void selectIP(char *inout) {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 15);
    swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
    swkbdSetNumpadKeys(&swkbd, '.', 0);
    swkbdInputText(&swkbd, inout, sizeof(inout));
}