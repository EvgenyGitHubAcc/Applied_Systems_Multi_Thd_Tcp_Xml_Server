#include "consolewriter.h"

void ConsoleWriter::operator<<(std::string str)
{
    canPrint = false;
    messQueue.push(std::move(str));
    canPrint = true;
}

unsigned int ConsoleWriter::getTimestamp()
{
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    return s.count();
}

ConsoleWriter::ConsoleWriter(ConsoleWriter && obj)
{
    messQueue = std::move(obj.messQueue);
    thdActive = obj.thdActive;
    canPrint = obj.canPrint;
    messQueue = std::move(std::queue<std::string>());
    obj.thdActive = NULL;
    obj.canPrint = NULL;
}

ConsoleWriter::ConsoleWriter(const ConsoleWriter & obj)
{
    messQueue = obj.messQueue;
    thdActive = obj.thdActive;
    canPrint = obj.canPrint;
}

ConsoleWriter::ConsoleWriter()
{

}

void ConsoleWriter::stopWriterThd()
{
    thdActive = false;
}

void ConsoleWriter::operator()(ConsoleWriter ** consoleWriterPtr)
{
    *consoleWriterPtr = this;
    thdActive = true;
    while(thdActive)
    {
        if(canPrint && !messQueue.empty())
        {
            std::cout << getTimestamp() << ": " << messQueue.front() << std::endl;
            messQueue.pop();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
