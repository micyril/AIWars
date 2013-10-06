#pragma once
#include <Windows.h>
#include <string>
#include <map>
using namespace std;

// POST запросы не тестировались

enum MethodType {
	GET, POST
};

enum ParserState {
	Method, Request, Version, NewLine, HeaderName, HeaderValue, Data
};

class HttpRequest {
private:
	typedef void (HttpRequest::*processState)(char c);

	string html_root;
	string default_page;

	ParserState state;
	string buf;
	string temp;
	int length;
	int NLcount;
	bool stop;
	processState state_map[7];

	void init(string hr, string dp);

	void processMethod(char c);
	void processRequest(char c);
	void processVersion(char c);
	void processNewLine(char c);
	void processHeaderName(char c);
	void processHeaderValue(char c);
	void processData(char c);

	void parseRequest(string s);

public:
	MethodType method;
	map<string, string> headers;
	string data;
	string path;
	map<string, string> _GET;

	HttpRequest(string hr, string dp);
	HttpRequest(SOCKET s, string hr, string dp);
	~HttpRequest();

	void parse(SOCKET s);
};

