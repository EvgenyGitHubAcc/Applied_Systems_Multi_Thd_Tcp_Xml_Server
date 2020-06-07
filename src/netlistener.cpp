#include "netlistener.h"

NetListener::NetListener(ConsoleWriter * _consWriter, MainTHD * _mainThd) : consWriter(_consWriter),
    mainThd(_mainThd),
    netMtxPtr(_mainThd->getNetMtx())
{

}

NetListener::NetListener(NetListener && obj)
{
    consWriter = obj.consWriter;
    obj.consWriter = nullptr;
    mainThd = obj.mainThd;
    obj.mainThd = nullptr;
    netMtxPtr = obj.netMtxPtr;
    obj.netMtxPtr = nullptr;
    servSocket = obj.servSocket;
    obj.servSocket = SOCKET_ERROR;
    clientSockThdMap = std::move(obj.clientSockThdMap);
    sin = std::move(obj.sin);
    from_sin = std::move(obj.from_sin);
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

void NetListener::shutdownServer()
{
    shutdown(servSocket, 0);
    closesocket(servSocket);
    *consWriter << "Disconnected server socket: " + std::to_string(servSocket) + ". Error:" + std::to_string(WSAGetLastError());
    WSACleanup();
}

void NetListener::connHandler()
{
    while(true)
    {
        SOCKET clientSocket = INVALID_SOCKET;

        int from_len = sizeof(from_sin);

        clientSocket = accept(servSocket, (struct sockaddr *)&from_sin, &from_len);

        if(clientSocket == INVALID_SOCKET)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            WSACleanup();
            continue;
        }

        std::thread currThd(&NetListener::clientHandler, &(*this), clientSocket);
        std::pair<SOCKET, std::thread> currPair(clientSocket, std::move(currThd));

        {
            std::lock_guard<std::mutex> lock(*netMtxPtr);
            clientSockThdMap.insert(std::move(currPair));
            clientSockThdMap[clientSocket].detach();
        }

        *consWriter << "Connected socket: " + std::to_string(clientSocket);
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

void NetListener::clientHandler(SOCKET sock)
{
    std::string text = "";
    char recvBuf[BUF_SIZE] = "";

    //while(recvWithTimeOut(sock, recvBuf, BUF_SIZE, 0, TIMEOUT) != SOCKET_ERROR)

    //    std::thread currThd;

    //    {
    //        std::lock_guard<std::mutex> lock(*netMtxPtr);
    //        currThd = std::move(clientSockThdMap[sock]);
    //        clientSockThdMap.erase(sock);
    //    }

    int bytesRecv = 0;

    while(true)
    {
        bytesRecv = recv(sock, recvBuf, BUF_SIZE, 0);

//        try
//        {
//            bytesRecv = recv(sock, recvBuf, BUF_SIZE, 0);
//        }
//        catch (...)
//        {
//            *consWriter << "Exception caught";
//            break;
//        }

        if(bytesRecv == SOCKET_ERROR)
        {
            *consWriter << "Error receive from socket " + std::to_string(sock) + ". Error: " + std::to_string(WSAGetLastError());
            break;
        }

        else if(!bytesRecv)
        {
            break;
        }

        text = std::move(std::string(recvBuf));
        *consWriter << "Received from socket: " + std::to_string(sock) + " " + text;

        text = std::move(mainThd->getResponse(text));

        if(text.size())
        {
            send(sock, text.data(), text.size(), 0);
            *consWriter << "Send to socket: " + std::to_string(sock) + " " + text;
        }
        else
        {
            send(sock, "No answer", 11, 0);
            *consWriter << "No answer was send, because no command is found in map";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    shutdown(sock, 0);
    closesocket(sock);
    WSACleanup();
    *consWriter << "Disconnected socket: " + std::to_string(sock);

    std::lock_guard<std::mutex> lock(*netMtxPtr);
    clientSockThdMap.erase(sock);
}

void NetListener::operator()(NetListener ** netPtr)
{
    *netPtr = this;
    if(!initServer())
    {
        return;
    }
    connHandler();

    //    while(true)
    //    {
    //        try
    //        {
    //            connHandler();
    //        }
    //        catch (...)
    //        {
    //            shutdownServer();
    //            if(!initServer())
    //            {
    //                return;
    //            }
    //        }
    //    }
}
