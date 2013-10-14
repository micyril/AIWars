#include "Message.h"
#include <sstream>
using namespace std;
using namespace Messages;

Action::Action(ActionType t) {
	mtype = "action";
	switch (t) {
	case Start:
		action = "start"; break;
	case Win:
		action = "win"; break;
	case Loose:
		action = "loose"; break;
	}
}

string Action::Serialize() {
	stringstream ss;
	ss << "{"
		"\"mtype\":\"" << mtype << "\","
		"\"action\":\"" << action << "\"";
	ss << "}";
	return ss.str();
}