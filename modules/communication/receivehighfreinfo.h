#ifndef _RECEIVE_HIGH_FREQUENCY_INFO_
#define _RECEIVE_HIGH_FREQUENCY_INFO_

#include <lcm/lcm-cpp.hpp>
#include <memory>

#include "modules/communication/highfredatacontainer.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/base/EventLoop.h"
#include "include/base/Channel.h"
#include "include/base/CountDownLatch.h"
#include "include/base/Logging.h"
namespace platoon
{
namespace communication
{

class ReceiveHighFreInfo
{
public:

    ReceiveHighFreInfo();

    ~ReceiveHighFreInfo();

    void Loop();

    void HandleEgoVehicleGpsInfo(const lcm::ReceiveBuffer *rbuf,
                                 const std::string &channel,
                                 const VehicleGpsData *msg);
    void HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &channel,
                                  const VehicleVcuData *msg);

private:

    lcm::LCM lcm_;
    
    base::EventLoop loop_;

    std::unique_ptr<base::Channel> lcm_channel_;

    int debug_gps_hz_;

    int debug_vcu_hz_;
};
}//namespace communication
}//mamespace platoon

#endif