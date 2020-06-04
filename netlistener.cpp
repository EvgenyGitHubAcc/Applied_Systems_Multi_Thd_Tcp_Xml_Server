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
        int from_len = sizeof(from_sin);
        clientSocket = accept(servSocket, (struct sockaddr *)&from_sin, &from_len);

        *consWriter << "Connected socket: " + std::to_string(clientSocket);

        char recvBuf[BUF_SIZE];
        if(recv(clientSocket, recvBuf, BUF_SIZE, 0) == SOCKET_ERROR)
        {
            *consWriter << "Error receving data from " + std::to_string(clientSocket) + ". Error: " + std::to_string(WSAGetLastError());
            shutdown(clientSocket, 0);
            closesocket(clientSocket);
            WSACleanup();
            return;
        }

        netData = std::move(std::string(recvBuf));

        *consWriter << "Received from socket: " + std::to_string(clientSocket) + " " + netData;

        netData = mainThd->getResponse(netData);

        if(netData.size())
        {
            send(clientSocket, netData.data(), netData.size(), 0);
            *consWriter << "Send to socket: " + std::to_string(clientSocket) + " " + netData;
        }
        else
        {
            *consWriter << "Answer was not send, because no command is found in map";
        }

        shutdown(clientSocket, 0);
        closesocket(clientSocket);
        *consWriter << "Disconnected socket: " + std::to_string(clientSocket);
        clientSocket = SOCKET_ERROR;
    }

}

void NetListener::operator()(NetListener ** netPtr)
{
    *netPtr = this;
    if(initServer())
    {
        connHandler();
    }
}
