#pragma comment(lib, "wsock32.lib")
#include "WebHandler.h"
#include "Utils.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <time.h>
using namespace std;



WSADATA wd;
int _ = WSAStartup(0x202, &wd);


TP_CALLBACK_ENVIRON WebHandler::pool_env;
PTP_POOL WebHandler::http_clients_pool = 0;
HANDLE WebHandler::http_thread_handle = 0;
HANDLE WebHandler::daemon_thread_handle = 0;
bool WebHandler::daemon_alive;
DWORD WebHandler::id = 0;
list<Client*> WebHandler::clients;
OnConnectCallBack WebHandler::on_connect;


void WebHandler::initPool() {
	if (http_clients_pool == 0) {		
		srand(time(0));
		InitializeThreadpoolEnvironment(&pool_env);
		http_clients_pool = CreateThreadpool(0);		
		SetThreadpoolThreadMinimum(http_clients_pool, MAX_CONNECTIONS);
		SetThreadpoolThreadMaximum(http_clients_pool, MAX_CONNECTIONS);
		SetThreadpoolCallbackPool(&pool_env, http_clients_pool);
	}
}

HANDLE WebHandler::StartHttp() {
	if (http_thread_handle == 0) {
		initPool();
		ListenerParams *p = new ListenerParams();
		p->port = PORT;
		p->request_handler = &HttpRequestHandler;
		http_thread_handle = CreateThread(0, 0, &Listener, (void*)p, 0, 0);
	}
	if (daemon_thread_handle == 0) {
		daemon_alive = true;
		//daemon_thread_handle = CreateThread(0, 0, &WatchingDaemon, 0, 0, 0);
	}
	return http_thread_handle;
}

void WebHandler::StopHttp() {
	if (http_thread_handle != 0) {
		TerminateThread(http_thread_handle, -1);
		http_thread_handle = 0;
	}
	daemon_alive = false;
	for (list<Client*>::iterator i = clients.begin(); i != clients.end(); i++)
		delete *i;
}

DWORD WINAPI WebHandler::Listener(void* param) {
	ListenerParams *p = (ListenerParams*)param;
	int port = p->port;
	PTP_WORK_CALLBACK rh = p->request_handler;
	delete p;

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr  = INADDR_ANY; 
	addr.sin_port = htons(port);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (0 != ::bind(s, (sockaddr*)&addr, sizeof(addr))) {
		cerr << "bind failed: " << GetLastError() << endl;
		return -1;
	}
	if (0 != listen(s, SOMAXCONN)) {
		cerr << "listen failed: " << GetLastError() << endl;
		return -1;
	}

	while (1) {
		SOCKADDR_IN caddr;
		int l = sizeof(caddr);
		SOCKET client = accept(s, (sockaddr*)&caddr, &l);
		PTP_WORK work = CreateThreadpoolWork(rh, (void*)client, &pool_env);
		if (0 == work) {
			cerr << "can't create threadpool work" << endl;
			continue;
		}
		SubmitThreadpoolWork(work);
	}

	return 0;
}

VOID CALLBACK WebHandler::HttpRequestHandler(PTP_CALLBACK_INSTANCE Instance, PVOID param, PTP_WORK Work) {
	SOCKET s = (SOCKET)param;
	timeval tv;
	tv.tv_sec = RECV_TIMEOUT_SEC;
	tv.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof( timeval));
	
	HttpRequest *req = 0;
	HttpResponse *res = 0;
	char *data = 0;
	bool dontClose = false;
	try {
		req = new HttpRequest(s, HTML_ROOT, DEFAULT_PAGE);
		if (req->headers["Upgrade"].compare("websocket") == 0) {
			// работаем с веб сокетом
			websocketHandshake(s, req);
			Client *cl = new Client(s, id++);
			clients.push_back(cl);
			if (on_connect)
				on_connect(cl);
			dontClose = true;
		} else {
			// просто отдаем запрошенный файл
			int size = readFile((char*)req->path.c_str(), &data);
			res = new HttpResponse(RESPONSE_OK);
			
			if (endsWith(req->path, ".html"))
				res->headers["Content-Type"] = CONTENT_TYPE_HTML;
			else if (endsWith(req->path, ".jpg")) {
				res->headers["Content-Type"] = "image/jpeg";
			}

			res->data.assign(data, size);

			char buf[20];
			itoa(size, buf, 10);
			res->headers["Content-Length"] = buf;			
			res->respond(s);
		}
		
#ifdef _DEBUG
		cout << req->path << endl;
		for (auto i = req->_GET.begin(); i != req->_GET.end(); i++)
			cout << i->first << "\t" << i->second << endl;
#endif
	} catch (HttpException &e) {
		res = new HttpResponse(e.what());
		res->respond(s);
	} catch (exception &e) {
		cerr << e.what() << endl;
		res = new HttpResponse("500 Internal Server Error");
		res->respond(s);
	}

	if (req)
		delete req;
	if (res)
		delete res;
	if (data)
		delete[] data;

	if (!dontClose)
		closesocket(s);	
}

void WebHandler::websocketHandshake(SOCKET s, HttpRequest *r) {
#ifdef _DEBUG
	cout << "\n WS: \n";
	cout << r->path << endl;
	for (auto i = r->_GET.begin(); i != r->_GET.end(); i++)
		cout << i->first << "\t" << i->second << endl;
	for (auto i = r->headers.begin(); i != r->headers.end(); i++)
		cout << i->first << "\t" << i->second << endl;
	cout << r->data;
#endif

	if (parseInt(r->headers["Sec-WebSocket-Version"]) < 13) 
		throw NotImplemented();

	string key(r->headers["Sec-WebSocket-Key"]);
	key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	BYTE hash[20];
	SHA1(key.c_str(), hash);

	HttpResponse *res = new HttpResponse("101 Switching Protocols");
	res->headers["Upgrade"] = "websocket";
	res->headers["Connection"] = "Upgrade";
	res->headers["Sec-WebSocket-Accept"] = base64_encode(hash, 20);

	res->respond(s);
}

// отключено
DWORD WINAPI WebHandler::WatchingDaemon(void* param) {
	while (daemon_alive) {
		for (auto i = clients.begin(); i != clients.end(); i++) {
			// здесь надо пинговать клиентов и отвечать на пинги - не отвечающих выпиливать
		}
		Sleep(SLEEP_TIME);
	}
	return 0;
}
