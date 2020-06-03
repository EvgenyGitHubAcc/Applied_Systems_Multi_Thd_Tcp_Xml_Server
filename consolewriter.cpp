#include "consolewriter.h"

void ConsoleWriter::operator<<(std::string str)
{
    canPrint = false;
    line = std::move(str);
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
    line = std::move(obj.line);
    thdActive = obj.thdActive;
    canPrint = obj.canPrint;
    obj.line = "";
    obj.thdActive = NULL;
    obj.canPrint = NULL;
}

ConsoleWriter::ConsoleWriter(const ConsoleWriter & obj)
{
    line = obj.line;
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
        if(line.size() && canPrint)
        {
            std::cout << getTimestamp() << " " << line << std::endl;
            line.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
