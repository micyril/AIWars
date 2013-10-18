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

int WebHandler::PORT = 80;
int WebHandler::MAX_CONNECTIONS = 100;
int WebHandler::RECV_TIMEOUT_SEC = 5;
char* WebHandler::HTML_ROOT = ".";
char* WebHandler::DEFAULT_PAGE = "/index.html";
char* WebHandler::LOGFILE = 0;
map<string, string> WebHandler::mime;

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

HANDLE WebHandler::StartHttp(char *cfg) {
	// парсим конфиг
	try {
		char *data;
		readFile(cfg ? cfg : DEFAULT_CONFIG, &data);
		char *p = strtok(data, "=\r\n");
		char *t;
		while (p) {
			if (*p == '#') {/* комментарии */}
			else if (strcmp(p, "PORT") == 0) {
				PORT = atoi(strtok(0, "\r\n"));
			} else if (strcmp(p, "MAX_CONNECTIONS") == 0) {
				MAX_CONNECTIONS = atoi(strtok(0, "\r\n"));
			} else if (strcmp(p, "RECV_TIMEOUT_SEC") == 0) {
				RECV_TIMEOUT_SEC = atoi(strtok(0, "\r\n"));
			} else if (strcmp(p, "HTML_ROOT") == 0) {
				t = strtok(0, "\r\n");
				HTML_ROOT = new char[strlen(t)];
				strcpy(HTML_ROOT, t);
			} else if (strcmp(p, "DEFAULT_PAGE") == 0) {
				t = strtok(0, "\r\n");
				DEFAULT_PAGE = new char[strlen(t)];
				strcpy(DEFAULT_PAGE, t);
			} else if (strcmp(p, "LOGFILE") == 0) {
				t = strtok(0, "\r\n");
				LOGFILE = new char[strlen(t)];
				strcpy(LOGFILE, t);
			} else {
				mime[string(p)] = string(strtok(0, "\r\n"));
			}
			p = strtok(0, "\r\n=");
		}
		delete[] data;
	} catch (NotFound &e) {}

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
	bool ws = false;
	try {
		req = new HttpRequest(s, HTML_ROOT, DEFAULT_PAGE);
		if (req->headers["Upgrade"].compare("websocket") == 0) {
			res = websocketHandshake(req);
			ws = true;
		} else {
			// отдаем запрошенный файл
			int size = readFile((char*)req->path.c_str(), &data);
			res = new HttpResponse(RESPONSE_OK);
			// муть какая то эти постоянные соединения
			res->headers["Connection"] = "close";

			const char *ext = strrchr(req->path.c_str(), '.');
			if (ext)
				res->headers["Content-Type"] = mime[string(++ext)];

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
	} catch (exception &e) {
		cerr << e.what() << endl;
		res = new HttpResponse("500 Internal Server Error");
	}

	res->respond(s);
	log(req, res, s);

	if (req)
		delete req;
	if (res)
		delete res;
	if (data)
		delete[] data;

	if (ws) {
		Client *cl = new Client(s, id++);
		clients.push_back(cl);
		if (on_connect)
			on_connect(cl);
	} else 
		closesocket(s);	
}

HttpResponse* WebHandler::websocketHandshake(HttpRequest *r) {
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

	return res;
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

void WebHandler::log(HttpRequest *req, HttpResponse *res, SOCKET s) {
	if (LOGFILE) {		
		string log;

		time_t now = time(0);
		log.assign(ctime(&now));
		if (log[log.length() - 1] == '\n')
			log.erase(log.length() - 1);
		log.append(" ");

		SOCKADDR_IN addr;
		int len = sizeof(addr);
		getsockname(s, (sockaddr*)&addr, &len);
		log.append(inet_ntoa(addr.sin_addr));
		log.append(" ");

		switch (req->method) {
			case GET:
				log.append("GET");
				break;
			case POST:
				log.append("POST");
				break;
		}
		log.append(" ");

		log.append(req->path);
		if (req->_GET.size()) {
			log.append("?");
			auto i = req->_GET.begin();
			log.append(i->first);
			log.append("=");
			log.append(i->second);
			for (i++; i != req->_GET.end(); i++) {
				log.append("&");
				log.append(i->first);
				log.append("=");
				log.append(i->second);
			}
		}
		log.append(" ");

		char buf[5];
		log.append(itoa(PORT, buf, 10));

		log.append(" \"");
		log.append(req->headers["User-Agent"]);
		log.append("\" ");

		log.append(res->code);
		log.append("\r\n");
		
		writeToFile(LOGFILE, log.c_str());
	}
}