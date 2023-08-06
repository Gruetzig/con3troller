#include "settings.h"

//settings constants
bool doHID = true;
bool doTouch = true;

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
    swkbdSetInitialText(&swkbd, inout);
    swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
    swkbdSetNumpadKeys(&swkbd, '.', 0);
    swkbdInputText(&swkbd, inout, 15);
}

bool allowHID() {
    return doHID;
}

bool allowTouch() {
    return doTouch;
}

void toggleHID() {
    if (doHID) {
        doHID = false;
    } else {
        doHID = true;
    }
}

void toggleTouch() {
    if (doTouch) {
        doTouch = false;
    } else {
        doTouch = true;
    }
}

