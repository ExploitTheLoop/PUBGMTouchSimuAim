//
// Created by PCAditiaID29 on 2/8/2021.
//

#ifndef RPATCHERDAEMON_SOCKET_H
#define RPATCHERDAEMON_SOCKET_H

#include "Include.h"

int sockfd;
sockaddr_un addr_server;
char socket_name[108];
bool created,connected;

bool Connect();
void Close();
int sendData(void *inData, size_t size);
int send(void* inData, size_t size);
int recvData(void *outData, size_t size);
size_t receive(void* outData);
bool isConnected();
bool Create();

bool Create() {
    created = (sockfd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0)) != -1;
    return created;
}

bool Connect() {
    memset(socket_name, 0, sizeof(socket_name));
    memcpy(&socket_name[0], "\0", 1);
    strcpy(&socket_name[1], SOCKET_NAME);

    memset(&addr_server, 0, sizeof(struct sockaddr_un));
    addr_server.sun_family = AF_UNIX; // Unix Domain instead of AF_INET IP domain
    strncpy(addr_server.sun_path, socket_name, sizeof(addr_server.sun_path) - 1); // 108 char max

    if (connect(sockfd, (sockaddr *) &addr_server, sizeof(addr_server)) == -1) {
        Close();
        return false;
    }

    connected = true;
    return true;
}

void Close() {
    connected = false;
    if (sockfd > 0)
        close(sockfd);
}

int sendData(void *inData, size_t size) {
    char *buffer = (char *) inData;
    int numSent = 0;

    while (size) {
        do {
            numSent = write(sockfd, buffer, size);
        } while (numSent == -1 && EINTR == errno);

        if (numSent <= 0) {
            Close();
            break;
        }

        size -= numSent;
        buffer += numSent;
    }
    return numSent;
}

int send(void *inData, size_t size) {
    uint32_t length = htonl(size);
    if(sendData(&length, sizeof(uint32_t)) <= 0){
        return 0;
    }
    return sendData(inData, size) > 0;
}

int recvData(void *outData, size_t size) {
    char *buffer = (char *) outData;
    int numReceived = 0;

    while (size) {
        do {
            numReceived = read(sockfd, buffer, size);
        } while (numReceived == -1 && EINTR == errno);

        if (numReceived <= 0) {
            Close();
            break;
        }

        size -= numReceived;
        buffer += numReceived;
    }
    return numReceived;
}

size_t receive(void *outData) {
    uint32_t length = 0;
    int code = recvData(&length, sizeof(uint32_t));
    if(code > 0){
        length = ntohl(length);
        recvData(outData, static_cast<size_t>(length));
    }
    return length;
}

bool isConnected() {
    return connected;
}


#endif //RPATCHERDAEMON_SOCKET_H
