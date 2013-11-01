#include "tools.h"



std::vector<std::string> Tools::StringSplitter::split(std::string text,char splitter){
        std::vector<std::string> elems;
        std::stringstream sstream(text);
        std::string token;
        while (std::getline(sstream, token, splitter)) {
            elems.push_back(token);
        }
        return elems;
}

std::vector<std::string> Tools::StringSplitter::split(std::string text,char splitter,int count){
    std::vector<std::string> elems;
    std::stringstream sstream(text);
    std::string token;
    int i = 0;
    while ((i < count) && std::getline(sstream, token, splitter) ) {
        elems.push_back(token);
        //text.erase(text.find(token),token.length()+1);
        i++;
    }
    std::getline(sstream, token);
    elems.push_back(token);
    return elems;
}
