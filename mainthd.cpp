#include "mainthd.h"

bool MainTHD::fillDict()
{
    pugi::xml_document doc;
    std::pair<std::string, std::string> dictCurrValue;
    if(!doc.load_file("ComRespServer.xml"))
    {
        *writerPtr << "Error XML file opening";
        return false;
    }

    *writerPtr << "XML file loaded";

    pugi::xml_node message = doc.child("message");

    for (pugi::xml_node_iterator it = message.begin(); it != message.end(); ++it)
    {
        if(std::string(it->name()) == "command")
        {
            dictCurrValue.first = it->child_value();
        }
        if(std::string(it->name()) == "response")
        {
            dictCurrValue.second = it->child_value();
        }
        if(dictCurrValue.first.size() && dictCurrValue.second.size())
        {
            dict.insert(dictCurrValue);
            dictCurrValue = {"", ""};
        }
    }

    *writerPtr << "Commands loaded";

    return true;
}

void MainTHD::mainLoop()
{
    if(!fillDict())
    {
        *writerPtr << "XML file open faild. The program stopped...";
        exit(0);
    }

    while (true)
    {
#ifdef CONS_WRITE_DEBUG
        *writerPtr << "Hi";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
    }
}

std::mutex *MainTHD::getNetMtx()
{
    return &netMtx;
}

const std::string MainTHD::getResponse(const std::string & key)
{
    if(key.find(key))
    {
        return dict[key];
    }
    else
    {
        return std::string("");
    }

}

MainTHD::MainTHD()
{
    ConsoleWriter writer;
    writerThread = std::move(std::thread(writer, &writerPtr));
    while(!writerPtr)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

#ifndef CONS_WRITE_DEBUG
    NetListener server(writerPtr, this);

    networkThread = std::move(std::thread(std::move(server), &networkPtr));
    while(!networkPtr)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
//    checkSockThread = std::move(std::thread(&NetListener::checkClientSock, &(*networkPtr)));
#endif
}

MainTHD::~MainTHD()
{
//    if(checkSockThread.joinable())
//    {
//        checkSockThread.join();
//    }
    if(networkThread.joinable())
    {
        (networkThread.join());
    }
    if(writerThread.joinable())
    {
        writerThread.join();
    }
}
