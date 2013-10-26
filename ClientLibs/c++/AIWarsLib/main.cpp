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
            cout<<ctrl.move((float)i)<<" ";
            cout<<ctrl.fire()<<" ";
            cout<<ctrl.rotate((float)i)<<" ";
            vector<WorldObject> v = ctrl.scan();
            cout<<v[2].type<<endl;
            if (v.size() == 0)
                cout << "problem" << endl;

        }

    cout<<"end"<<endl;
    ctrl.end_game();
    }
    /*vector<string> v = Tools::StringSplitter::split("aaa"," ",2);
    for(int i = 0;i<v.size();i++)
        cout<<v[i]<<endl;*/
    return 0;
}

