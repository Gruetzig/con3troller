#include "connect.h"

int sockfd = -1;
struct sockaddr_in serv_addr;
bool sock = false;

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
        return -2;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
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

int socketSetup() {
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        return -1;
    } else {
        sock = true;
    }
    if (!set_socket_nonblocking(sockfd)) {
        return -2;
    }
    return 0;
}

int connectToServer(char* ip, int port) {

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        return -1;
    }
    return 0;
}

bool getSockState() {
    return sock;
}

int attemptHandshake(u64 timeomsec, char* resp) {
    sendto(sockfd, "Hey", 4, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buf[10];
    memset(buf, 0, 10);
    u64 curtick = svcGetSystemTick();
    int recvfromres = -1;
    while (recvfromres < 0 && (curtick + (timeomsec*1000*1000)) > svcGetSystemTick()) {
        recvfromres = recvfrom(sockfd, buf, 10, 0, NULL, NULL);
    }
    if  (recvfromres < 0) {
        return -1;
    }
    strncpy(resp, buf, 10);
    return 0;
}

ssize_t sendData(void* buf, size_t bufsize, int flags) {
    return sendto(sockfd, buf, bufsize, flags, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

void disconnectfromServer() {
    sendData("Byebyebye~~", 12, 0);
}

void socketUnsetup() {
    sock = false;
    close(sockfd);
}
