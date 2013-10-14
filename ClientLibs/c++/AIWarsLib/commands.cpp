#include "commands.h"

using namespace Commands;

Command::Command(){}

int Command::parse_response(std::string response){

    this->proto.setprotoline(response);

    // ACK = 0 , NAK = 1 , EOG = -1 , default = NAK = 1

    if      (this->proto.head == this->proto.ACK) return 0;
    else if (this->proto.head == this->proto.NAK) return 1;
    else if (this->proto.head == this->proto.EOG) return -1;

    return 1;
}

std::string Command::produce_request(std::string response){
    return response+this->proto.end_line;
}


//--------MoveCommand--------------
std::string MoveCommand::produce_request(int ID, float dest){
    this->proto.setprotofields(Tools::ToStrConverter<int>::convert(ID),this->proto.MOV,Tools::ToStrConverter<float>::convert(dest));
    return this->proto.getprotoline();
}
//---------------------------------


//--------RotateCommand------------
std::string RotateCommand::produce_request(int ID, float angle){
    this->proto.setprotofields(Tools::ToStrConverter<int>::convert(ID),this->proto.ROT,Tools::ToStrConverter<float>::convert(angle));
    return this->proto.getprotoline();
}
//---------------------------------


//--------FireCommand--------------
std::string FireCommand::produce_request(int ID){
    this->proto.setprotofields(Tools::ToStrConverter<int>::convert(ID),this->proto.FR,"");
    return this->proto.getprotoline();
}
//---------------------------------

//--------ScanCommand--------------
std::string ScanCommand::produce_request(int ID){
    this->proto.setprotofields(Tools::ToStrConverter<int>::convert(ID),this->proto.SC,"");
    return this->proto.getprotoline();
}

std::pair< int , std::vector<obstacle> > ScanCommand::parse_response(std::string response){
    int state;
    std::pair< int , std::vector<obstacle> > out;

    if((state = Command::parse_response(response)) != 0){ //attention ! this->proto will be changed
        out.first = state;
        return out;
    }

    out.first = state;

    std::vector<std::string> args = Tools::StringSplitter::split(this->proto.args_line," ");
    std::vector<obstacle> parsed_args;
    for(int i = 2; i < args.size(); i+=3){
        parsed_args.push_back(obstacle(args[i-2],std::pair<float,float>(Tools::FromStrConverter<float>::convert(args[i-1]),Tools::FromStrConverter<float>::convert(args[i]))));
    }
    out.second = parsed_args;
    return out;
}

//---------------------------------


