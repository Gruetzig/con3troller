#include <3ds.h>

u32 exithotkey;

void setDefaultHotkeys();
u32 listenAndSetHotkey();

void setDefaultHotkeys() {
    exithotkey = (KEY_START);
}

u32 listenAndSetHotkey() {
    u32 kDown;
    u32 keydowns = 0;
    hidScanInput();
    kDown = hidKeysHeld();
    while (kDown) {
        hidScanInput();
        kDown = hidKeysHeld();  
        keydowns = kDown;
        char buf[10];
        snprintf(buf, 10, "%ld", keydowns);
        stringLog(buf);
    }
    return keydowns;
}