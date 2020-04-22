#ifndef _RECEIVE_LOW_FREQUENCY_INFO_
#define _RECEIVE_LOW_FREQUENCY_INFO_

#include <memory>
#include <lcm/lcm-cpp.hpp>

#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"

#include "include/proto/DataMessageProto.pb.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/base/EventLoop.h"
#include "include/base/Channel.h"
#include "include/base/Logging.h"

namespace platoon
{
namespace communication
{
using atd::ipc::RxMsgPtr;
using atd::ipc::sendMessageViaLcm;

class ReceiveLowFreInfo
{
public:
    ReceiveLowFreInfo();

    ~ReceiveLowFreInfo();

    void Loop();

private:
    lcm::LCM lcm_;

    std::unique_ptr<base::Channel> lcm_channel_;

    std::shared_ptr<FMSPreFormationInfo> pre_info_ptr_;

    std::shared_ptr<FMSApplyResultInfo> apply_result_ptr_;

    base::EventLoop loop_;

    int actual_drive_mode_;

    int8_t fms_order_;
    
    float safe_distance_;

    int platoon_number_;

    std::string pre_serial_id_;

    std::string back_serial_id_;

    int debug_plan_hz_;

    int debug_hmi_hz_;

    void HandleHmiFmsInfo(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &channel,
                                  const HmiFmsInfo *msg);

    void HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                            const std::string &channel,
                            const EgoPlanningMsg *msg);

    bool ReceiveFmsPreInfo();
   
    bool ReceiveFmsApplyBack();
};
} //namespace communication
}//namespace platoon


#endif