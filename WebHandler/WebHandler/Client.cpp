#include "Client.h"
#include "WebSocketPacket.h"
#include "Utils.h"


Client::Client(SOCKET s, int id) {
	this->s = s;
	this->id = id;
}


Client::~Client() {	
	close();
	m.lock();
	for (auto i = queue.begin(); i != queue.end(); i++)
		delete *i;
	m.unlock();
}

void Client::sendData(char *data, unsigned long long len) {
	m.lock();
	WebSocketPacket p;
	p.setData((BYTE*)data, len);
	p.sendTo(s);
	m.unlock();
}

void Client::close() {
	m.lock();
	WebSocketPacket p;
	p.close();
	p.sendTo(s);
	char c;
	_recv(s, &c, 1, 0); // костыль
	closesocket(s);
	m.unlock();
}

WebSocketPacket* Client::recivePacket() {
	m.lock();
	WebSocketPacket *p = new WebSocketPacket();
	p->recvFrom(s);
	m.unlock();
	return p;
}

int Client::reciveData(char **data) {
	WebSocketPacket *p;
	if (queue.empty()) 
		p = recivePacket();
	else {
		m.lock();
		p = *(queue.begin());
		queue.pop_front();
		m.unlock();
	}
	int len = p->getData(0);
	*data = new char[len];
	p->getData((BYTE*)*data);
	delete p;
	return len;
}

void Client::ping() {
	m.lock();
	WebSocketPacket p;
	p.ping();
	p.sendTo(s);
	m.unlock();
}

void Client::pong(WebSocketPacket *pp) {
	m.lock();
	if (pp == 0) {
		WebSocketPacket p;
		p.pong();
		p.sendTo(s);
	} else 
		pp->sendTo(s);
	m.unlock();
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