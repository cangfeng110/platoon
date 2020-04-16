#ifndef _UDP_
#define _UDP_

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <memory>


class Udp{
public:
    Udp(std::string &ip, const int port);
    Udp(){}
    ~Udp();

    void init();
    void rbind(const int port, const std::string ip = "");
    void send(const char *buf, const int len);
    int recv(char *buf, const int &len, sockaddr_in &peerAddr, socklen_t &socklen);
    bool sInitialized(){ return s_initialized_; }

private:
    bool s_initialized_;
    bool r_initialized_;
    int sockfd_;

    // send addr
    std::string sip_;
    int sport_;
    struct sockaddr_in sAddr_;

    // bind addr
    std::string rip_;
    int rport_;
    struct sockaddr_in rAddr_;
};

typedef std::shared_ptr<Udp> UdpPtr;

#endif
