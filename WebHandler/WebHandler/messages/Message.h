#pragma once
#include "..\Serializable.h"
#include <string>
using namespace std;

namespace Messages {

	enum ActionType {
		Start, Win, Loose
	};

	class Message : public Serializable {
	public:
		string mtype;

		virtual string Serialize() = 0;
	};

	class Info : public Message {
	public:
		int id;
	
		Info(bool self, int id);
		virtual std::string Serialize();
	};

	class Action : public Message {
	public:
		string action;

		Action(ActionType t);
		virtual std::string Serialize();
	};
}