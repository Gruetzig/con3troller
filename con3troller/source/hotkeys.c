#include "hotkeys.h"
#include "log.h"
#include "utils.h"

u32 exithotkey = KEY_START;

void setDefaultHotkeys() {
    exithotkey = (KEY_START);
}

u32 listenAndSetHotkey() {
    u32 kDown = 0;
    u32 keydowns = 0;
    do {
        hidScanInput();
        kDown = ~(u32)(KEY_TOUCH) & hidKeysHeld();
    } while (!kDown);
    do {
        keydowns |= kDown;
        hidScanInput();
        kDown = hidKeysHeld();  
    } while (kDown);
    char log[100];
    char out[50];
    getComboString(keydowns, out);
    sprintf(log, "Keys: %ld, String: %s", keydowns, out);
    stringLog(log);
    return keydowns;
}