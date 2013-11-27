#include "commutator.h"

std::string Commutator::end_line = "\r\n";

Commutator::Commutator(std::string hostname,std::string port)
{
    this->port = atoi(port.c_str());
    this->host = hostname;
    this->connected = false;
    this->buffer_size = 1024;
    this->buffer = new char[this->buffer_size];
}

void Commutator::send_all(std::string msg){
	msg += this->end_line;
    size_t i = 0;
    size_t msg_len = msg.length();
    const char * raw_msg = msg.c_str();
    for(i = 0; i< msg_len; i+=send(this->sock,raw_msg,msg_len - i,0));
}

std::string Commutator::recv_all(){
    size_t i = 0;
    std::string tmp = "";
	memset(this->buffer,'\0',this->buffer_size);
	if(recv(this->sock,this->buffer,this->buffer_size,0) > -1){
		tmp+=this->buffer;
		size_t index = tmp.find(this->end_line,this->end_line.length());
		if(index != std::string::npos)
			tmp.erase(index);
		return tmp;
	}
	else{
		return "";
	}
    
}

bool Commutator::up_connection(){
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;

	int iResult = WSAStartup(wVersionRequested, &wsaData);
	if(iResult){
		std::cerr << "Error, problems with starting WSA!" << std::endl;
        return false;
	}
    this->sock = socket(AF_INET,SOCK_STREAM,0);
	if(this->sock == INVALID_SOCKET){
        std::cerr << "Error, problems with opening socket!" << std::endl;
        return false;
    }

    this->server = gethostbyname(this->host.c_str());
    if(this->server == NULL){
        std::cerr << "Error, no such host!" << std::endl;
        return false;
    }

	memset((char*) &this->serv_addr,'\0',sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;

	memcpy((char *) &this->serv_addr.sin_addr.s_addr,(char*)server->h_addr_list[0],this->server->h_length);
    //bcopy((char*)server->h_addr_list[0], (char *) &this->serv_addr.sin_addr.s_addr,this->server->h_length);

    this->serv_addr.sin_port = htons(this->port);

    if(connect(this->sock,(struct sockaddr *) &this->serv_addr,sizeof(this->serv_addr)) < 0){
        std::cerr << "Error, problem with connection to host!" << std::endl;
        return false;
    }
    this->connected = true;
    return true;
}

std::string Commutator::exchange(std::string request){
    if(!this->connected) return "";
    this->send_all(request);
    return this->recv_all();
}

bool Commutator::hand_shake(std::string hand_shake_msg){
     if(!this->connected) return false;
     this->send_all(hand_shake_msg);
     this->recv_all();
     return true;
}

bool Commutator::down_connection(std::string last){
    if(!this->connected) return false;
    if(this->sock < 0) return false;
    this->send_all(last);
    this->connected = false;
	closesocket(this->sock);
    return true;
}
