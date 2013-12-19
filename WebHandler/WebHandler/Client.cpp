#include "Client.h"
#include "WebSocketPacket.h"
#include "Utils.h"
#include "messages\Message.h"
#include <sstream>
using namespace Messages;

Client::Client(SOCKET s, int id) {
	this->s = s;
	this->id = id;
	this->mutex = CreateMutex(0, false, 0);
}

Client::~Client() {	
	close();
}

void Client::sendData(char *data, unsigned long long len) {
	WaitForSingleObject(mutex, INFINITE);
	WebSocketPacket p;

	p.setData((BYTE*)data, len);
	p.sendTo(s);
	ReleaseMutex(mutex);
}

int Client::reciveData(char **data) {
	WebSocketPacket *p;
	p = recivePacket();
	int len = p->getData(0);
	*data = new char[len];
	p->getData((BYTE*)*data);
	delete p;
	return len;
}

void Client::close() {
	WaitForSingleObject(mutex, INFINITE);
	WebSocketPacket p;
	p.close();
	p.sendTo(s);
	char c;
	_recv(s, &c, 1, 0); // костыль
	closesocket(s);
	ReleaseMutex(mutex);
}

void Client::ping() {
	WaitForSingleObject(mutex, INFINITE);
	WebSocketPacket p;
	p.ping();
	p.sendTo(s);
	ReleaseMutex(mutex);
}

void Client::pong(WebSocketPacket *pp) {
	WaitForSingleObject(mutex, INFINITE);
	if (pp == 0) {
		WebSocketPacket p;
		p.pong();
		p.sendTo(s);
	} else 
		pp->sendTo(s);
	ReleaseMutex(mutex);
}

WebSocketPacket* Client::recivePacket() {
	WaitForSingleObject(mutex, INFINITE);
	WebSocketPacket *p = new WebSocketPacket();
	p->recvFrom(s);
	ReleaseMutex(mutex);
	return p;
}

void Client::sendPacket(WebSocketPacket* p) {
	p->sendTo(s);
}

void Client::sendObject(Serializable &o) {
	string s = o.Serialize();
	sendData((char*)s.c_str(), s.length());
}

void Client::sendObjects(Serializable *o, int n) {
	string s = "[";
	for (int i=0; i<n; i++) {
		s.append(o[n].Serialize());
		s.append(",");
	}
	s[s.length() - 1] = ']';
	sendData((char*)s.c_str(), s.length());
}

void Client::sendObjects(list<Serializable*> *l) {
	string s = "[";
	for (auto i = l->begin(); i != l->end(); i++) {
		s.append((*i)->Serialize());
		s.append(",");
	}
	s[s.length() - 1] = ']';
	sendData((char*)s.c_str(), s.length());
}

void Client::sendSelfInfo() {
	sendObject(Info(true, id));
}

void Client::sendEnemyInfo(int _id) {
	enemy_id = _id;
	sendObject(Info(false, _id));
}

void Client::sendGameInfo(Serializable *w) {
	sendObject(Info(w));
}

void Client::notifyStart() {
	sendObject(Action());
}

void Client::notifyUpdate(list<Serializable*> *l) {
	sendObject(Action(l));
}

void Client::notifyFinish(int id) {
	sendObject(Action(id));
}