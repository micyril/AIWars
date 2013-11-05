#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include "tools.h"
#include "protocol.h"


namespace Commands {
    //------base CommandAnswer--------
    class CommandAnswer{
    public:
        int state;
        CommandAnswer(int state = 1);
    };
    //--------------------------------

    //----- WorldObject class --------
    class WorldObject{
    public:
        WorldObject(std::string type = "",float distance = 0.0f,float angle = 0.0f);
        ~WorldObject();
        std::string type;
        float distance;
        float angle;
    };
    //--------------------------------

    //---------Scan Answer------------
    class ScanCommandAnswer : public CommandAnswer{
    public:
        std::vector<WorldObject> scannedWorldObjects;
        ScanCommandAnswer();
        ScanCommandAnswer(std::vector<WorldObject> scWorldObjects,int state);
    };
    //--------------------------------

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
        ScanCommandAnswer parse_response(std::string response);

    };
    //-------------------------------

}
#endif // COMMANDS_H
