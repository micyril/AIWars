#include <iostream>
#include "controller.h"
#include "time.h"
using namespace std;
using namespace Commands;

int main()
{

    Controller ctrl("87.251.167.242","2560","0");
    if(ctrl.ready()){
        cout<<"start"<<endl;
        for(int i = 0; i < 100; i++){
            cout<<ctrl.move(0.1)<<" ";
            /*cout<<ctrl.fire()<<" ";
            cout<<ctrl.rotate(10)<<" ";
            vector<WorldObject> v = ctrl.scan();
            cout<<v[3].distance<<endl;
            if (v.size() == 0)
                cout << "problem" << endl;*/

        }
    ctrl.end_game();
    cout<<"end"<<endl;
    }
    /*vector<string> v = Tools::StringSplitter::split("aaa"," ",2);
    for(int i = 0;i<v.size();i++)
        cout<<v[i]<<endl;*/
    return 0;
}

