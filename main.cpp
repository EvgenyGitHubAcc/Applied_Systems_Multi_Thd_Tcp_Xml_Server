#include <iostream>
#include <thread>
#include "consolewriter.h"
#include "thd.h"

int main(void)
{
    THD thds;
    thds.mainLoop();

    system("pause");
    return 0;
}
