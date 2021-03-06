#include <iostream>
#include "controller.h"
#include "time.h"
using namespace std;
using namespace Commands;

int main(int argc, char *argv[])
{
    if(argc < 3){
        cerr << "Program args error! Try : programname <HOST IP> <HOST PORT> <CLIENT ID>"<<endl;
        return 1;
    }
    string IP(argv[1]);
    string PORT(argv[2]);
    string ID(argv[3]);

    Controller ctrl(IP,PORT,ID);
    if(ctrl.ready()){
        cout<<"start"<<endl;
        for(int i = 0; i < 1000; i++){
            cout<<ctrl.move(0.1)<<" ";
            cout<<ctrl.fire()<<" ";
            cout<<ctrl.rotate(10)<<" ";
            ScanCommandAnswer answ = ctrl.scan();
            if (answ.scannedWorldObjects.size() == 0)
                cout << "problem" << endl;
            else{
                cout<<answ.scannedWorldObjects[2].type<<endl;
            }

        }
    ctrl.end_game();
    cout<<"end"<<endl;
    }


    /*vector<string> v = Tools::StringSplitter::split("ACK aaa bbb ccc",' ',2);
    for(int i = 0;i<v.size();i++)
        cout<<v[i]<<endl;*/
    return 0;
}

