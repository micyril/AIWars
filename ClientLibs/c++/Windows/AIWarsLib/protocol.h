#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string.h>
#include "tools.h"


class Message{

// ACK = 1 , NAK = 0 , EOG = -1

public:
    Message(std::string head  = "" ,std::string raw_command = "" ,std::string args_line = "");
    ~Message();
    std::string head; // ID|ACK|NAK|EOG
    std::string raw_command; // MOV|ROT|FR|SC|RET
    std::string args_line; // [FLOATS]|[TYPE,[FLOATS]]
    static std::string end_line;
};

class Protocol{
public:
    Protocol(){}
    static Message get_msg_by_line(std::string line);
    static std::string get_line_by_msg(Message &msg);
    static std::string get_line_by_fields(std::string head, std::string raw_command, std::string args_line);
    static std::string get_simple_msg_line(std::string str_msg);
};



#endif // PROTOCOL_H
