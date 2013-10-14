#include <WinSock2.h>
#include "WebSocketPacket.h"
#include "Utils.h"


WebSocketPacket::WebSocketPacket() {
	flags_opcode = 0;
	mask_len = 0;
	exlen = 0;
	exlen2 = 0;
	mask_key[0] = 0;
	mask_key[1] = 0;
	mask_key[2] = 0;
	mask_key[3] = 0;	
	data = 0;
}

WebSocketPacket::~WebSocketPacket() {
	if (data)
		delete data;
}

void WebSocketPacket::setData(BYTE *d, unsigned long long len) {
	flags_opcode = 0x81; // text mode (0x82 for binary mode)
	if (len < 126) 
		mask_len = 0x80 | len;
	else if (len < 0xffff) {
		mask_len = 0x80 | 126;
		exlen = htons(len);
	} else {
		mask_len = 0x80 | 127;
		exlen2 = htonll(len);
	}
	mask_key[0] = rand() & 0xff;
	mask_key[1] = rand() & 0xff;
	mask_key[2] = rand() & 0xff;
	mask_key[3] = rand() & 0xff;

	data = new BYTE[len];
	for (int i=0; i<len; i++)
		data[i] = d[i] ^ mask_key[i & 3];
}

unsigned long long WebSocketPacket::getData(BYTE *d) {
	unsigned long long len = mask_len & 0x7f;
	if (len == 126)
		len = ntohs(exlen);
	else if (len == 127)
		len = ntohll(exlen2);

	if (d != 0) {
		for (int i=0; i<len; i++)
			d[i] = data[i] ^ mask_key[i & 3];
	}
	return len;
}

void WebSocketPacket::ping() {
	flags_opcode = 0x89;
}

void WebSocketPacket::pong() {
	flags_opcode = 0x8a;
}

void WebSocketPacket::close() {
	flags_opcode = 0x88;
}

WebSocketMessageType WebSocketPacket::getType() {
	switch (flags_opcode & 0xf) {
		case 0x1: return TextData;
		case 0x2: return BinaryData;
		case 0x8: return Close;
		case 0x9: return Ping;
		case 0xa: return Pong;
	}
}

void WebSocketPacket::sendTo(SOCKET s) {
	unsigned long long len = mask_len & 0x7f;
	
	send(s, (char*)&flags_opcode, 1, 0);
	send(s, (char*)&mask_len, 1, 0);
	if (len == 126) {
		send(s, (char*)&exlen, 2, 0);
		len = exlen;
	} else if (len == 127) {
		send(s, (char*)&exlen2, 8, 0);
		len = exlen2;
	}
	if (mask_len & 0x80)
		send(s, (char*)mask_key, 4, 0);
	if (len != 0)
		send(s, (char*)data, len, 0);
}

void WebSocketPacket::recvFrom(SOCKET s) {
	_recv(s, (char*)&flags_opcode, 1, 0);
	_recv(s, (char*)&mask_len, 1, 0);
	
	unsigned long long len = mask_len & 0x7f;
	if (len == 126) {
		_recv(s, (char*)&exlen, 2, 0);
		len = exlen;
	} else if (len == 127) {
		_recv(s, (char*)&exlen2, 8, 0);
		len = exlen2;
	}

	if (mask_len & 0x80)
		_recv(s, (char*)&mask_key, 4, 0);

	if (len != 0) {
		data = new BYTE[len];
		_recv(s, (char*)data, len, 0);
	}
}