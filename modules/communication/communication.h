#ifndef _COMMU_H_
#define _COMMU_H_

#include <memory>

#include <lcm/lcm-cpp.hpp>

#include "include/base/CountDownLatch.h"
#include "include/base/EventLoop.h"
#include "include/base/Logging.h"
#include "protocol/lcmDataNameTypeDefine.h"

#include "modules/communication/communication_config.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/handler.h"

namespace platoon {
namespace communication {

class communication : public base::NonCopyable {
public:
    communication();
    ~communication();

    void Loop();

private:
    void HandleEgoVehicleGpsInfo(const lcm::ReceiveBuffer *rbuf,
                                 const std::string &string,
                                 const VehicleGpsData *msg);
    void HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &string,
                                  const VehicleVcuData *msg);
    
    void BroastEgoVehicleInfo();

    
    void ReceiveV2xOtherVehicleInfo();

    std::unique_ptr<base::FileLogger> logger_;
    base::EventLoop loop_;

    // lcm channel
    lcm::LCM lcm_;
    std::unique_ptr<base::Channel> lcm_channel_;
    // v2x channel
    std::unique_ptr<base::Channel> v2x_channel_;

    // handler
    Handler handler_;
};

} // namesapce communication

} // namespace platoon

#endif // _COMMU_H_
