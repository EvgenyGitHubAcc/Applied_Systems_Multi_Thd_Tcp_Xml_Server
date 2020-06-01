#include <iostream>

#include "serializer.h"

int main(void)
{
    byteArray arr;
    std::pair<std::string, std::string> data;
    Serializer::unserialize(arr);
    Serializer::serialize(data);
    system("pause");
    return 0;
}
