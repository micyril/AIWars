#pragma once
#include "HttpRequest.h"
#include "Client.h"
#include <Windows.h>
#include <list>

#define PORT 80
#define MAX_CONNECTIONS 100

// ����� ���� �������� ���������� ���� � ����� �� ���������
#define HTML_ROOT "../../html" 
#define DEFAULT_PAGE "/index.html"

typedef void (*OnConnectCallBack)(Client*);

class WebHandler {
private:
	static TP_CALLBACK_ENVIRON pool_env;
	static PTP_POOL http_clients_pool;
	static HANDLE http_thread_handle;

	static void initPool();
	static DWORD WINAPI Listener(void* param);
	static VOID CALLBACK HttpRequestHandler(PTP_CALLBACK_INSTANCE Instance, PVOID param, PTP_WORK Work);
	static void websocketHandshake(SOCKET s, HttpRequest *r);

	static DWORD id;
	static list<Client*> clients;
	static OnConnectCallBack on_connect;

	WebHandler() {};
	~WebHandler() {};
public:
	// ������ �������
	static HANDLE StartHttp();
	// ��������� �������
	static void StopHttp();
	// �������� ������ ����������� �������� � �� id
	static list<Client*>& getClients();

	static void setOnConnectCallback(OnConnectCallBack cb) {
		on_connect = cb;
	}
};
