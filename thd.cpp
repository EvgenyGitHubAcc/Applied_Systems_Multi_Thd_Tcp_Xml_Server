#include "thd.h"

void THD::mainLoop()
{
    if(!writerPtr)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    while (true)
    {
        *writerPtr << "Hi";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

THD::THD()
{
    ConsoleWriter writer;
    writerThread = std::move(std::thread(writer, &writerPtr));
}

THD::~THD()
{

}
