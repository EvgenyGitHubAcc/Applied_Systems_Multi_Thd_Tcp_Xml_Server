#ifndef THD_H
#define THD_H

#include "consolewriter.h"

class THD
{
private:
    ConsoleWriter * writerPtr = nullptr;
    std::thread writerThread;
public:
    void mainLoop();
    THD();
    ~THD();
};

#endif // THD_H
