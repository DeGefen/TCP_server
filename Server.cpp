#include "Config.h"
#include "NetworkManager.h"
#include "RequestsManager.h"

int main()
{
    WSAData wsaData;

    if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
    {
        cout<<"Time Server: Error at WSAStartup()\n";
        return 0;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == listenSocket)
    {
        cout<<"Time Server: Error at socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    }

    sockaddr_in serverService;
    serverService.sin_family = AF_INET;
    serverService.sin_addr.s_addr = INADDR_ANY;
    serverService.sin_port = htons(TIME_PORT);

    if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR *) &serverService, sizeof(serverService)))
    {
        cout<<"Time Server: Error at bind(): "<<WSAGetLastError()<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

    if (SOCKET_ERROR == listen(listenSocket, 5))
    {
        cout << "Time Server: Error at listen(): " << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

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
            cout <<"Time Server: Error at select(): " << WSAGetLastError() << endl;
            WSACleanup();
            return 0;
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

    // Closing connections and Winsock.
    cout << "Time Server: Closing Connection.\n";
    closesocket(listenSocket);
    WSACleanup();
}