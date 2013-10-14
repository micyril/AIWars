#pragma once
#include <Windows.h>
#include <string>
using namespace std;

enum WebSocketMessageType {
	TextData = 1, 
	BinaryData = 2, 
	AnyData = TextData | BinaryData,
	Ping = 4, 
	Pong = 8, 
	Close = 16, 
	UnknownType = 0
};

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

	WebSocketMessageType getType();

	void sendTo(SOCKET s);
	void recvFrom(SOCKET s);
};
