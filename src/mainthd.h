#ifndef THD_H
#define THD_H

#include "consolewriter.h"
#include "netlistener.h"
#include "pugixml.hpp"
#include <map>
#include <string>

class NetListener;

class MainTHD
{
private:
    ConsoleWriter * writerPtr = nullptr;
    NetListener * networkPtr = nullptr;
    std::thread writerThread;
    std::thread networkThread;
    std::mutex netMtx;
//    std::thread checkSockThread;
    std::map<std::string, std::string> dict;
    bool fillDict();
public:
    void mainLoop();
    std::mutex * getNetMtx();
    const std::string getResponse(const std::string &);
    MainTHD();
    ~MainTHD();
};

#endif // THD_H
