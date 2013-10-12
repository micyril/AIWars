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
	SOCKET s;

public:	
	int id;

	Client(SOCKET s, int id);
	~Client();

	// text level api

	void sendData(char *data, unsigned long long len);
	int reciveData(char **data);
	void close();
	void ping();
	void pong(WebSocketPacket *pp = 0);

	// packet level api

	WebSocketPacket* recivePacket();
	void sendPacket(WebSocketPacket *p);

	// object level api

	void sendObject(Serializable &o);
	void sendObjects(Serializable *o, int n);

	// game message level api

	void sendSelfInfo();
	void sendEnemyInfo(int id);
	void notifyStart();
};
