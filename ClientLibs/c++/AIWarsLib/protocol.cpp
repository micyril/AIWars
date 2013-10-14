#include "protocol.h"
//--------------------------------
//------------CMD names-----------
//--------------------------------
std::string Protocol::ACK = "ACK";
std::string Protocol::NAK = "NAK";
std::string Protocol::EOG = "EOG";
std::string Protocol::MOV = "MOV";
std::string Protocol::ROT = "ROT";
std::string Protocol::FR  = "FR" ;
std::string Protocol::SC  = "SC" ;
std::string Protocol::RET = "RET";
//--------------------------------
//--------------------------------


Protocol::Protocol(std::string head ,std::string raw_command ,std::string args_line ){
    this->head = head;
    this->raw_command = raw_command;
    this->args_line = args_line;
    this->end_line = "\n";
}

void Protocol::setprotofields(std::string head, std::string raw_command, std::string args_line) {
    this->head = head;
    this->raw_command = raw_command;
    this->args_line = args_line;
}

void Protocol::setprotoline(std::string line){
    std::vector< std::string > tmp = Tools::StringSplitter::split(line," ",2);
    this->head = "";
    this->raw_command = "";
    this->args_line = "";

    switch (tmp.size()) {
    case 3:
        this->args_line = tmp[2];
    case 2:
        this->raw_command = tmp[1];
    case 1:
        this->head = tmp[0];
        break;
    default:
        break;
    }
}

std::string Protocol::getprotoline(){
    return this->head+" "+this->raw_command+" "+this->args_line+this->end_line;
}
