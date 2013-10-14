#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string.h>
#include <pair.h>
#include <vector.h>
#include "commands.h"

using namespace Commands;

class Controller{
private:

public:
    Controller();
    bool move(float destination);
    bool rotate(float angle);
    bool fire();
    std::vector<Obstacle> scan();
};



#endif // CONTROLLER_H
