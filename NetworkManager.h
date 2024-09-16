#pragma once
#include "Config.h"
#include "HTTP_Response.h"


// Manages network connections, socket operations, and HTTP responses.
class NetworkManager {
public:

    // Constructor initializes the server.
    NetworkManager() { initializeServer(); }

    // Main loop that runs the network manager.
    void run();

private:
    struct SocketState {
        SOCKET id;            // Socket handle
        int recv;             // Receiving status
        int send;             // Sending status
        time_t last_request;  // Time of the last request
        HTTP_Response *response;  // Associated HTTP response
        char buffer[BUFFER_SIZE]; // Buffer for incoming data
        int len;              // Length of received data
    };

    // Socket states
    static constexpr int EMPTY = 0;
    static constexpr int LISTEN = 1;
    static constexpr int RECEIVE = 2;
    static constexpr int IDLE = 3;
    static constexpr int PREPARE = 4;
    static constexpr int SEND = 5;
    static constexpr int TIMEOUT = 2 * 60;  // Timeout duration (2 minutes)


    // Initializes the server, setting up the listening socket.
    void initializeServer();

    // Adds a new socket to the manager with a specified state.
    bool addSocket(SOCKET id, int what);

    // Removes a socket from the manager by index.
    void removeSocket(int index);

    // Accepts a new connection on the listening socket.
    void acceptConnection(int index);

    // Receives data from a socket.
    void receiveMessage(int index);

    // Prepares an HTTP response based on the received request.
    void prepareResponse(int index);

    // Sends a message to a socket.
    void sendMessage(int index);

    WSAData wsaData;                 // Windows Sockets API data
    sockaddr_in serverService;       // Server socket address structure
    SOCKET listenSocket;             // Listening socket

    struct SocketState sockets[MAX_SOCKETS] = {0};
    int socketsCount = 0;
};
