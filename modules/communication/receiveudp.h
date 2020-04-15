#ifndef _RECEIVE_UDP_H_
#define _RECEIVE_UDP_H_

#include <memory>

#include "modules/communication/handler.h"

#include "include/base/EventLoop.h"
#include "include/base/Logging.h"
#include "include/base/Channel.h"

namespace platoon
{
namespace communication
{
class ReceiveUDP
{
public:
    ReceiveUDP();

    ~ReceiveUDP();

    void Loop();

private:
    Handler handler_;

    std::unique_ptr<base::Channel> v2x_channel_;

    base::EventLoop loop_;

};
}//namespace communication
}//namespace platoon

#endif