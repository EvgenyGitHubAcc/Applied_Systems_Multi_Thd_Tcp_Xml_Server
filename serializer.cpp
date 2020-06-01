#include "serializer.h"


std::string Serializer::serialize(const std::pair<std::string, std::string> & data)
{
    pugi::xml_document doc;
    pugi::xml_node message = doc.append_child("message");
    pugi::xml_node comm = message.append_child("command");
    pugi::xml_node resp = message.append_child("response");

#ifdef DEBUG_XML

    comm.append_child(pugi::node_pcdata).set_value("Comm");
    resp.append_child(pugi::node_pcdata).set_value("Resp");


#else

    comm.append_child(pugi::node_pcdata).set_value(data.first.data());
    resp.append_child(pugi::node_pcdata).set_value(data.second.data());

#endif

    doc.save_file("TestOut.xml");
    std::string text(readXmlFile("TestOut.xml"));

 #ifdef DEBUG_XML

    std::cout << text << std::endl;
 #endif

    return text;
}

std::string Serializer::readXmlFile(const std::string filename)
{
    std::fstream str(filename, std::ios::in);
    std::string text, line;
    if (str.is_open())
    {
        while(getline(str, line))
        {
            text += line;
        }
        str.close();
   }
    return text;
}

std::pair<std::string, std::string> Serializer::unserialize(const std::string & arr)
{
    pugi::xml_document doc;
    std::pair<std::string, std::string> result;

#ifdef DEBUG_XML

    if(doc.load_file("TestInput.xml"))
    {
        std::cout << "File opened" << std::endl;
    }
    else
    {
        std::cout << "Error file opening" << std::endl;
        return result;
    }

#else

    std::fstream str(arr);
    if(str.is_open())
    {
        doc.load(str);
    }
    else
    {
        return result;
    }
    str.close();

#endif

    pugi::xml_node message = doc.child("message");

    for (pugi::xml_node_iterator it = message.begin(); it != message.end(); ++it)
    {

#ifdef DEBUG_XML

        std::cout << it->name() << std::endl;

#endif

        if(std::string(it->name()) == "command")
        {
            result.first = it->child_value();
            continue;

        }
        if(std::string(it->name()) == "response")
        {
            result.second = it->child_value();
            continue;
        }
    }
    return result;
}
