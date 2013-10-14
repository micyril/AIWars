#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <iostream>
#include <sstream>
#include <vector.h>

namespace Tools {

    template <typename InType,typename OutType>
    class Converter
    {
    public:
        Converter(){}
        OutType convert(InType in);
    };

    //-----------------------------------------------------------------------
    template <typename InType>
    class ToStrConverter : Converter<InType,std::string>{
        public:
            static std::string convert(InType in){
                std::ostringstream ss ;
                ss<<in;
                return ss.str();
            }
    };
    //------------------------------------------------------------------------

    //-----------------------------------------------------------------------
    template <typename OutType>
    class FromStrConverter : Converter<std::string,OutType>{
        public:
            static OutType convert(std::string in){
                std::istringstream ss(in);
                OutType tmp;
                ss>>tmp;
                return tmp;
            }
    };
    //------------------------------------------------------------------------



    class StringSplitter{
        public:
        StringSplitter(){}
        static std::vector<std::string> split(std::string text,std::string splitter){
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
        static std::vector<std::string> split(std::string text,std::string splitter,int count){
            std::vector<std::string> out;
            const char* splt = splitter.c_str();
            int splt_ln = splitter.length();
            char* txt = (char*)text.c_str();
            char* pnt;
            pnt = strtok(txt,splt);
            int i = 0;
            while(pnt){
                if(i==count){
                    out.push_back(text);
                    break;
                }
                out.push_back(pnt);
                text.erase(0,strlen(pnt) + splt_ln);
                pnt = strtok(NULL,splt);
                i++;
            }
            return out;
        }

    };

}
#endif // TOOLS_H
