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
}

void NetworkManager::runServer()
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
            if (sockets[i].send == SEND)
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
                    case SEND:
                        sendMessage(i);
                        break;
                }
            }
        }
    }

    cout << "Server: Closing Connection.\n";
    closesocket(listenSocket);
    WSACleanup();
}

bool NetworkManager::addSocket(SOCKET id, int what)
{
    for (int i = 0; i < MAX_SOCKETS; i++)
    {
        if (sockets[i].recv == EMPTY)
        {
            sockets[i].id = id;
            sockets[i].recv = what;
            sockets[i].send = IDLE;
            sockets[i].len = 0;
            socketsCount++;
            return (true);
        }
    }

    unsigned long flag=1;
    if (ioctlsocket(id, FIONBIO, &flag) != 0)
    {
        cout<<"Server: Error at ioctlsocket(): "<<WSAGetLastError()<<endl;
    }

    return (false);
}

void NetworkManager::removeSocket(int index)
{
    sockets[index].recv = EMPTY;
    sockets[index].send = EMPTY;
    socketsCount--;
}

void NetworkManager::acceptConnection(int index)
{
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
    SOCKET msgSocket = sockets[index].id;

    int len = sockets[index].len;
    int bytesRecv = recv(msgSocket, &sockets[index].buffer[len], sizeof(sockets[index].buffer) - len, 0);

    if (SOCKET_ERROR == bytesRecv)
    {
        cout << "Server: Error at recv(): " << WSAGetLastError() << endl;
        closesocket(msgSocket);
        removeSocket(index);
        return;
    }
    if (bytesRecv == 0)
    {
        closesocket(msgSocket);
        removeSocket(index);
        return;
    }
    else
    {
        sockets[index].buffer[len + bytesRecv] = '\0'; //add the null-terminating to make it a string
        cout<<"Server: Recieved: "<<bytesRecv<<" bytes of \""<<&sockets[index].buffer[len]<<"\" message.\n";

        sockets[index].len += bytesRecv + 1;    // "+ 1" to separate each request by '\0'
        loadMesseage(index);
    }
}

void NetworkManager::loadMesseage (int index) {
    if (sockets[index].len > 0)
    {
        sockets[index].send  = RESPONES;
        sockets[index].request.insert(sockets[index].buffer);
        int requestSize = strlen(sockets[index].buffer) + 1;
        memcpy(sockets[index].buffer, sockets[index].buffer + requestSize, sockets[index].len - requestSize);
    }
    else sockets[index].send  = IDLE;
}

void NetworkManager::prepareResponse(int index)
{

}

void NetworkManager::sendMessage(int index)
{
    int bytesSent = 0;
    char sendBuff[255];

    SOCKET msgSocket = sockets[index].id;

    bytesSent = send(msgSocket, sendBuff, (int)strlen(sendBuff), 0);
    if (SOCKET_ERROR == bytesSent)
    {
        cout << "Server: Error at send(): " << WSAGetLastError() << endl;
        return;
    }

    cout<<"Server: Sent: "<<bytesSent<<"\\"<<strlen(sendBuff)<<" bytes of \""<<sendBuff<<"\" message.\n";

    loadMesseage(index);
}
