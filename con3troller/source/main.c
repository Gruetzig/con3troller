#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "connect.h"
#include "ui.h"
#include "log.h"
#include "strdraw.h"

#define PORT 7078

typedef struct controls {
    u16 touchx;
    u16 touchy;
    u32 keydown;
    u32 keyup;
} Controls;

enum states {
    STATE_INIT,
    STATE_INITIAL,
    STATE_LOAD_IP,
    STATE_SELECT_IP,
    STATE_CONNECTION_SETUP,
    STATE_HANDSHAKE,
    STATE_RUNNING
};

int main() {
    //vars
    int state = STATE_INIT;
    char ip[20];
    char ipmsgbuf[100];
    DrawContext ctx;
    int result;
    bool showLogs = false;
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
    while (aptMainLoop()) {
        u32 _kDown, kDown, _kUp, kUp;
        kDown = 0, kUp = 0;
        touchPosition touch, _touch;
        touch.px = 0;
        touch.py = 0;
        Controls controls;
        hidScanInput();
        _kDown = hidKeysDown();
        _kUp = hidKeysUp();
        hidTouchRead(&_touch);
        switch(state) {
            case STATE_INIT:
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
            case STATE_INITIAL:
                if (_kDown & KEY_START) goto deinit;
                if (_kDown & KEY_X) {
                    if (showLogs) {
                        showLogs = false;
                    } else {
                        showLogs = true;
                    }
                }
                if (_kDown & KEY_Y) state = STATE_SELECT_IP;
                if (_kDown & KEY_A) state = STATE_LOAD_IP;
                break;
            case STATE_LOAD_IP:
                //parse ip
                result = parseIP(ip);
                switch(result) {
                    case 0:
                        snprintf(ipmsgbuf, 100, "Loaded IP %s from configuration", ip);
                        stringLog(ipmsgbuf);
                        break;
                    case -1:
                        stringLog("Failed to open con3troller/ip.txt!");
                        break;
                    case -2:
                        stringLog("Out of memory!");
                        break;
                }
                if (result) {
                    state = STATE_INITIAL;
                } else {
                    state = STATE_CONNECTION_SETUP;
                }
                break;
            case STATE_SELECT_IP:
                selectIP(ip);
                state = STATE_INITIAL;
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
                break;
            case STATE_RUNNING:
                if ((kDown != _kDown) || (touch.px != _touch.px || touch.py != _touch.py) || (_kUp != kUp)) {
                    controls.keydown = _kDown;
                    controls.keyup = _kUp;
                    controls.touchx = _touch.px;
                    controls.touchy = _touch.py;
                    sendData(&controls, sizeof(Controls), 0);
                }
                if (_kDown & KEY_START) {
                    disconnectfromServer();
                    state = STATE_INITIAL;
                    break;
                }

        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(ctx.top, ctx.clrBgDark);
        C2D_SceneBegin(ctx.top);
        if (showLogs) {
            drawStringBoxAtPos(10.0f, 10.0f, 0.0f, 0.5f, ctx.clrWhite, ctx.clrBgBright, getLog(), 2.0f);
        }

        switch(state) {
            case STATE_INITIAL:
                drawStringBoxXCentered(SCREEN_HEIGHT-(30*0.5f)-1, 0, 0.5f, ctx.clrWhite, ctx.clrBgBright, "START: exit  A: begin  X: toggle logs  Y: change IP", 1.0f);
            
            default:
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
    exitSocket();
    exitLog();
    exitUI();
    cfguExit();
    gfxExit();
    return 0;
}