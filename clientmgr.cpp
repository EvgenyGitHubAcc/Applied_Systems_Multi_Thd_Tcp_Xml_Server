#include "clientmgr.h"

ClientMgr::ClientMgr()
{

}

void ClientMgr::operator()()
{
//    char recvBuf[BUF_SIZE];
//    if(recv(clientSocket, recvBuf, BUF_SIZE, 0) == SOCKET_ERROR)
//    {
//        *consWriter << "Error receving data from " + std::to_string(clientSocket) + ". Error: " + std::to_string(WSAGetLastError());
//        shutdown(clientSocket, 0);
//        closesocket(clientSocket);
//        WSACleanup();
//        return;
//    }

//    netData = std::move(std::string(recvBuf));

//    *consWriter << "Received from socket: " + std::to_string(clientSocket) + " " + netData;

//    netData = mainThd->getResponse(netData);

//    if(netData.size())
//    {
//        send(clientSocket, netData.data(), netData.size(), 0);
//        *consWriter << "Send to socket: " + std::to_string(clientSocket) + " " + netData;
//    }
//    else
//    {
//        *consWriter << "Answer was not send, because no command is found in map";
//    }
}
