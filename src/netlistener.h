#ifndef NETLISTENER_H
#define NETLISTENER_H

#define SRV_PORT 1234
#define MAX_CLIENTS_COUNT 10
#define BUF_SIZE 64
#define TIMEOUT 1

#include <winsock.h>
#include <string>
#include <deque>
#include "consolewriter.h"
#include "mainthd.h"
#include <algorithm>
#include <mutex>
#include <map>
#include <sstream>

class MainTHD;

class NetListener
{
private:
    ConsoleWriter * consWriter = nullptr;
    MainTHD * mainThd = nullptr;
    SOCKET servSocket = 0;
    std::mutex * netMtxPtr;
    std::map<SOCKET, std::thread> clientSockThdMap;
    struct sockaddr_in sin = {0};
    struct sockaddr_in from_sin = {0};
public:
    NetListener(ConsoleWriter *, MainTHD *);
    NetListener(NetListener &&);
    bool initServer();
    void connHandler();
    int recvWithTimeOut(SOCKET, char *, int, int, long);
    void clientHandler(SOCKET);
    void operator()(NetListener **);
};

#endif // NETLISTENER_H
