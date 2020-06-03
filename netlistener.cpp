#include "netlistener.h"

NetListener::NetListener(ConsoleWriter * _consWriter) : consWriter(_consWriter)
{

}

void NetListener::initServer()
{
    servSocket = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = SRV_PORT;
    bind(servSocket, (struct sockaddr *)&sin, sizeof(sin));
    listen(servSocket, MAX_CLIENTS_COUNT);
}

void NetListener::connHandler()
{
    char recvBuf[BUF_SIZE];
    while(true)
    {
        int from_len = sizeof(from_sin);
        clientSocket = accept(servSocket, (struct sockaddr *)&from_sin, &from_len);
        if(clientSocket == INVALID_SOCKET)
        {
            *consWriter << "Error socket creation";
            return;
        }

        *consWriter << "Connected " + std::to_string(clientSocket);

        if(recv(clientSocket, recvBuf, BUF_SIZE, 0) == -1)
        {
            *consWriter << "Error receving data from " + std::to_string(clientSocket);
            shutdown(clientSocket, 0);
            closesocket(clientSocket);
            return;
        }

        netData = std::move(std::string(recvBuf));

        *consWriter << "Received from " + std::to_string(clientSocket) + " " + netData;

        // Here should be a method for return an answer for command

        //netData =

        send(clientSocket, netData.data(), netData.size(), 0);

        *consWriter << "Send to " + std::to_string(clientSocket) + " " + netData;

        shutdown(clientSocket, 0);
        closesocket(clientSocket);

        *consWriter << "Disconnected " + std::to_string(clientSocket);
    }

}

void NetListener::operator()()
{
    initServer();
    connHandler();
}
