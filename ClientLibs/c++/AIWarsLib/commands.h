#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include "tools.h"
#include "protocol.h"


namespace Commands {

    typedef std::pair< std::string,std::pair<float,float> > obstacle; // obstacle = [TYPE,[DIST,ANGLE]]


    class Command{
    protected:
        Protocol proto;
    public:
        Command();
        std::string produce_request(std::string response);
        int parse_response(std::string response);
    };


    class MoveCommand : public Command{
    public :
        MoveCommand() : Command(){}
        std::string produce_request(int ID,float dest);
    };

    class RotateCommand : public Command{
    public :
        RotateCommand() : Command(){}
        std::string produce_request(int ID,float angle);
    };

    class FireCommand : public Command{
    public :
        FireCommand() : Command(){}
        std::string produce_request(int ID);
    };

    class ScanCommand : public Command{
    public :
        ScanCommand() : Command(){}
        std::string produce_request(int ID);
        std::pair< int , std::vector<obstacle> > parse_response(std::string response);

    };

}
#endif // COMMANDS_H
