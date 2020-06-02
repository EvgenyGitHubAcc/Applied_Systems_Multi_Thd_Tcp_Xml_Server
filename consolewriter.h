#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

class ConsoleWriter
{
private:
    std::string& line;
    bool thdActive = false;
    unsigned int getTimestamp();
public:
    ConsoleWriter(std::string &);
    void stopWriterThd();
    void operator()();
};

#endif // CONSOLEWRITER_H
