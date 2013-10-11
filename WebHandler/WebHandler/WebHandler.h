#pragma once
#include "HttpRequest.h"
#include "Client.h"
#include <Windows.h>
#include <list>

#define PORT 80
#define MAX_CONNECTIONS 100
#define RECV_TIMEOUT_SEC 5
#define SLEEP_TIME 10000

// здесь надо написать правильный путь к папке со страницей
#define HTML_ROOT "../../html" 
#define DEFAULT_PAGE "/index.html"

typedef void (*OnConnectCallBack)(Client*);

class WebHandler {
private:
	static TP_CALLBACK_ENVIRON pool_env;
	static PTP_POOL http_clients_pool;
	static HANDLE http_thread_handle;
	static HANDLE daemon_thread_handle;
	static bool daemon_alive;

	static void initPool();
	static DWORD WINAPI Listener(void* param);
	static VOID CALLBACK HttpRequestHandler(PTP_CALLBACK_INSTANCE Instance, PVOID param, PTP_WORK Work);
	static void websocketHandshake(SOCKET s, HttpRequest *r);
	static DWORD WINAPI WatchingDaemon(void* param);

	static DWORD id;
	static list<Client*> clients;
	static OnConnectCallBack on_connect;

	WebHandler() {};
	~WebHandler() {};
public:
	// запуск сервера
	static HANDLE StartHttp();
	// остановка сервера
	static void StopHttp();
	// назначить функцию, которыя будет вызываться при подклчении нового клиента
	static void setOnConnectCallback(OnConnectCallBack cb) {
		on_connect = cb;
	}
};
