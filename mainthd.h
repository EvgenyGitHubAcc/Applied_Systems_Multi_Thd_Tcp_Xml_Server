#ifndef THD_H
#define THD_H

#include "consolewriter.h"
#include "netlistener.h"
#include "pugixml.hpp"
#include <map>

class NetListener;

class MainTHD
{
private:
    ConsoleWriter * writerPtr = nullptr;
    NetListener * networkPtr = nullptr;
    std::thread writerThread;
    std::thread networkThread;
    std::thread checkSockThread;
    std::map<std::string, std::string> dict;
    bool fillDict();
public:
    void mainLoop();
    const std::string & getResponse(const std::string &);
    MainTHD();
    ~MainTHD();
};

#endif // THD_H
