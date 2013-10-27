#ifndef COMMUTATOR_H
#define COMMUTATOR_H
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class Commutator
{
private:
    int sock, port,buffer_size;
    bool connected;
    std::string host;
    char *buffer;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    void send_all(std::string msg);
    std::string recv_all();

public:
    Commutator(std::string hostname = "localhost",std::string port = "1234");
    std::string exchange(std::string request);
    bool up_connection();
    bool hand_shake(std::string hand_shake_msg = "HI\r\n");
    bool down_connection(std::string last = "EOG\r\n");
};

#endif // COMMUTATOR_H
