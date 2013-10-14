#include <iostream>
#include "tools.h"
#include "commands.h"
using namespace std;
using namespace Commands;

int main()
{
    string s = "ACK RET BIG 1.1 1.2 WALL 1.01 1.9 AAA 1.2 1.2";
    ScanCommand sc;
    //s = mv.produce_request(666,10.1);
    pair< int,vector<Obstacle> > v = sc.parse_response(s);
    cout<<v.second[0].type<<endl;
    return 0;
}

