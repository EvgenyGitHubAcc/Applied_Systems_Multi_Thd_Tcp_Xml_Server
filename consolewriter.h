#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

class ConsoleWriter
{
private:
    std::string line = "";
    bool thdActive = false;
    bool canPrint = true;
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
