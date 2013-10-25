#include "HttpRequest.h"
#include "Utils.h"
#include <string>
#include <exception>
#include <sstream>
using namespace std;


HttpRequest::HttpRequest(string hr, string dp) {
	init(hr, dp);
}

HttpRequest::HttpRequest(SOCKET s, string hr, string dp) {
	init(hr, dp);
	parse(s);
}

HttpRequest::~HttpRequest(void) {
}

void HttpRequest::init(string hr, string dp) {
	int i=0;
	state_map[i++] = &HttpRequest::processMethod;
	state_map[i++] = &HttpRequest::processRequest;
	state_map[i++] = &HttpRequest::processVersion;
	state_map[i++] = &HttpRequest::processNewLine;
	state_map[i++] = &HttpRequest::processHeaderName;
	state_map[i++] = &HttpRequest::processHeaderValue;
	state_map[i++] = &HttpRequest::processData;
	html_root.assign(hr);
	default_page.assign(dp);
}

void HttpRequest::parse(SOCKET s) {
	state = ParserState::Method;
	NLcount = 0;
	length = -1;
	stop = false;
	buf.clear();
	temp.clear();

	path.clear();
	_GET.clear();
	data.clear();
	headers.clear();

	char c;
	int n = 1;
	while (!stop && length != 0 && n != 0) {
		n = _recv(s, &c, 1, 0);
		if (n < 0)
			throw InternalServerError();
		if (c != '\r')
			(this->*state_map[state])(c);		
	}

}

void HttpRequest::processMethod(char c) {
	switch (c) {
		case ' ':
			if (buf.length() == 0)
				throw BadRequest();
			if (buf.compare("GET") == 0)
				method = MethodType::GET;
			else if (buf.compare("POST") == 0)
				method = MethodType::POST;
			else 
				throw NotImplemented();
			buf.clear();
			state = ParserState::Request;
			break;
		case '\n':
		case ':':
			throw BadRequest();
		default:
			buf.append(1, c);
			break;
	}
}

void HttpRequest::processRequest(char c) {
	switch (c) {
		case ' ':
			if (buf.length() == 0)
				throw BadRequest();
			if (buf.find("/../") != string::npos) 
				throw Forbidden();
			temp.append(html_root);			
			temp.append(buf);
			if (buf[buf.length() - 1] == '/')
				temp.append(default_page);

			parseRequest(temp);
			temp.clear();
			state = ParserState::Version;
			break;
		case '\n':
			throw BadRequest();
		default:
			buf.append(1, c);
			break;
	}
}

void HttpRequest::processVersion(char c) {
	switch (c) {
		case ' ':
			throw BadRequest();
		case '\n':
			buf.clear();
			state = ParserState::NewLine;
			break;
		default:
			buf.append(1, c);
			break;
	}
}

void HttpRequest::processNewLine(char c) {
	NLcount++;
	switch (c) {
		case ' ':
		case ':':
			throw BadRequest();
		case '\n':
			if (method == MethodType::GET)
				stop = true;
			break;
		default:
			buf.append(1, c);
			if (NLcount == 1) {
				NLcount = 0;
				state = ParserState::HeaderName;
			} else {
				if (method == MethodType::POST) {
					length = parseInt(headers["Content-Length"]) - 1;
					state = ParserState::Data;
				}
			}
			break;
	}
}

void HttpRequest::processHeaderName(char c) {
	switch (c) {
		case ' ':
		case '\n':
			throw BadRequest();
		case ':':
			if (buf.length() == 0)
				throw BadRequest();
			temp = buf;
			buf.clear();
			state = ParserState::HeaderValue;
			break;
		default:
			buf.append(1, c);
			break;
	}
}

void HttpRequest::processHeaderValue(char c) {
	switch (c) {
		case ' ':
			if (buf.length() != 0)
				buf.append(1, c);
			break;
		case '\n':
			if (buf.length() == 0 || temp.length() == 0)
				throw BadRequest();
			headers[temp] = buf;
			temp.clear();
			buf.clear();
			state = ParserState::NewLine;
			break;
		default:
			buf.append(1, c);
			break;
	}
}

void HttpRequest::processData(char c) {
	buf.append(1, c);
	length--;
}

void HttpRequest::parseRequest(string request) {
	string s = urlDecode(request);

	char *r = (char*)s.c_str();
	char *p = strtok(r, "?");
	path.append(p);
	
	char *k, *v;
	while (1) {
		k = strtok(0, "=");
		v = strtok(0, "&");
		if (!k || !v) 
			break;
		_GET[string(k)] = string(v);
	}
}