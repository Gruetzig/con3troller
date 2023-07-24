#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>

#define PORT 7078

typedef struct controls {
    u16 touchx;
    u16 touchy;
    u32 keybits;
} Controls;

int initSocket() {
    Result ret=0;
    u32 *soc_sharedmem, soc_sharedmem_size = 0x100000;

    soc_sharedmem = memalign(0x1000, soc_sharedmem_size);
    if(soc_sharedmem==NULL) {
        printf("Failed to allocate SOC sharedmem.\n");
        return -1;
    }
    else {
    ret = socInit(soc_sharedmem, soc_sharedmem_size);
    }
    if(R_FAILED(ret)) {
        printf("socInit failed: 0x%08x.\n", (unsigned int)ret);
        return -1;
    }
    return 0;
}

int exitSocket() {
    if (R_FAILED(socExit())) return -1;
    return 0;
}

int main() {
    //vars
    struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
    bool failed = false;

    //inits
    gfxInitDefault();
    consoleInit(GFX_TOP, GFX_LEFT);
    initSocket();
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
        failed = true;
        goto fail;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        failed = true;
        goto fail;
    }
fail:   if (failed) {
            printf("connection failed :( \n");
            goto exit;
        }
    printf("attempting connection...\n");
    sendto(sockfd, "Hey", 4, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buf[10];
    memset(buf, 0, 10);
    recvfrom(sockfd, buf, 10, 0, NULL, NULL);
    if (!(strcmp("Smosh", buf))) {
        printf("connected to %s", ip);
    } else {
        printf("%s", buf);
        goto fail;
    }
    free(ip);
   u32 _kDown, kDown;
    kDown = 0;
    touchPosition touch, _touch;
    touch.px = 0;
    touch.py = 0;
    Controls controls;
    while (aptMainLoop()) {
        hidScanInput();
        _kDown = hidKeysDown();
        hidTouchRead(&_touch);
        
        if ((kDown != _kDown) || (touch.px != _touch.px || touch.py != _touch.py)) {
            controls.keybits = _kDown;
            controls.touchx = _touch.px;
            controls.touchy = _touch.py;
            sendto(sockfd, &controls, sizeof(Controls), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        }
        kDown = _kDown;
        touch = _touch;
        if (kDown & KEY_START) goto deinit;
    }
exit:
    while (aptMainLoop()) {
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) goto deinit;
    }
deinit:
    close(sockfd);
    gfxExit();
    exitSocket();
}