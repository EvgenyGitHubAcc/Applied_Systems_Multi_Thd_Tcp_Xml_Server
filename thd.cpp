#include "thd.h"

void THD::mainLoop()
{
    while (true)
    {
        line = "Hi";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

THD::THD()
{
    ConsoleWriter writer(line);
    writerThread = std::move(std::thread(writer));
}

THD::~THD()
{

}
