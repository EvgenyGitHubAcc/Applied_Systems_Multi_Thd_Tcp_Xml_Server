#include "netlistener.h"

NetListener::NetListener(ConsoleWriter * _consWriter, MainTHD * _mainThd) : consWriter(_consWriter),
    mainThd(_mainThd)
{

}

bool NetListener::initServer()
{
    WSADATA wsaData;
    if (WSAStartup(0x101, &wsaData) == SOCKET_ERROR)
    {
        *consWriter << "Error WSAStartup. Error: " + std::to_string(WSAGetLastError());
        WSACleanup();
        return false;
    }
    servSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(!servSocket)
    {
        *consWriter << "Error server socket creation. Error: " + std::to_string(WSAGetLastError());
        WSACleanup();
        return false;
    }
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = SRV_PORT;
    if(bind(servSocket, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        *consWriter << "Error bind server socket. Error: " + std::to_string(WSAGetLastError());
        WSACleanup();
        return false;
    }
    if(listen(servSocket, MAX_CLIENTS_COUNT) == SOCKET_ERROR)
    {
        *consWriter << "Error server socket listen. Error: " + std::to_string(WSAGetLastError());
        WSACleanup();
        return false;
    }
    return true;
}

void NetListener::connHandler()
{
    while(true)
    {
        SOCKET clientSocket = SOCKET_ERROR;

        int from_len = sizeof(from_sin);
        clientSocket = accept(servSocket, (struct sockaddr *)&from_sin, &from_len);

        if(clientSocket == SOCKET_ERROR)
        {
            continue;
        }

        clientSockDeque.push_back(clientSocket);
        *consWriter << "Connected socket: " + std::to_string(clientSocket);
    }
}

void NetListener::checkClientSock()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (std::deque<SOCKET>::iterator it = clientSockDeque.begin(); it != clientSockDeque.end();)
        {
            char recvBuf[BUF_SIZE] = "";
            send(*it, "ping", 5, 0);
            if (recvWithTimeOut(*it, recvBuf, BUF_SIZE, 0, TIMEOUT) == SOCKET_ERROR)
            {
                shutdown(*it, 0);
                closesocket(*it);
                *consWriter << "Disconnected socket: " + std::to_string(*it);
                it = clientSockDeque.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

int NetListener::recvWithTimeOut(SOCKET sock, char *Buffer, int Len, int flags, long timeout)
{
    fd_set ReadSet;
    int n;
    struct timeval Time;
    FD_ZERO(&ReadSet);
    FD_SET(sock, &ReadSet);
    Time.tv_sec = timeout;
    Time.tv_usec = 0;
    n = select(sock + 1, &ReadSet, NULL, NULL, &Time);
    if (n > 0)
    {
        return recv(sock, Buffer, Len, flags);
    }
    return(n);
}

void NetListener::operator()(NetListener ** netPtr)
{
    *netPtr = this;
    if(!initServer())
    {
        return;
    }
    connHandler();
}
