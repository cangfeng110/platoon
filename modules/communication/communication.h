#ifndef _COMMU_H_
#define _COMMU_H_

#include <memory>

#include <lcm/lcm-cpp.hpp>

#include "modules/communication/configdata.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/handler.h"
#include "modules/communication/manager.h"
#include "modules/communication/fms.h"
#include "modules/communication/fmshandler.h"

#include "include/base/CountDownLatch.h"
#include "include/base/EventLoop.h"
#include "include/base/Logging.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/proto/DataMessageProto.pb.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"


namespace platoon 
{
namespace communication 
{
using atd::ipc::RxMsgPtr;
using atd::ipc::sendMessageViaLcm;

class communication : public base::NonCopyable 
{
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
    void HandleHmiFmsInfo(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &channel,
                                  const HmiFmsInfo *msg);
    void HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                            const std::string &channel,
                            const EgoPlanningMsg *msg);

    void PublishManagerInfo();
    
    void BroastEgoVehicleInfo();

    void SilBroastEgoVehicleInfo();

    void ReceiveV2xOtherVehicleInfo();

    void SilReceiveV2xOtherVehicleInfo();

    void PublishToFmsInfo();

    void PublishToFusionInfo();

private:

    std::unique_ptr<base::FileLogger> logger_;

    base::EventLoop loop_;

    // lcm channel
    lcm::LCM lcm_;

    std::unique_ptr<base::Channel> lcm_channel_;

    // v2x channel
    std::unique_ptr<base::Channel> v2x_channel_;

    // fms channel
    std::shared_ptr<ToFMSInfo> to_fms_ptr_;

    // handler
    Handler handler_;

    Manager manager_;

    FMS fms_;

    FmsHandler fms_handler_;

    int m_debug_flags;
    int m_debug_gps_HZ;
    int m_debug_vcu_HZ;
    int m_debug_pmi_HZ;
    int m_debug_plan_HZ;
    int m_debug_hmi_HZ;
};

} // namesapce communication

} // namespace platoon

#endif // _COMMU_H_
