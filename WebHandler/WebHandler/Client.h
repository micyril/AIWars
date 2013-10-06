#pragma once
#include <Windows.h>

class Client {
public:
	SOCKET s;
	int id;

	Client(SOCKET s, int id);
	~Client();

	void sendData(char *data, unsigned long long len);
	void close();
};

