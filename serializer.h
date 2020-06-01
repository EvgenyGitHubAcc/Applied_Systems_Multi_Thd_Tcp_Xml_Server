#ifndef SERIALIZER_H
#define SERIALIZER_H

#include<string>
#include<fstream>

#ifdef DEBUG_XML
#include <iostream>
#endif


#include "pugiconfig.hpp"
#include "pugixml.hpp"

using byteArray = char*;

class Serializer
{
private:
    static std::string readXmlFile(const std::string);
public:
    static std::pair<std::string, std::string> unserialize(const std::string &);
    static std::string serialize(const std::pair<std::string, std::string> & data);
};

#endif // SERIALIZER_H
