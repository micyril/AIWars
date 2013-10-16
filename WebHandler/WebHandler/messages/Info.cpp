#include "Message.h"
#include <sstream>
using namespace std;
using namespace Messages;

Info::Info(bool self, int id) {
	mtype = self ? "self_info" : "enemy_info";
	this->id = id;
	w = 0;
}

Info::Info(Serializable *world) {
	mtype = "game_info";
	w = world;
}

string Info::Serialize() {
	stringstream ss;
	ss << "{"
		"\"mtype\":\"" << mtype << "\",";
	if (w == 0) {
		ss << "\"id\":" << id;
	} else {
		ss << "\"world\": " << w->Serialize();
	}
	ss << "}";
	return ss.str();
}