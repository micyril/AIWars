#include "Message.h"
#include <sstream>
using namespace std;
using namespace Messages;

Action::Action() {
	mtype = "action";
	action = "start";
	l = 0;
	id  = -1;
}

Action::Action(list<Serializable*> *list) {
	mtype = "action";
	action = "update";
	l = list;
	id  = -1;
}

Action::Action(int winid) {
	mtype = "action";
	action = "finish";
	l = 0;
	id = winid;
}

string Action::Serialize() {
	stringstream ss;
	ss << "{"
		"\"mtype\":\"" << mtype << "\"," << 
		"\"action\":\"" << action << "\"" ;
	if (l != 0) {
		ss << ",\"mapelements\": [";
		auto i = l->begin();
		ss << (*i)->Serialize();
		for (i++; i != l->end(); i++)
			ss << "," << (*i)->Serialize();
		ss << "]";
	}
	if (id != -1)  {
		ss << ",\"winner\": " << id;
	}
	ss << "}";
	return ss.str();
}