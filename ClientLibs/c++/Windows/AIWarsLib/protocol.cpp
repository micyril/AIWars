#include "protocol.h"


std::string Message::end_line = "\r\n";

Message::Message(std::string head ,std::string raw_command ,std::string args_line ){
    this->head = head;
    this->raw_command = raw_command;
    this->args_line = args_line;
}

Message::~Message(){
}

Message Protocol::get_msg_by_line(std::string line){
    size_t index = line.find(Message::end_line,Message::end_line.length());
    if(index != std::string::npos)
        line.erase(index);
    std::vector< std::string > tmp = Tools::StringSplitter::split(line,' ',2);
    Message result;
    switch (tmp.size()) {
    case 3:
        result.args_line = tmp[2];
    case 2:
        result.raw_command = tmp[1];
    case 1:
        result.head = tmp[0];
        break;
    default:
        break;
    }
    return result;
}


std::string Protocol::get_line_by_msg(Message &msg){
    return msg.head+" "+msg.raw_command+" "+msg.args_line+" "+Message::end_line;
}


std::string Protocol::get_line_by_fields(std::string head, std::string raw_command, std::string args_line){
    return head+" "+raw_command+" "+args_line+" "+Message::end_line;
}

std::string Protocol::get_simple_msg_line(std::string str_msg){
    return str_msg+Message::end_line;
}
