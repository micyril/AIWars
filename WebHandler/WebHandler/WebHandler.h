#pragma once
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Client.h"
#include <Windows.h>
#include <list>
#include <map>
#include <mutex>
using namespace std;

#define DEFAULT_CONFIG "config.ini"

typedef void (*OnConnectCallBack)(Client*);

class WebHandler {
private:
	// settings

	static int PORT;
	static int MAX_CONNECTIONS;
	static int RECV_TIMEOUT_SEC;
	static string IP;
	static string HTML_ROOT;
	static string DEFAULT_PAGE;
	static string LOGFILE;
	static map<string, string> mime;

	// threads, pools, etc...

	static TP_CALLBACK_ENVIRON pool_env;
	static PTP_POOL http_clients_pool;
	static HANDLE http_thread_handle;
	static mutex log_mutex;

	// accept loop
	static DWORD WINAPI Listener(void* param);
	static VOID CALLBACK HttpRequestHandler(PTP_CALLBACK_INSTANCE Instance, PVOID param, PTP_WORK Work);
	static HttpResponse* websocketHandshake(HttpRequest *r);
	static void initPool();
	static void log(HttpRequest *req, HttpResponse *res, SOCKET s, SOCKADDR_IN addr);

	static DWORD id;
	static list<Client*> clients;
	static OnConnectCallBack on_connect;

	WebHandler() {};
	~WebHandler() {};
public:
	// запуск сервера
	static HANDLE StartHttp(char* cfg = 0);
	// остановка сервера
	static void StopHttp();
	// назначить функцию, которыя будет вызываться при подключении нового клиента
	static void setOnConnectCallback(OnConnectCallBack cb) {
		on_connect = cb;
	}
};
