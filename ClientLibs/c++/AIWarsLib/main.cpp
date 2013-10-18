#include <iostream>
#include "controller.h"
#include "commutator.h"
#include "time.h"
using namespace std;
using namespace Commands;

int main()
{
    Controller ctrl("213.159.196.198","2001","0");
    if(ctrl.ready()){
        cout<<"start"<<endl;
        for(int i = 0; i < 1000; i++){
            ctrl.move((float)i);
            ctrl.fire();
            ctrl.rotate((float)i);
            vector<Obstacle> v = ctrl.scan();
        }

    cout<<"end"<<endl;
    ctrl.end_game();
    }

    return 0;
}

