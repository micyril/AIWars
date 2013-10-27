#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include "tools.h"
#include "protocol.h"


namespace Commands {

    //----- WorldObject class ----------
    class WorldObject{
    public:
        WorldObject(std::string type = "",float distance = 0.0f,float angle = 0.0f);
        ~WorldObject();
        std::string type;
        float distance;
        float angle;
    };
    //-------------------------------

    //----------base cmd class--------
    class Command{
    protected:
        static std::string ACK;
        static std::string NAK;
        static std::string EOG;
        static std::string RET;
    public:
        std::string NAME;
        Command();
        std::string produce_request(std::string response);
        int parse_response(std::string response);
    };
    //-------------------------------


    //-----------move cmd------------
    class MoveCommand : public Command{
    public:
        MoveCommand();
        std::string produce_request(int ID,float dest);
    };
    //-------------------------------


    //----------rotate cmd-----------
    class RotateCommand : public Command{
    public:
        RotateCommand();
        std::string produce_request(int ID,float angle);
    };
    //-------------------------------


    //-----------fire cmd------------
    class FireCommand : public Command{
    public:
        FireCommand();
        std::string produce_request(int ID);
    };
    //-------------------------------


    //----------scan cmd-------------
    class ScanCommand : public Command{
    public:
        ScanCommand();
        std::string produce_request(int ID);
        std::pair< int , std::vector<WorldObject> > parse_response(std::string response);

    };
    //-------------------------------

}
#endif // COMMANDS_H
