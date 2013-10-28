#include "tools.h"



std::vector<std::string> Tools::StringSplitter::split(std::string text,std::string splitter){
    std::vector<std::string> out;
    const char* splt = splitter.c_str();
    char* txt = (char*)text.c_str();
    char* pnt;
    pnt = strtok(txt,splt);
    while(pnt){
        out.push_back(pnt);
        pnt = strtok(NULL,splt);
    }
    return out;
}

std::vector<std::string> Tools::StringSplitter::split(std::string text,std::string splitter,int count){
    std::vector<std::string> out;

    const char* splt = splitter.c_str();
    int splt_ln = splitter.length();
    char* txt = (char*)text.c_str();
    char* pnt;

    int i = 0;
    do{
        if(i==count){
            out.push_back(text);
            break;
        }
        pnt = strtok(txt,splt);
        if(!pnt) break;
        out.push_back(pnt);
        text.erase(0,strlen(pnt) + splt_ln);
        i++;
    } while(pnt);
    return out;
}
