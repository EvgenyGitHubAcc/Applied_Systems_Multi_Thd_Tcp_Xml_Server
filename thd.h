#ifndef THD_H
#define THD_H

#include "consolewriter.h"

class THD
{
private:
    std::thread writerThread;
    std::string line;
public:
    void mainLoop();
    THD();
    ~THD();
};

#endif // THD_H
