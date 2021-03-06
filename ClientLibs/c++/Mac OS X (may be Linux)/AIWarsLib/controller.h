#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string.h>
#include <vector>
#include "commutator.h"
#include "commands.h"

using namespace Commands;

class Controller{
private:
    Commutator comm;
    int ID;

    MoveCommand move_cmd;
    RotateCommand rotate_cmd;
    FireCommand fire_cmd;
    ScanCommand scan_cmd;

    int state_process(int state);
public:
    Controller(std::string host = "localhost", std::string port = "2345", std::string ID = "0");
    bool ready();
    bool end_game();
    int move(float destination);
    int rotate(float angle);
    int fire();
    ScanCommandAnswer scan();
};



#endif // CONTROLLER_H
