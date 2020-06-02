#include "consolewriter.h"

unsigned int ConsoleWriter::getTimestamp()
{
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    return s.count();
}

ConsoleWriter::ConsoleWriter(std::string & str) : line(str)
{

}

void ConsoleWriter::stopWriterThd()
{
    thdActive = false;
}

void ConsoleWriter::operator()()
{
    thdActive = true;
    while(thdActive)
    {
        if(line.size())
        {
            std::cout << getTimestamp() << " " << line << std::endl;
            line.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
