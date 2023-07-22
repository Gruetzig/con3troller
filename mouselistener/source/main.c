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

void waitf(const char* string) {
    printf(string);
    while (1) {
        hidScanInput();
        u32 kdown = hidKeysDown();
        if (kdown & KEY_A) {
            break;
        }
    }
}

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
    struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
    bool failed = false;
    int ret = 0;
    gfxInitDefault();
    consoleInit(GFX_TOP, GFX_LEFT);
    initSocket();
    FILE *ipf = fopen("sdmc:/con3troller/ip.txt", "r");
    char *ip = malloc(20*sizeof(char));
    fread(ip, 1, 19, ipf);
    ip[19] = '\0';
    size_t iplen = strlen(ip);
    ip = realloc(ip, (iplen+1)*sizeof(char));
    ip[iplen] = '\0';
    fclose(ipf);
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
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
    ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        failed = true;
        goto fail;
    }
fail:    if (failed) {
            printf("connection failed :( \n");
            goto exit;
        } 
    
    printf("connection successful!!, to \n%s:%d \n", ip, PORT);
    free(ip);


     
while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        touchPosition touch, _touch;
        hidTouchRead(&_touch);
        touch.px = 0;
        touch.py = 0;
        
        if (touch.px != _touch.px || touch.py != _touch.py) {
            send(sockfd, &_touch.px, sizeof(u16), 0);
            send(sockfd, &_touch.py, sizeof(u16), 0);
        }
        touch = _touch;
        if (kDown & KEY_START) break;
    }
exit:
    close(sockfd);
    gfxExit();
    exitSocket();
}