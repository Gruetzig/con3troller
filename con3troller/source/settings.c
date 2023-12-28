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

int charCount(char* string, char character, size_t maxlen, bool ignoreNullTerminator) {
    int ret = 0;
    for (int i = 0;i < maxlen;i++) {
        if (string[i] == character) ret++;
        if (string[i] == '\0' && !ignoreNullTerminator) break;
    }
    return ret;
}

bool charPaired(const char* string, char character) {
    for (int i = 0;i<strlen(string)-1;i++) {
        if ((string[i] == character) &&  (character == string[i+1])) {
            return true;
        }
    }
    return false;
}
bool validIP(const char *ip) {
    char work[16]; //12 digits max, 3 dots, 1 null character
    strncpy(work, ip, 16);
    work[15] = '\0'; //ensure null termination
    if (strlen(work) < 7) return false;
    if (charCount(work, '.', 16, false) != 3) return false;
    if (charPaired(work, '.')) return false;
    return true;
}

void selectIP(char *inout) {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 15);
    if (inout[0] != '<') swkbdSetInitialText(&swkbd, inout); //if loop to prevent empty text showing up
    swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
    swkbdSetNumpadKeys(&swkbd, '.', 0);
    swkbdInputText(&swkbd, inout, 16);
    return;
}

void saveIP(char *in) {
    FS_Archive SDarchive;
    fsInit();
    FSUSER_OpenArchive(&SDarchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
    FSUSER_CreateDirectory(SDarchive, fsMakePath(PATH_ASCII, "/con3troller"), FS_ATTRIBUTE_DIRECTORY);
    FSUSER_CloseArchive(SDarchive);
    fsExit();
    FILE *ipf = fopen("sdmc:/con3troller/ip.txt", "w");
    fwrite(in, 1, strlen(in)+1, ipf);
    fclose(ipf);
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

