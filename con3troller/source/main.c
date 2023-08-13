#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "connect.h"
#include "ui.h"
#include "log.h"
#include "strdraw.h"
#include "bth.h"
#include "hotkeys.h"
#include "utils.h"

#define PORT 7078

typedef struct controls {
    u16 touchx;
    u16 touchy;
    u32 keydown;
    u32 keyup;
} Controls;

enum mainstates {
    STATE_INIT,
    STATE_INITIAL,
    STATE_BUTTONPRESSED_X,
    STATE_BUTTONPRESSED_B,
    STATE_LOAD_IP_FROM_CONFIG,
    STATE_SELECT_IP,
    STATE_SAVE_IP_TO_CONFIG,
    STATE_PRE_CONNECTION_SETUP,
    STATE_CONNECTION_SETUP,
    STATE_HANDSHAKE,
    STATE_RUNNING,
    STATE_SETTINGS_MENU
};

enum settingsstates {
    SETTINGS_GENERAL,
    SETTINGS_HOTKEYS,
    SETTINGS_MENU
};

enum settingshotkeysstates {
    HOTKEYS_MENU,
    HOTKEYS_SET_EXIT
};

int main() {
    //general variables
    int
    state = STATE_INIT,
    settingsstate = SETTINGS_MENU,
    hotkeystate = HOTKEYS_MENU,
    result;

    char
    ip[20] = "",
    currentIP[50] = "",
    actionText[200],
    errText[60],
    logbuf[100],
    pbuttons[1000],
    pbuttons2[900]; 
    DrawContext ctx;
    bool
    showLogs = false;
    //inits
    gfxInitDefault();
    cfguInit();
    if (initLog(DEFAULT_LOG_SIZE)) {
        printf("log init fail\n");
        goto exit;
    }
    stringLog("log init success");
    result = initSocket();
    switch(result) {
        case 0:
            stringLog("socket init success");
            break;
        case -1:
            stringLog("out of memory");
            break;
        case -2:
            stringLog("socInit fail");
            break;
    }
    if (result) {
        goto exit;
    }
    result = initUI();
    switch(result) {
        case 0:
            stringLog("UI init success");
            break;
        case -1:
            stringLog("C3D init fail");
            break;
        case -2:
            stringLog("C2D init fail");
            break;
    }
    if (result) {
        goto exit;
    }
    initContext(&ctx);
    initColors(&ctx);
    setDefaultHotkeys();
    sprintf(errText, "All good");
    Button buttonQuit, buttonHotkeys, buttonReturn, buttonGeneral, buttonSettingsReturn, buttonGeneralHID, buttonGeneralTouch, buttonHotkeyCExit, buttonsHotkeyCReset;
    newButton(&buttonGeneral, 0.0f, 0.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 60.0f, "General", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonHotkeys, 0.0f, 60.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 60.0f, "Hotkeys", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonReturn, 0.0f, 120.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 60.0f, "Return to main menu", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonQuit, 0.0f, 180.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 60.0f, "Quit", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonSettingsReturn, 0.0f, 180.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 60.0f, "Return to settings menu", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonGeneralHID, 0.0f, 0.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 30.0f, "Enable HID: ", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonGeneralTouch, 0.0f, 30.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 30.0f, "Enable Touch: ", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonHotkeyCExit, 0.0f, 30.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 30.0f, "Exit combo: ", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    newButton(&buttonsHotkeyCReset, 0.0f, 150.0f, 0.0f, SCREEN_WIDTH_BOTTOM, 30.0f, "Reset to default", 2.0f, ctx.clrWhite, ctx.clrBgDark, 0.5f);
    while (aptMainLoop()) {
        u32 _kDown, kDown, _kUp, kUp, _kHeld;
        kDown = 0, kUp = 0;
        touchPosition touch, _touch;
        touch.px = 0;
        touch.py = 0;
        Controls controls;
        hidScanInput();
        _kDown = hidKeysDown();
        _kUp = hidKeysUp();
        _kHeld = hidKeysHeld();
        hidTouchRead(&_touch);
        switch(state) {
            case STATE_INIT:
            sprintf(actionText, "Initializing...");
                result = socketSetup();
                switch(result) {
                    case 0:
                        stringLog("socket setup successful");
                        break;
                    case -1:
                        stringLog("UDP socket creation failed");
                        break;
                    case -2:
                        stringLog("fcntl failed to set socket to non-blocking.");
                        break;
                }
                if (result) {
                    goto exit;
                } else {
                    state = STATE_INITIAL;
                }
                break;
            case STATE_INITIAL:
                sprintf(currentIP, "IP: %s", ip);
                if (_kHeld & KEY_START) goto deinit;
                if (_kDown & KEY_X) {
                    state = STATE_BUTTONPRESSED_X;
                }
                if (_kDown & KEY_Y) {
                    state = STATE_SETTINGS_MENU;
                }
                if (_kDown & KEY_B) {
                    state = STATE_BUTTONPRESSED_B;
                }
                if (_kDown & KEY_A) state = STATE_PRE_CONNECTION_SETUP;
                break;
            case STATE_BUTTONPRESSED_B:
                while(_kHeld & KEY_B) { // B combos / IP
                    hidScanInput();
                    u32 _kHeld = hidKeysHeld();
                    if (_kHeld & KEY_DUP) { // B + Dpad Up: load from config
                        state = STATE_LOAD_IP_FROM_CONFIG;
                        break;
                    }
                    if (_kHeld & KEY_DDOWN) { // B + Dpad Down: open swkbd
                        state = STATE_SELECT_IP;
                        break;
                    }
                    if (_kHeld & KEY_DLEFT) { // B + Dpad Left: save to config
                        state = STATE_SAVE_IP_TO_CONFIG;
                        break;
                    }
                    if (!_kHeld) {
                        state = STATE_INITIAL;
                        break;
                    }
                }
                break;
            case STATE_BUTTONPRESSED_X:
                while(_kHeld & KEY_X) { // X combos / Log
                    hidScanInput();
                    u32 _kHeld = hidKeysHeld();
                    
                    if (_kHeld & KEY_DUP) { // X + Dpad Up: toggleLog
                        if (showLogs) {
                            showLogs = false;
                        } else {
                            showLogs = true;
                        }
                        break;
                    }
                    if (_kHeld & KEY_DDOWN) { // X + Dpad Down: clearLog
                        clearLog();
                        break;
                    }
                    if (!_kHeld) {
                        break;
                    }
                }
                state = STATE_INITIAL;
                break;
            case STATE_LOAD_IP_FROM_CONFIG:
                //parse ip
                result = parseIP(ip);
                switch(result) {
                    case 0:
                        snprintf(logbuf, 100, "Loaded IP %s from configuration", ip);
                        stringLog(logbuf);
                        break;
                    case -1:
                        stringLog("Failed to open con3troller/ip.txt!");
                        break;
                    case -2:
                        stringLog("Out of memory!");
                        break;
                }
                state = STATE_INITIAL;
                if (result) {
                    sprintf(errText, "parseIP fail: %d", result);
                } else {
                    sprintf(errText, "All good");
                }
                break;
            case STATE_SELECT_IP:
                selectIP(ip);
                sprintf(logbuf, "IP %s gotten from swkbd", ip);
                stringLog(logbuf);
                state = STATE_INITIAL;
                break;
            case STATE_SAVE_IP_TO_CONFIG:
                saveIP(ip);
                sprintf(logbuf, "IP %s saved to /con3troller/ip.txt", ip);
                stringLog(logbuf);
                state = STATE_INITIAL;
                break;
            case STATE_PRE_CONNECTION_SETUP:
                if (!strlen(ip)) {
                    sprintf(errText, "No IP set");
                    state = STATE_INITIAL;
                } else {
                    sprintf(errText, "All good");
                    state = STATE_CONNECTION_SETUP;
                }
                sprintf(logbuf, "Using IP %s", ip);
                stringLog(logbuf);
                break;
            case STATE_CONNECTION_SETUP:
                result = connectToServer(ip, PORT);
                switch(result) {
                    case 0:
                        stringLog("Connection setup successful");
                        break;
                    case -1:
                        stringLog("IP address is invalid!");
                }
                if (result) {
                    state = STATE_INITIAL;
                } else {
                    state = STATE_HANDSHAKE;
                }
                if (result) {
                    sprintf(errText, "connectToServer fail(invalid IP): %d", result);
                } else {
                    sprintf(errText, "All good");
                }
                break;
            case STATE_HANDSHAKE:
                char out[10];
                result = attemptHandshake(1000*5, out);
                switch(result) {
                    case 0:
                        stringLog("Hand shaken");
                        break;
                    case -1:
                        stringLog("Hand held to PC but no hand from PC, \nconnection timed out.");
                        break;
                }
                if (result) {
                    state = STATE_INITIAL;
                } else {
                    state = STATE_RUNNING;
                    stringLog("Doing, press START to exit");
                }
                if (result) {
                    sprintf(errText, "attemptHandshake(connection Timeout) fail: %d", result);
                } else {
                    sprintf(errText, "All good");
                }
                break;
            case STATE_RUNNING:
                if ((kDown != _kDown) || (touch.px != _touch.px || touch.py != _touch.py) || (_kUp != kUp)) {
                    if (allowTouch()) {
                        controls.keydown = _kDown;
                        controls.keyup = _kUp;
                    } else {
                        controls.keydown = 0;
                        controls.keyup = 0;
                    }
                    if (allowHID()) {
                        controls.touchx = _touch.px;
                        controls.touchy = _touch.py;
                    } else {
                        controls.touchx = 0;
                        controls.touchy = 0;
                    }
                    sendData(&controls, sizeof(Controls), 0);
                }
                if (_kDown & exithotkey) {
                    disconnectfromServer();
                    state = STATE_INITIAL;
                }
                break;
            case STATE_SETTINGS_MENU:
                switch(settingsstate) {
                    case SETTINGS_MENU:
                        if (isButtonPressed(&buttonGeneral)) {
                            settingsstate = SETTINGS_GENERAL;
                        }
                        if (isButtonPressed(&buttonHotkeys)) {
                            settingsstate = SETTINGS_HOTKEYS;
                        }
                        if (isButtonPressed(&buttonReturn)) {
                            state = STATE_INITIAL;
                        }
                        if (isButtonPressed(&buttonQuit)) {
                            goto deinit;
                        }
                        break;
                    case SETTINGS_GENERAL:
                        if (isButtonPressed(&buttonGeneralHID)) {
                            toggleHID();
                        }
                        if (isButtonPressed(&buttonGeneralTouch)) {
                            toggleTouch();
                        }
                        if (isButtonPressed(&buttonSettingsReturn)) {
                            settingsstate = SETTINGS_MENU;
                        }
                        break;
                    case SETTINGS_HOTKEYS:
                        switch(hotkeystate) {
                            case HOTKEYS_MENU:
                                if (isButtonPressed(&buttonsHotkeyCReset)) {
                                    setDefaultHotkeys();
                                }
                                if (isButtonPressed(&buttonSettingsReturn)) {
                                    settingsstate = SETTINGS_MENU;
                                }
                                if (isButtonPressed(&buttonHotkeyCExit)) {
                                    hotkeystate = HOTKEYS_SET_EXIT;
                                }
                                break;
                            case HOTKEYS_SET_EXIT:
                                isButtonPressed(&buttonHotkeyCExit);
                                u32 hotkey = listenAndSetHotkey();
                                getComboString(hotkey, pbuttons);
                                sprintf(pbuttons, "Exit combo: %s", pbuttons2);
                                changeButtonString(&buttonHotkeyCExit, pbuttons);
                                hotkeystate = HOTKEYS_MENU;
                                break;
                        }
                        break;  
                }
                break;



        }
        if (state == STATE_SETTINGS_MENU) {
            if (settingsstate == SETTINGS_GENERAL) {
                if (allowHID()) {
                    changeButtonString(&buttonGeneralHID, "Enable HID: ON");
                } else {
                    changeButtonString(&buttonGeneralHID, "Enable HID: OFF");
                }
                if (allowTouch()) {
                    changeButtonString(&buttonGeneralTouch, "Enable Touch: ON");
                } else {
                    changeButtonString(&buttonGeneralTouch, "Enable Touch: OFF");
                }
            }
        }

        switch(state) {
            case STATE_INIT:
                sprintf(actionText, "Initializing...");
                break;
            case STATE_INITIAL:
                sprintf(actionText, "Welcome to con3troller");
                break;
            case STATE_CONNECTION_SETUP:
                sprintf(actionText, "Setting up connection to %s...", ip);
                break;
            case STATE_LOAD_IP_FROM_CONFIG:
                sprintf(actionText, "Loading IP from file...");
                break;
            case STATE_PRE_CONNECTION_SETUP:
                sprintf(actionText, "Looking at the IP...");
            case STATE_SELECT_IP:
                sprintf(actionText, "Enter the desired IP address...");
                break;
            case STATE_HANDSHAKE:
                sprintf(actionText, "Attempting connection to %s...", ip);
                break;
            case STATE_RUNNING:
                sprintf(actionText, "Sending input to %s", ip);
                break;
            case STATE_SETTINGS_MENU:
                sprintf(actionText, "    Settings    ");
                break;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(ctx.top, ctx.clrBgDark);
        C2D_TargetClear(ctx.bottom, ctx.clrBlack);
        C2D_SceneBegin(ctx.bottom); //workaround to definitely get targetclear through
        C2D_SceneBegin(ctx.top);
        drawStringBoxXCentered(5, 0, 0.5f, ctx.clrWhite, ctx.clrBgBright, actionText, 1.0f);
        if (showLogs) {
            drawStringBoxAtPos(10.0f, 40.0f, 0.0f, 0.5f, ctx.clrWhite, ctx.clrBgBright, getLog(), 2.0f);
        }
        drawStringBoxXCentered(SCREEN_HEIGHT-(60.0f*0.5f)-3, 0, 0.5f, ctx.clrWhite, ctx.clrBgBright, errText, 1.0f);
        drawStringBoxXCentered(SCREEN_HEIGHT-(90.0f*0.5f)-5, 0, 0.5f, ctx.clrWhite, ctx.clrBgBright, currentIP, 1.0f);
        switch(state) {
            case STATE_INITIAL:
                if (_kDown & KEY_X || _kHeld & KEY_X) {
                    drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.45f, ctx.clrWhite, ctx.clrBgBright, "Down: Clear Log  Up: Hide/show Log", 1.0f);
                }
                else if (_kHeld & KEY_B || _kHeld & KEY_B) {
                    drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.41f, ctx.clrWhite, ctx.clrBgBright, "Down: Select IP  Up: Load IP from configuration  Left: Save IP to configuration", 1.0f);
                }
                else {
                    drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.45f, ctx.clrWhite, ctx.clrBgBright, "START: Exit  A: Begin  B: IP things  X: Configure IP  Y: Settings", 1.0f);
                }
                break; 
            case STATE_BUTTONPRESSED_B:
                drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.41f, ctx.clrWhite, ctx.clrBgBright, "Down: Select IP  Up: Load IP from configuration  Left: Save IP to configuration", 1.0f);
                break;
            case STATE_BUTTONPRESSED_X:
                drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.45f, ctx.clrWhite, ctx.clrBgBright, "Down: Clear Log  Up: Hide/show Log", 1.0f);
                break;
            case STATE_RUNNING:
                drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.45f, ctx.clrWhite, ctx.clrBgBright, "START: Stop", 1.0f);
                break;
            case STATE_SETTINGS_MENU:
                switch(settingsstate) {
                    case SETTINGS_MENU:
                        C2D_SceneBegin(ctx.bottom);
                        drawButton(&buttonGeneral);
                        drawButton(&buttonHotkeys);
                        drawButton(&buttonReturn);
                        drawButton(&buttonQuit);
                        break;
                    case SETTINGS_GENERAL:
                        C2D_SceneBegin(ctx.bottom);
                        drawButton(&buttonGeneralHID);
                        drawButton(&buttonGeneralTouch);
                        drawButton(&buttonSettingsReturn);
                        break;
                    case SETTINGS_HOTKEYS:
                        C2D_SceneBegin(ctx.bottom);
                        drawButton(&buttonSettingsReturn);
                        drawButton(&buttonsHotkeyCReset);
                        drawButton(&buttonHotkeyCExit);
                        break;
                }
                
        }
        C3D_FrameEnd(0);
        
        kDown = _kDown;
        kUp = _kUp;
        touch = _touch;
    }
exit:
    consoleInit(GFX_TOP, GFX_LEFT);
    printf("%s\nFail menu, press START to exit", getLog());
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) goto deinit;
    }
deinit:
    if (getSockState()) {
        socketUnsetup();
    }
    destroyButton(&buttonGeneral);
    destroyButton(&buttonHotkeys);
    destroyButton(&buttonReturn);
    destroyButton(&buttonQuit);
    exitSocket();
    exitLog();
    exitUI();
    cfguExit();
    gfxExit();
    return 0;
}