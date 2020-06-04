#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <mutex>

class ConsoleWriter
{
private:
    std::queue<std::string> messQueue;
    std::mutex writerMtx;
    bool thdActive = false;
    unsigned int getTimestamp();
public:
    ConsoleWriter();
    ConsoleWriter(ConsoleWriter &&);
    ConsoleWriter(const ConsoleWriter &);
    void stopWriterThd();
    void operator()(ConsoleWriter **);
    void operator<<(std::string str);
};

#endif // CONSOLEWRITER_H
