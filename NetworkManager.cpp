#include "NetworkManager.h"

void NetworkManager::initializeServer()
{
    if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
    {
        cout<<"Server: Error at WSAStartup()\n";
        return;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == listenSocket)
    {
        cout<<"Server: Error at socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return;
    }

    serverService.sin_family = AF_INET;
    serverService.sin_addr.s_addr = INADDR_ANY;
    serverService.sin_port = htons(PORT);

    if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR *) &serverService, sizeof(serverService)))
    {
        cout<<"Server: Error at bind(): "<<WSAGetLastError()<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    if (SOCKET_ERROR == listen(listenSocket, 5))
    {
        cout << "Server: Error at listen(): " << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    cout << "starting server" << endl;

    addSocket(listenSocket, LISTEN);
}

void NetworkManager::run()
{
    while (true)
    {
        fd_set waitRecv;
        FD_ZERO(&waitRecv);
        for (int i = 0; i < MAX_SOCKETS; i++)
        {
            if ((sockets[i].recv == LISTEN) || (sockets[i].recv == RECEIVE))
                FD_SET(sockets[i].id, &waitRecv);
        }

        fd_set waitSend;
        FD_ZERO(&waitSend);
        for (int i = 0; i < MAX_SOCKETS; i++)
        {
            if (sockets[i].send == SEND || sockets[i].send == PREPARE)
                FD_SET(sockets[i].id, &waitSend);
        }

        int nfd;
        nfd = select(0, &waitRecv, &waitSend, NULL, NULL);
        if (nfd == SOCKET_ERROR)
        {
            cout <<"Server: Error at select(): " << WSAGetLastError() << endl;
            WSACleanup();
            return;
        }

        for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
        {
            if (FD_ISSET(sockets[i].id, &waitRecv))
            {
                nfd--;
                switch (sockets[i].recv)
                {
                    case LISTEN:
                        acceptConnection(i);
                        break;

                    case RECEIVE:
                        receiveMessage(i);
                        break;
                }
            }
        }

        for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
        {
            if (FD_ISSET(sockets[i].id, &waitSend))
            {
                nfd--;
                switch (sockets[i].send)
                {
                    case PREPARE:
                        prepareResponse(i);
                        break;
                    case SEND:
                        sendMessage(i);
                        break;
                }
            }
        }

        for (int i = 1; i < MAX_SOCKETS; i++)
        {
            if (sockets[i].recv != EMPTY && time(nullptr) - sockets[i].last_request >= TIMEOUT)
                removeSocket(i);
        }
    }

    cout << "Server: Closing Connection.\n";
    closesocket(listenSocket);
    WSACleanup();
}

bool NetworkManager::addSocket(SOCKET id, int what)
{

    unsigned long flag = 1;
    if (ioctlsocket(id, FIONBIO, &flag) != 0)
    {
        cout << "Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
    }
    
    for (int i = 0; i < MAX_SOCKETS; i++)
    {
        if (sockets[i].recv == EMPTY)
        {
#ifdef _DEBUGING_SERVER
            cout << "Server: in \"addSocket\" - id " << id << endl;;
#endif // _DEBUGING_SERVER

            sockets[i].id = id;
            sockets[i].recv = what;
            sockets[i].send = IDLE;
            sockets[i].last_request = time(nullptr);
            sockets[i].response = nullptr;
            sockets[i].len = 0;
            socketsCount++;
            return (true);
        }
    }

    return (false);
}

void NetworkManager::removeSocket(int index)
{
#ifdef _DEBUGING_SERVER
    cout << "Server: in \"removeSocket\" - index " << index << endl;
#endif // _DEBUGING_SERVER

    sockets[index].recv = EMPTY;
    sockets[index].send = EMPTY;
    socketsCount--;
}

void NetworkManager::acceptConnection(int index)
{
#ifdef _DEBUGING_SERVER
    cout << "Server: in \"acceptConnection\" - index " << index << endl;;
#endif // _DEBUGING_SERVER

    SOCKET id = sockets[index].id;
    struct sockaddr_in from;		// Address of sending partner
    int fromLen = sizeof(from);

    SOCKET msgSocket = accept(id, (struct sockaddr *)&from, &fromLen);
    if (INVALID_SOCKET == msgSocket)
    {
        cout << "Server: Error at accept(): " << WSAGetLastError() << endl;
        return;
    }

    cout << "Server: Client "<<inet_ntoa(from.sin_addr)<<":"<<ntohs(from.sin_port)<<" is connected." << endl;


    if (addSocket(msgSocket, RECEIVE) == false)
    {
        cout<<"\t\tToo many connections, dropped!\n";
        closesocket(id);
    }
    return;
}

void NetworkManager::receiveMessage(int index)
{
#ifdef _DEBUGING_SERVER
    cout << "Server: in \"receiveMessage\" - index " << index << endl;;
#endif // _DEBUGING_SERVER

    SOCKET msgSocket = sockets[index].id;

    int len = sockets[index].len;
    int bytesRecv = recv(msgSocket, &sockets[index].buffer[len], sizeof(sockets[index].buffer) - len, 0);

    sockets[index].last_request = time(nullptr);

    if (SOCKET_ERROR == bytesRecv)
    {
        cout << "Server: Error at recv(): " << WSAGetLastError() << endl;
        closesocket(msgSocket);
        removeSocket(index);
        return;
    }
    if (bytesRecv == 0)
    {
#ifdef _DEBUGING_SERVER
        cout << "Server: index " << index << " finished" << endl;;
#endif // _DEBUGING_SERVER
        closesocket(msgSocket);
        removeSocket(index);
        return;
    }

    sockets[index].buffer[len + bytesRecv] = '\0'; //add the null-terminating to make it a string
    cout<<"Server: Recieved: "<<bytesRecv<<" bytes of \n\""<<&sockets[index].buffer[len]<<"\" message.\n";

    sockets[index].len += bytesRecv + 1;    // "+ 1" to separate each request by '\0'
    if (sockets[index].len > 0)
        sockets[index].send  = PREPARE;
    else
        sockets[index].send  = IDLE;
}

void NetworkManager::prepareResponse(int index)
{
#ifdef _DEBUGING_SERVER
    cout << "Server: in \"prepareResponse\" - index " << index << endl;;
#endif // _DEBUGING_SERVER

    HTTP_Request request;
    request.insert(sockets[index].buffer);
    sockets[index].response = new HTTP_Response(request);
    int requestSize = strlen(sockets[index].buffer) + 1;
    memcpy(sockets[index].buffer, sockets[index].buffer + requestSize, sockets[index].len - requestSize);
    sockets[index].len -= requestSize;
    sockets[index].send = SEND;
}

void NetworkManager::sendMessage(int index)
{
#ifdef _DEBUGING_SERVER
    cout << "Server: in \"sendMessage\" - index " << index << endl;;
#endif // _DEBUGING_SERVER

    int bytesSent = 0;
    char sendBuff[BUFFER_SIZE];

    SOCKET msgSocket = sockets[index].id;

    try {
        strcpy(sendBuff, sockets[index].response->extract().c_str());
    }
    catch (...) {
        
    }
    bytesSent = send(msgSocket, sendBuff, (int)strlen(sendBuff), 0);
    if (SOCKET_ERROR == bytesSent)
    {
        cout << "Server: Error at send(): " << WSAGetLastError() << endl;
        return;
    }

    cout<<"Server: Sent: "<<bytesSent<<"\\"<<strlen(sendBuff)<<" bytes of \""<<sendBuff<<"\" message.\n";
    
    delete sockets[index].response;
    if (sockets[index].len > 0)
        sockets[index].send  = PREPARE;
    else
        sockets[index].send  = IDLE;
}
