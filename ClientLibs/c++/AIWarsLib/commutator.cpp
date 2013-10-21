#include "commutator.h"


Commutator::Commutator(std::string hostname,std::string port)
{
    this->port = atoi(port.c_str());
    this->host = hostname;
    this->connected = false;
    this->buffer_size = 1024;
    this->buffer = new char[this->buffer_size];
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
    if(write(this->sock,request.c_str(),request.length()) < 0){
        return "";
    }
    bzero(this->buffer,this->buffer_size);
    if(read(this->sock,this->buffer,this->buffer_size) < 0){
        return "";
    }
    return std::string(this->buffer);
}

bool Commutator::hand_shake(std::string hand_shake_msg){
     if(!this->connected) return false;
     if(write(this->sock,hand_shake_msg.c_str(),hand_shake_msg.length()) < 0){
         return false;
     }
     bzero(buffer,this->buffer_size);
     while(read(this->sock,this->buffer,this->buffer_size) < 0);
     return true;
}

bool Commutator::down_connection(std::string last){
    if(!this->connected) return false;
    if(this->sock < 0) return false;
    this->exchange(last);
    this->connected = false;
    close(this->sock);
    return true;
}
