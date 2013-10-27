#include "commutator.h"


Commutator::Commutator(std::string hostname,std::string port)
{
    this->port = atoi(port.c_str());
    this->host = hostname;
    this->connected = false;
    this->buffer_size = 1024;
    this->buffer = new char[this->buffer_size];
}

void Commutator::send_all(std::string msg){
    size_t i = 0;
    size_t msg_len = msg.length();
    const char * raw_msg = msg.c_str();
    for(i = 0; i< msg_len; i+=send(this->sock,raw_msg,msg_len - i,0));
}

std::string Commutator::recv_all(){
    size_t i = 0;
    std::string tmp;
    bzero(this->buffer,this->buffer_size);
    recv(this->sock,this->buffer,this->buffer_size,0);
    tmp+=this->buffer;
    return tmp;
}

bool Commutator::up_connection(){
    this->sock = socket(AF_INET,SOCK_STREAM,0);
    if(this->sock < 0){
        std::cerr << "Error, problems with opening socket!" << std::endl;
        return false;
    }

    this->server = gethostbyname(this->host.c_str());
    if(this->server == NULL){
        std::cerr << "Error, no such host!" << std::endl;
        return false;
    }
    bzero((char*) &this->serv_addr,sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;

    bcopy((char*)server->h_addr_list[0], (char *) &this->serv_addr.sin_addr.s_addr,this->server->h_length);

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
    this->exchange(last);
    this->connected = false;
    shutdown(this->sock,2);
    return true;
}
