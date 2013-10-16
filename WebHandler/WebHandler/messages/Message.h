#pragma once
#include "..\Serializable.h"
#include <string>
#include <list>
using namespace std;

namespace Messages {

	class Message : public Serializable {
	protected:
		string mtype;
	public:
		virtual string Serialize() = 0;
	};

	class Info : public Message {
	protected:
		int id;
		Serializable *w;
	public:		
		// self or enemy info
		Info(bool self, int id);
		// game info
		Info(Serializable *world);
		virtual std::string Serialize();
	};

	class Action : public Message {
	protected:
		string action;
		list<Serializable*> *l;
		int id;
	public:		

		// start action
		Action();
		// update action
		Action(list<Serializable*> *l);
		// finish action
		Action(int winid);
		virtual std::string Serialize();
	};
}