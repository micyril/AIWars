#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

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
        static std::vector<std::string> split(std::string text,char splitter);
        static std::vector<std::string> split(std::string text,char splitter,int count);
    };

}
#endif // TOOLS_H
