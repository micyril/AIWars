#pragma comment(lib, "wsock32.lib")
#include <Windows.h>
#include <string>
#include <iostream>
#include <time.h>
#include "WebHandler.h"
#include "Utils.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
using namespace std;

WSADATA wd;
int _ = WSAStartup(0x202, &wd);

int WebHandler::PORT = 80;
int WebHandler::MAX_CONNECTIONS = 100;
int WebHandler::RECV_TIMEOUT_SEC = 5;
string WebHandler::IP = "0.0.0.0";
string WebHandler::HTML_ROOT = ".";
string WebHandler::DEFAULT_PAGE = "index.html";
string WebHandler::LOGFILE;
map<string, string> WebHandler::mime;

TP_CALLBACK_ENVIRON WebHandler::pool_env;
PTP_POOL WebHandler::http_clients_pool = 0;
HANDLE WebHandler::http_thread_handle = 0;
DWORD WebHandler::id = 0;
list<Client*> WebHandler::clients;
OnConnectCallBack WebHandler::on_connect;
HANDLE WebHandler::log_mutex = CreateMutex(0, false, 0);


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
				HTML_ROOT.assign(strtok(0, "\r\n"));
			} else if (strcmp(p, "DEFAULT_PAGE") == 0) {
				DEFAULT_PAGE.assign(strtok(0, "\r\n"));
			} else if (strcmp(p, "LOGFILE") == 0) {
				LOGFILE.assign(strtok(0, "\r\n"));
			} else if (strcmp(p, "IP") == 0) {
				IP.assign(strtok(0, "\r\n"));
			} else {
				mime[toLowerCase(string(p))] = string(strtok(0, "\r\n"));
			}
			p = strtok(0, "\r\n=");
		}
		delete[] data;
	} catch (NotFound &e) {}

	if (http_thread_handle == 0) {
		initPool();
		http_thread_handle = CreateThread(0, 0, &Listener, 0, 0, 0);
	}
	return http_thread_handle;
}

void WebHandler::StopHttp() {
	if (http_thread_handle != 0) {
		TerminateThread(http_thread_handle, -1);
		http_thread_handle = 0;
	}
	for (list<Client*>::iterator i = clients.begin(); i != clients.end(); i++)
		delete *i;
}

DWORD WINAPI WebHandler::Listener(void* param) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP.c_str()); // INADDR_ANY; 
	addr.sin_port = htons(PORT);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (0 != ::bind(s, (sockaddr*)&addr, sizeof(addr))) {
		cerr << "bind failed: " << WSAGetLastError() << endl;
		return 0;
	}
	if (0 != listen(s, SOMAXCONN)) {
		cerr << "listen failed: " << WSAGetLastError() << endl;
		return 0;
	}

	while (1) {
		auto p  = new pair<SOCKET, SOCKADDR_IN>();
		int l = sizeof(p->second);
		p->first = accept(s, (sockaddr*)&(p->second), &l);		
		PTP_WORK work = CreateThreadpoolWork(&HttpRequestHandler, (void*)p, &pool_env);
		if (0 == work) {
			cerr << "can't create threadpool work" << endl;
			continue;
		}
		SubmitThreadpoolWork(work);
	}

	return 0;
}

VOID CALLBACK WebHandler::HttpRequestHandler(PTP_CALLBACK_INSTANCE Instance, PVOID param, PTP_WORK Work) {
	auto p = (pair<SOCKET, SOCKADDR_IN>*)param;
	SOCKET s = p->first;
	SOCKADDR_IN addr = p->second;
	delete p;
	timeval tv;
	tv.tv_sec = RECV_TIMEOUT_SEC;
	tv.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(timeval));
	
	HttpRequest *req = 0;
	HttpResponse *res = 0;
	char *data = 0;
	bool ws = false;
	try {
		req = new HttpRequest(s, HTML_ROOT, DEFAULT_PAGE);
		if (req->headers["Upgrade"].compare("websocket") == 0) {
			// переходим на веб-сокет
			res = websocketHandshake(req);
			ws = true;
		} else {
			// отдаем запрошенный файл
			int size = readFile((char*)req->path.c_str(), &data);
			res = new HttpResponse(RESPONSE_OK);
			res->headers["Connection"] = "close";

			const char *ext = strrchr(req->path.c_str(), '.');
			if (ext) 
				res->headers["Content-Type"] = mime[toLowerCase(string(++ext))];

			res->data.assign(data, size);
			char buf[20];
			itoa(size, buf, 10);
			res->headers["Content-Length"] = buf;			
		}
		
#ifdef _DEBUG
		cout << req->path << endl;
#endif
	} catch (HttpException &e) {
		res = new HttpResponse(e.what());
	} catch (SocketIOException &e) {
		// насрать
	} catch (exception &e) {
		cerr << e.what() << endl;
		res = new HttpResponse("500 Internal Server Error");
		char buf[20];
		res->data.assign(itoa(GetLastError(), buf, 10));
	}

	
	if (res)
		res->respond(s);
	log(req, res, s, addr);

	if (req)
		delete req;
	if (res)
		delete res;
	if (data)
		delete[] data;

	if (ws) {
		Client *cl = new Client(s, id++);
#ifdef _DEBUG
		cout << "Client created (" << cl << ") with socket " << s << endl;
#endif
		clients.push_back(cl);
		if (on_connect)
			on_connect(cl);
	} else 
		closesocket(s);	
}

HttpResponse* WebHandler::websocketHandshake(HttpRequest *r) {
#ifdef _DEBUG
	cout << "WS\n";
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

void WebHandler::log(HttpRequest *req, HttpResponse *res, SOCKET s, SOCKADDR_IN addr) {
	if (!LOGFILE.empty()) {		
		string log;

		time_t now = time(0);
		log.assign(ctime(&now));
		if (log[log.length() - 1] == '\n')
			log.erase(log.length() - 1);
		log.append(" ");

		log.append(inet_ntoa(addr.sin_addr));
		log.append(" ");
		if (req) {
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
		}
		
		if (res)
			log.append(res->code);
		log.append("\r\n");
		
		WaitForSingleObject(log_mutex, INFINITE);
		writeToFile(LOGFILE.c_str(), log.c_str());
		ReleaseMutex(log_mutex);
	}
}
