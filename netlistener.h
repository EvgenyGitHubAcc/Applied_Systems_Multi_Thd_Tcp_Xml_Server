#ifndef NETLISTENER_H
#define NETLISTENER_H

#define SRV_PORT 1234
#define MAX_CLIENTS_COUNT 10
#define BUF_SIZE 64

#include <winsock.h>
#include <string>
#include "consolewriter.h"

class NetListener
{
private:
    ConsoleWriter * consWriter = nullptr;
    std::string netData;
    SOCKET servSocket;
    SOCKET clientSocket;
    struct sockaddr_in sin = {0};
    struct sockaddr_in from_sin = {0};
public:
    NetListener(ConsoleWriter *_consWriter);
    void initServer();
    void connHandler();
    void operator()();
};

#endif // NETLISTENER_H
