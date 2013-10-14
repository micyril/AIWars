#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string.h>
#include "tools.h"
class Protocol
{

public:
    static std::string ACK;
    static std::string NAK;
    static std::string EOG;
    static std::string MOV;
    static std::string ROT;
    static std::string FR;
    static std::string SC;
    static std::string RET;



    std::string head; // ID|ACK|NAK|EOG
    std::string raw_command; // MOV|ROT|FR|SC|RET
    std::string args_line; // [FLOATS]|[TYPE,[FLOATS]]
    std::string end_line;

    Protocol(std::string head = "",std::string raw_command = "",std::string args_line = "");
    void setprotofields(std::string head ,std::string raw_command ,std::string args_line);
    void setprotoline(std::string line);
    std::string getprotoline();

};



#endif // PROTOCOL_H
