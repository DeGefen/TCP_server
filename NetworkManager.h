#pragma once
#include "Config.h"

class NetworkManager {
    bool addSocket(SOCKET id, int what);
    void removeSocket(int index);
    void acceptConnection(int index);
    void receiveMessage(int index);
    void prossesRequest(int index);
    void sendMessage(int index);

private:
    static struct SocketState sockets[MAX_SOCKETS]={0};
    int socketsCount = 0;
};
