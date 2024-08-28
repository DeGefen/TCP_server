#pragma once
#include "Config.h"
#include "HTTPRequest.h"

class NetworkManager {
public:
    NetworkManager() { void initializeServer();}
    void runServer();

private:
    struct SocketState
    {
        SOCKET id;			// Socket handle
        int	recv;			// Receiving?
        int	send;			// Sending?
        HTTPRequest request;	// Sending sub-type
        char buffer[128];
        int len;
    };

    static constexpr int TIME_PORT = 27015;
    static constexpr int MAX_SOCKETS = 60;
    static constexpr int EMPTY = 0;
    static constexpr int LISTEN  = 1;
    static constexpr int RECEIVE = 2;
    static constexpr int IDLE = 3;
    static constexpr int RESPONES = 4;
    static constexpr int SEND = 5;
    static constexpr int SEND_TIME = 1;
    static constexpr int SEND_SECONDS = 2;


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
