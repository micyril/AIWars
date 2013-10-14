#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include "tools.h"
#include "protocol.h"


namespace Commands {

    //----- obstacle class ----------
    class Obstacle{
    public:
        Obstacle(std::string type = "",float distance = 0.0f,float angle = 0.0f);
        std::string type;
        float distance;
        float angle;
    };
    //-------------------------------

    //----------base cmd class--------
    class Command{
    protected:
        Protocol proto;
    public:
        Command();
        std::string produce_request(std::string response);
        int parse_response(std::string response);
    };
    //-------------------------------


    //-----------move cmd------------
    class MoveCommand : public Command{
    public :
        MoveCommand() : Command(){}
        std::string produce_request(int ID,float dest);
    };
    //-------------------------------


    //----------rotate cmd-----------
    class RotateCommand : public Command{
    public :
        RotateCommand() : Command(){}
        std::string produce_request(int ID,float angle);
    };
    //-------------------------------


    //-----------fire cmd------------
    class FireCommand : public Command{
    public :
        FireCommand() : Command(){}
        std::string produce_request(int ID);
    };
    //-------------------------------


    //----------scan cmd-------------
    class ScanCommand : public Command{
    public :
        ScanCommand() : Command(){}
        std::string produce_request(int ID);
        std::pair< int , std::vector<Obstacle> > parse_response(std::string response);

    };
    //-------------------------------

}
#endif // COMMANDS_H
