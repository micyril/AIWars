#include "HttpResponse.h"


HttpResponse::HttpResponse(const char *_code) {
	init();
	code.assign(_code);
}

HttpResponse::HttpResponse(string &_code) {
	init();
	code = _code;
}

HttpResponse::~HttpResponse(){
}

void HttpResponse::init() {
	version = VERSION;
	headers["Server"] = SERVER;
}

void HttpResponse::respond(SOCKET s) {
	send(s, version.c_str(), version.length(), 0);
	send(s, " ", 1, 0);
	send(s, code.c_str(), code.length(), 0);
	send(s, "\r\n", 2, 0);

	for (map<string, string>::iterator i = headers.begin(); i != headers.end(); i++) {
		send(s, i->first.c_str(), i->first.length(), 0);
		send(s, ": ", 2, 0);
		send(s, i->second.c_str(), i->second.length(), 0);
		send(s, "\r\n", 2, 0);
	}
	send(s, "\r\n", 2, 0);
	send(s, data.c_str(), data.length(), 0);
}
