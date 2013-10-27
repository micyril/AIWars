#include "commands.h"

using namespace Commands;

//--------------------------------
//------------CMD names-----------
//--------------------------------
std::string Command::ACK = "ACK";
std::string Command::NAK = "NAK";
std::string Command::EOG = "EOG";
std::string Command::RET = "RET";

MoveCommand::MoveCommand() : Command(){
    this->NAME = "MOV";
}

RotateCommand::RotateCommand() : Command(){
    this->NAME = "ROT";
}

FireCommand::FireCommand() : Command(){
    this->NAME = "FR";
}

ScanCommand::ScanCommand() : Command(){
    this->NAME = "SC";
}
//--------------------------------
//--------------------------------



//---------------WorldObject-------------------------
WorldObject::WorldObject(std::string type, float distance, float angle){
    this->type = type;
    this->distance = distance;
    this->angle = angle;
}
WorldObject::~WorldObject(){
}

//------------------------------------------------

//------------------------------------------------
//--------- base class methods--------------------
//------------------------------------------------
Command::Command(){}

int Command::parse_response(std::string response){

    Message msg = Protocol::get_msg_by_line(response);

    // ACK = 0 , NAK = 1 , EOG = -1 , default = NAK = 1

    if      (msg.head == this->ACK) return 0;
    else if (msg.head == this->NAK) return 1;
    else if (msg.head == this->EOG) return -1;

    return 1;
}

std::string Command::produce_request(std::string response){
    return response + Message::end_line;
}
//------------------------------------------------
//------------------------------------------------

//--------MoveCommand--------------
std::string MoveCommand::produce_request(int ID, float dest){
    return Command::produce_request(Protocol::get_line_by_fields(Tools::ToStrConverter<int>::convert(ID),this->NAME,Tools::ToStrConverter<float>::convert(dest)));
}
//---------------------------------


//--------RotateCommand------------
std::string RotateCommand::produce_request(int ID, float angle){
    return Command::produce_request(Protocol::get_line_by_fields(Tools::ToStrConverter<int>::convert(ID),this->NAME,Tools::ToStrConverter<float>::convert(angle)));
}
//---------------------------------


//--------FireCommand--------------
std::string FireCommand::produce_request(int ID){
    return Command::produce_request(Protocol::get_line_by_fields(Tools::ToStrConverter<int>::convert(ID),this->NAME,""));
}
//---------------------------------

//--------ScanCommand--------------
std::string ScanCommand::produce_request(int ID){
    return Command::produce_request(Protocol::get_line_by_fields(Tools::ToStrConverter<int>::convert(ID),this->NAME,""));
}

std::pair< int , std::vector<WorldObject> > ScanCommand::parse_response(std::string response){
    std::pair< int , std::vector<WorldObject> > out;

    Message msg  = Protocol::get_msg_by_line(response);

    if      (msg.head == this->ACK) out.first =  0;
    else if (msg.head == this->NAK){

        out.first =  1;
        return out;
    }
    else if (msg.head == this->EOG){
        out.first =  -1;
        return out;
    }

    if(msg.raw_command != "RET"){
        out.first = -1;
        return out;
    }

    std::vector<std::string> args = Tools::StringSplitter::split(msg.args_line," ");
    std::vector<WorldObject> parsed_args;
    for(int i = 2; i < args.size(); i+=3){
       parsed_args.push_back(WorldObject(args[i-2],Tools::FromStrConverter<float>::convert(args[i-1]),Tools::FromStrConverter<float>::convert(args[i])));
    }

    out.second = parsed_args;

    return out;
}

//---------------------------------


