#pragma once
#include <Windows.h>
#include <string>
#include <exception>
using namespace std;

int readFile(char* file, char** data);
void writeToFile(const char* file, const char* data);
bool startsWith(string &str, string &prefix);
bool endsWith(string &str, string &suffix);
int parseInt(string &str);
string& toLowerCase(string& str);
string base64_encode(unsigned char const* s, unsigned int len);
string base64_decode(string const& s);
bool SHA1(const char *msg, unsigned char *hash);
string urlDecode(string &SRC);
// обертка над обычным recv но с выбрасыванием исключения при таймауте (или другой ошибке чтения)
int _recv(SOCKET s, char* buf, int len, int flags);

class SocketIOException : public exception {};

//===========================================================
//========== Http Errors ====================================
//===========================================================

class HttpException : public exception {
public:
	virtual const char* what() const throw() {
		return "500 Internal Server Error";
	}
};

class BadRequest : public HttpException {
public:
	virtual const char* what() const throw() {
		return "400 Bad Request";
	}
};

class Forbidden : public HttpException {
public:
	virtual const char* what() const throw() {
		return "403 Forbidden";
	}
};

class NotFound : public HttpException {
public:
	virtual const char* what() const throw() {
		return "404 Not Found";
	}
};

class InternalServerError : public HttpException {
public:
	virtual const char* what() const throw() {
		return "500 Internal Server Error";
	}
};

class NotImplemented : public HttpException {
public:
	virtual const char* what() const throw() {
		return "501 Not Implemented";
	}
};