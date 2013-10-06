#include "Client.h"
#include "WebSocketPacket.h"


Client::Client(SOCKET s, int id) {
	this->s = s;
	this->id = id;
}


Client::~Client() {
	close();
}

void Client::sendData(char *data, unsigned long long len) {
	WebSocketPacket p;
	p.setData((BYTE*)data, len);
	p.sendTo(s);
}

void Client::close() {
	WebSocketPacket p;
	p.close();
	p.sendTo(s);
	char c;
	recv(s, &c, 1, 0); // костыль
	closesocket(s);
}
