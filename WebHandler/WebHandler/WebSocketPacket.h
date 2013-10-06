#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class WebSocketPacket {
private:
	BYTE flags_opcode;
	BYTE mask_len;	
	WORD exlen;
	unsigned long long exlen2;
	BYTE mask_key[4];
	BYTE *data;

public:
	WebSocketPacket();
	~WebSocketPacket();

	// данные из d будут скопированы
	void setData(BYTE *d, unsigned long long len);
	// просто вернет длину если d == 0
	unsigned long long getData(BYTE *d);
	
	void ping();
	void pong();
	void close();

	void sendTo(SOCKET s);
};