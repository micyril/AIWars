#pragma once
#include "WebSocketPacket.h"
#include "Serializable.h"
#include <Windows.h>
#include <mutex>
#include <list>
using namespace std;

class Client {
private:
	mutex m;
	list<WebSocketPacket*> queue;
public:
	SOCKET s;
	int id;

	Client(SOCKET s, int id);
	~Client();

	void sendObjects(Serializable *o, int n);
	void sendObject(Serializable &o);
	void sendData(char *data, unsigned long long len);
	int reciveData(char **data);
	void close();
	void ping();
	void pong(WebSocketPacket *pp = 0);

	WebSocketPacket* recivePacket();
};

