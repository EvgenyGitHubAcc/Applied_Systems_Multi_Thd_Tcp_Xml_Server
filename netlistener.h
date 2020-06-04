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


class MainTHD;

class NetListener
{
private:
    ConsoleWriter * consWriter = nullptr;
    MainTHD * mainThd = nullptr;
    SOCKET servSocket = 0;
    std::deque<SOCKET> clientSockDeque;
    struct sockaddr_in sin = {0};
    struct sockaddr_in from_sin = {0};
public:
    NetListener(ConsoleWriter *, MainTHD *);
    bool initServer();
    void connHandler();
    void checkClientSock();
    int recvWithTimeOut(SOCKET, char *, int, int, long);
    void operator()(NetListener **);
};

#endif // NETLISTENER_H
