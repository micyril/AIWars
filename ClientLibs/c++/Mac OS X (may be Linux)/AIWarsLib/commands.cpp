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


//--------------base commandAnswer----------------
CommandAnswer::CommandAnswer(int state){
    this->state = state;
}
//------------------------------------------------

//---------------WorldObject----------------------
WorldObject::WorldObject(std::string type, float distance, float angle){
    this->type = type;
    this->distance = distance;
    this->angle = angle;
}
WorldObject::~WorldObject(){
}
//------------------------------------------------

//--------------Scan Command Answer---------------
ScanCommandAnswer::ScanCommandAnswer(std::vector<WorldObject> scWorldObjects, int state) : CommandAnswer(state){
    this->scannedWorldObjects = scWorldObjects;
}

ScanCommandAnswer::ScanCommandAnswer(){
}

//------------------------------------------------

//------------------------------------------------
//--------- base class methods--------------------
//------------------------------------------------
Command::Command(){}

int Command::parse_response(std::string response){

    if(response.length() == 0){
        return -1;
    }

    Message msg = Protocol::get_msg_by_line(response);

    // ACK = 1 , NAK = 0 , EOG = -1 , default = NAK = 0
    if      (msg.head == this->ACK) return 1;
    else if (msg.head == this->NAK) return 0;
    else if (msg.head == this->EOG) return -1;

    return 0;
}

std::string Command::produce_request(std::string response){
    return response;
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

ScanCommandAnswer ScanCommand::parse_response(std::string response){
    ScanCommandAnswer out;
    if(response.length() == 0){
       out.state = -1;
       return out;
    }

    Message msg  = Protocol::get_msg_by_line(response);

    if      (msg.head == this->ACK) out.state =  1;
    else if (msg.head == this->NAK){
        out.state =  0;
        return out;
    }
    else if (msg.head == this->EOG){
        out.state =  -1;
        return out;
    }

    if(msg.raw_command != "RET"){
        out.state = 0;
        return out;
    }

    std::vector<std::string> args = Tools::StringSplitter::split(msg.args_line,' ');
    std::vector<WorldObject> parsed_args;
    for(int i = 2; i < args.size(); i+=3){
       parsed_args.push_back(WorldObject(args[i-2],Tools::FromStrConverter<float>::convert(args[i-1]),Tools::FromStrConverter<float>::convert(args[i])));
    }

    out.scannedWorldObjects = parsed_args;

    return out;
}

//---------------------------------


