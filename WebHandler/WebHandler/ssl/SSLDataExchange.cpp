// magic, do not touch this

#define STRICT
#include "ssl.h"
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Secur32.lib")

void SSLDataExchange(const char* data, int len, SOCKET ss, SOCKET s) {	
	SSL_SOCKET* ssl_socket = new SSL_SOCKET(ss, 0, 0);
	ssl_socket->ClientInit();
	ssl_socket->s_ssend((char*)data, len);

	char *bf = new char[1024];
	int n;	
	while ((n = ssl_socket->s_recv(bf, 1024)) > 0)
		::send(s, bf, n, 0);
	ssl_socket->ClientOff();
	delete[] bf;
}