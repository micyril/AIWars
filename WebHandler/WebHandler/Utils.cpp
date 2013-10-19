#pragma comment(lib, "Crypt32.lib")
#include "WebHandler.h"
#include "Utils.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include <algorithm>
#ifdef _DEBUG
#include <iostream>
#endif
using namespace std;

int readFile(char* file, char** data) {
	DWORD size = MultiByteToWideChar(CP_UTF8, 0, file, -1, 0, 0);
	wchar_t* wfile = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, file, -1, wfile, size);

	HANDLE f = CreateFile(wfile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (f == INVALID_HANDLE_VALUE) {
#ifdef _DEBUG
		cerr << "can't open file " << file << endl;
#endif
		delete[] wfile;
		throw NotFound();
	}
	size = GetFileSize(f, 0);
	*data = new char[size + 1];
	ReadFile(f, (void*)*data, size, &size, 0);
	(*data)[size] = '\0';
	CloseHandle(f);

	delete[] wfile;

	return size;
}

void writeToFile(const char* file, const char* data) {
	DWORD size = MultiByteToWideChar(CP_UTF8, 0, file, -1, 0, 0);
	wchar_t* wfile = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, file, -1, wfile, size);

	HANDLE f = CreateFile(wfile, FILE_APPEND_DATA, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (f == INVALID_HANDLE_VALUE) {
#ifdef _DEBUG
		cerr << "can't open file " << file << " for logging" << endl;
#endif
		delete[] wfile;
		return;
	}
	DWORD len;
	WriteFile(f, (void*)data, strlen(data), &len, 0);
	CloseHandle(f);
	delete[] wfile;
}

bool startsWith(string str, string prefix) {
	if (str.length() < prefix.length()) 
		return false;
	for (int i=0; i<prefix.length(); i++)
		if (str[i] != prefix[i])
			return false;
	return true;
}

bool endsWith(string str, string suffix) {
	if (str.length() < suffix.length()) 
		return false;
	for (int i=1; i<=suffix.length(); i++)
		if (str[str.length() - i] != suffix[suffix.length() - i])
			return false;
	return true;
}

int parseInt(string str) {
	int value;
	stringstream ss(str);
	ss >> value;
	if (ss.fail())
		throw BadRequest();
	return value;
}

bool SHA1(const char *msg, unsigned char *hash) {
	HCRYPTPROV _prov;
    HCRYPTHASH _hash;
    DWORD _size = 20;
	bool ok = false;

	if (CryptAcquireContext(&_prov, NULL, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        if (CryptCreateHash(_prov, CALG_SHA1, 0, 0, &_hash)) {
            if (CryptHashData(_hash, (BYTE*)msg, strlen(msg), 0))
                if (CryptGetHashParam(_hash, HP_HASHVAL, (BYTE*)hash, &_size, 0))
					ok = true;
            CryptDestroyHash(_hash);
        }
        CryptReleaseContext(_prov, 0);
    }
 
    return ok;
}

string urlDecode(string &SRC) {
    string ret;
    char ch;
    int i, ii;
    for (i=0; i<SRC.length(); i++) {
        if (int(SRC[i])==37) {
            sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
            ch=static_cast<char>(ii);
            ret+=ch;
            i=i+2;
        } else {
            ret+=SRC[i];
        }
    }
    return (ret);
}

int _recv(SOCKET s, char* buf, int len, int flags) {
	int b = recv(s, buf, len, flags);
	if (b < 0)
		throw SocketIOException();
	return b;
}

string& toLowerCase(string& str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}