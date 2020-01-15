#include "modules/communication/udp.h"

#include <string.h>


Udp::Udp(std::string &ip, const int port)
    : s_initialized_(false), r_initialized_(false),sip_(ip), sport_(port)
{

}

Udp::~Udp()
{
    close(sockfd_);
}

void Udp::init()
{
    bzero(&sAddr_, sizeof(sAddr_));
    sAddr_.sin_family = AF_INET;
    sAddr_.sin_port = htons(sport_);
    sAddr_.sin_addr.s_addr = inet_addr(sip_.c_str());

    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd_ < 0){
        printf("Create socket failed.");
        exit(1);
    }
    s_initialized_ = true;
}

void Udp::send(const char *buf, const int len)
{
    if(s_initialized_)
    {
        if(sendto(sockfd_, buf, len, 0,
               (struct sockaddr *)&sAddr_, sizeof(sAddr_)) < 0)
        {
            printf("Udp send error to %s:%d.\n", sip_.c_str(), sport_);
        }
        else
        {
            //printf("Udp send %d bytes to %s:%d.\n", len, sip_.c_str(), sport_);

            int i = len;

            char *p = (char*)buf;
            while(i--) {
                //printf("%02x ", ( (unsigned char) (*p++) ));
            }
            //printf("\n");
        }
    }
    else
    {
        //printf("Socket isn't initialized. "
               //"Use init() before send.\n");
    }
}

void Udp::rbind(const int port, const std::string ip)
{
    rip_ = ip;
    rport_ = port;

    bzero(&rAddr_, sizeof(rAddr_));
    rAddr_.sin_family = AF_INET;
    rAddr_.sin_port = htons(rport_);
    if(rip_ != ""){
        rAddr_.sin_addr.s_addr = inet_addr(rip_.c_str());
    }else{
        rAddr_.sin_addr.s_addr = INADDR_ANY;
    }

    if(s_initialized_)
    {
        if(bind(sockfd_, (struct sockaddr *)&rAddr_, sizeof(rAddr_)) < 0){
            printf("udp bind error.\n");
            exit(1);
        }
        printf("udp bind port:%d.\n", rport_);
        r_initialized_ = true;
    }
    else
    {
        printf("Socket isn't initialized. "
               "Use init() before bind.\n");
    }
}

int Udp::recv(char *buf, const int &len,
               struct sockaddr_in &peerAddr, socklen_t &socklen)
{
    if(r_initialized_)
    {
        int rlen = recvfrom(sockfd_, buf, len, 0,
                            (struct sockaddr*)&peerAddr, &socklen);
        if(rlen < 0){
            printf("udp receive error.\n");
            exit(1);
        }
        printf("udp receive %d bytes from port %d.\n", rlen, peerAddr.sin_port);
        return rlen;
    }
    else
    {
        printf("Socket isn't binded. "
               "Use bind() before recv.\n");
        return -1;
    }
}






