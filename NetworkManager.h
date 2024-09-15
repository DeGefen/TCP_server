#pragma once
#include "Config.h"
#include "HTTP_Response.h"

class NetworkManager {
public:
    NetworkManager() { initializeServer();}
    void run();

private:
    struct SocketState
    {
        SOCKET id;			// Socket handle
        int	recv;			// Receiving?
        int	send;			// Sending?
        time_t last_request;
        HTTP_Response *response;
        char buffer[BUFFER_SIZE];
        int len;
    };

    static constexpr int EMPTY = 0;
    static constexpr int LISTEN  = 1;
    static constexpr int RECEIVE = 2;
    static constexpr int IDLE = 3;
    static constexpr int PREPARE = 4;
    static constexpr int SEND = 5;
    static constexpr int TIMEOUT = 2*60;


    void initializeServer();
    bool addSocket(SOCKET id, int what);
    void removeSocket(int index);
    void acceptConnection(int index);
    void receiveMessage(int index);
    void prepareResponse(int index);
    void sendMessage(int index);

    WSAData wsaData;
    sockaddr_in serverService;
    SOCKET listenSocket;

    struct SocketState sockets[MAX_SOCKETS]={0};
    int socketsCount = 0;
};
