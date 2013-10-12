#include "Message.h"
#include <sstream>
using namespace std;
using namespace Messages;

Info::Info(bool self, int id) {
	mtype = self ? "self_info" : "enemy_info";
	this->id = id;
}

string Info::Serialize() {
	stringstream ss;
	ss << "{"
		"\"mtype\":\"" << mtype << "\","
		"\"id\":" << id;
	ss << "}";
	return ss.str();
}