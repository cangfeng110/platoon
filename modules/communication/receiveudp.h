#ifndef _RECEIVE_UDP_H_
#define _RECEIVE_UDP_H_

#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <lcm/lcm-cpp.hpp>

//#include "modules/communication/handler.h"
#include "modules/communication/udp.h"

#include "include/base/EventLoop.h"
#include "include/base/Logging.h"
#include "include/base/Channel.h"

namespace platoon
{
namespace communication
{
#define MAX_RECEIVE_LEN 1024
class ReceiveUDP
{
public:
    ReceiveUDP();

    ~ReceiveUDP();

    void Loop();

private:

    int debug_flags_;

    std::unique_ptr<base::Channel> v2x_channel_;

    base::EventLoop loop_;

    int sockfd_;

    struct sockaddr_in v2x_sockaddr_;

    char v2x_buffer_[MAX_RECEIVE_LEN];

    lcm::LCM lcm_;
    
    int DecodeV2xVechileInfo3();

    int DecodeV2xVehicleInfo2(); 

    int SilDecodeV2xVechileInfo();

};
}//namespace communication
}//namespace platoon

#endif