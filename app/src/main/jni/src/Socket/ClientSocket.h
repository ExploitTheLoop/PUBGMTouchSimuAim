//
// Created by PCAditiaID29 on 3/1/2021.
//

#ifndef RCHEATS_V2_CLIENTSOCKET_H
#define RCHEATS_V2_CLIENTSOCKET_H

#include "Include.h"

class ClientSocket {
public:
    int listenfd, acceptfd;
    bool isCreated;
    sockaddr_un addr_server;
    char socket_name[108];
    bool Create();
    bool Bind();
    bool Listen();
    bool Accept();
    void Close();
    int sendData(void *inData, size_t size);
    int send(void* inData, size_t size);
    int recvData(void *outData, size_t size);
    size_t receive(void* outData);

};


#endif //RCHEATS_V2_CLIENTSOCKET_H
