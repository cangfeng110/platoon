#ifndef _COMMU_H_
#define _COMMU_H_

#include <memory>

#include <lcm/lcm-cpp.hpp>

#include "include/base/CountDownLatch.h"
#include "include/base/EventLoop.h"
#include "include/base/Logging.h"
#include "protocol/lcmDataNameTypeDefine.h"

#include "modules/communication/configdata.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/handler.h"
#include "modules/communication/manager.h"

namespace platoon {
namespace communication {

class communication : public base::NonCopyable {
public:
    communication();
    ~communication();

    void Loop();

private:
    void HandleEgoVehicleGpsInfo(const lcm::ReceiveBuffer *rbuf,
                                 const std::string &channel,
                                 const VehicleGpsData *msg);
    void HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &channel,
                                  const VehicleVcuData *msg);
    void HandleFmsInfo(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &channel,
                                  const FmsInfo *msg);
    void HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                            const std::string &channel,
                            const EgoPlanningMsg *msg);
    void HandleTestVehicleInfo(const lcm::ReceiveBuffer *rbuf,
                               const std::string &channel,
                               const VehicleData *msg);

    void PublishManagerInfo();
    
    void BroastEgoVehicleInfo();

    void ReceiveV2xOtherVehicleInfo();

    //void PublishWorldmodelInfo();

    std::unique_ptr<base::FileLogger> logger_;
    base::EventLoop loop_;

    // lcm channel
    lcm::LCM lcm_;
    std::unique_ptr<base::Channel> lcm_channel_;
    // v2x channel
    std::unique_ptr<base::Channel> v2x_channel_;

    // handler
    Handler handler_;
    Manager manager_;
};

} // namesapce communication

} // namespace platoon

#endif // _COMMU_H_
