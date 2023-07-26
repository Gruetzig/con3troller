#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/time.h>
#include <fcntl.h>

#define PORT 7078

typedef struct controls {
    u16 touchx;
    u16 touchy;
    u32 keydown;
    u32 keyup;
} Controls;

int initSocket() {
    Result ret=0;
    u32 *soc_sharedmem, soc_sharedmem_size = 0x100000;

    soc_sharedmem = memalign(0x1000, soc_sharedmem_size);
    if(soc_sharedmem==NULL) {
        printf("Out of memory? PLease restart your 3DS.\n");
        return -1;
    }
    else {
    ret = socInit(soc_sharedmem, soc_sharedmem_size);
    }
    if(R_FAILED(ret)) {
        printf("socket couldnt be created!! (socInit) 0x%08x.\n", (unsigned int)ret);
        return -1;
    }
    return 0;
}

int exitSocket() {
    if (R_FAILED(socExit())) return -1;
    return 0;
}

static bool set_socket_nonblocking(int sock)
{
	int flags = fcntl(sock, F_GETFL);
	if (flags == -1) return false;
	return fcntl(sock, F_SETFL, flags | O_NONBLOCK) == 0;
}

int main() {
    //vars
    struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
    bool sock = false;
    //inits
    gfxInitDefault();
    consoleInit(GFX_TOP, GFX_LEFT);
    if (initSocket()) {
        goto exit;
    }
    //parse ip
    FILE *ipf = fopen("sdmc:/con3troller/ip.txt", "r");
    char *ip = malloc(20*sizeof(char));
    fread(ip, 1, 19, ipf);
    ip[19] = '\0';
    size_t iplen = strlen(ip);
    ip = realloc(ip, (iplen+1)*sizeof(char));
    ip[iplen] = '\0';
    fclose(ipf);
    printf("loaded ip %s from configuration\n", ip);
    //socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        printf("Socket could not be created\n");
        goto exit;
    } else {
        sock = true;
    }
    if (!set_socket_nonblocking(sockfd)) {
        printf("Setting non-blocking failed\n");
        goto exit;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("IP address invalid\n");
        goto exit;
    }
    printf("attempting connection...\n");
    sendto(sockfd, "Hey", 4, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buf[10];
    memset(buf, 0, 10);
    u64 curtick = svcGetSystemTick();
    int recvfromres = -1;
    while (recvfromres < 0 && (curtick + (((u64)10)*1000*1000*1000)) > svcGetSystemTick()) {
        recvfromres = recvfrom(sockfd, buf, 10, 0, NULL, NULL);
    }
    if  (recvfromres < 0) {
        printf("Response not received, won't connect\n");
        goto exit;
    }
    if (!(strcmp("Smosh", buf))) {
        printf("connected to %s\n", ip);
    } else {
        printf("Invalid response received, won't connect, %s\n", buf);
        goto exit;
    }
    free(ip);
    u32 _kDown, kDown, _kUp, kUp;
    kDown = 0, kUp = 0;
    touchPosition touch, _touch;
    touch.px = 0;
    touch.py = 0;
    Controls controls;
    printf("Doing, press START to exit");
    while (aptMainLoop()) {
        hidScanInput();
        _kDown = hidKeysDown();
        _kUp = hidKeysUp();
        hidTouchRead(&_touch);
        
        if ((kDown != _kDown) || (touch.px != _touch.px || touch.py != _touch.py) || (_kUp == kUp)) {
            controls.keydown = _kDown;
            controls.keyup = _kUp;
            controls.touchx = _touch.px;
            controls.touchy = _touch.py;
            sendto(sockfd, &controls, sizeof(Controls), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        }
        kDown = _kDown;
        kUp = _kUp;
        touch = _touch;
        
        if (kDown & KEY_START) goto deinit;
        svcSleepThread(5*1000*1000);
    }
exit:
    printf("Fail menu, press START to exit");
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) goto deinit;
    }
deinit:
    if (sock) {
        close(sockfd);
    }
    exitSocket();
    gfxExit();
    return 0;
}