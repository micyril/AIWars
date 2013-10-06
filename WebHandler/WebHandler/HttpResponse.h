#pragma once
#include <Windows.h>
#include <string>
#include <map>
using namespace std;

#define VERSION "HTTP/1.1"
#define RESPONSE_OK "200 OK"
#define SERVER "WebHandler (C++)"
#define CONTENT_TYPE_HTML "text/html; charset=utf-8"

class HttpResponse {
private:
	void init();
public:
	string version;
	string code;
	map<string, string> headers;
	string data;

	HttpResponse(const char *_code);
	HttpResponse(string &_code);
	~HttpResponse();

	void respond(SOCKET s);
};

