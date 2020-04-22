#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <lcm/lcm-cpp.hpp>
#include <memory>

#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"
#include "modules/communication/sendpdm.h"
#include "modules/communication/manager.h"
#include "modules/communication/fms.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"
#include "include/base/EventLoop.h"
#include "include/base/Logging.h"

namespace platoon
{
namespace communication
{
using atd::ipc::RxMsgPtr;
using atd::ipc::sendMessageViaLcm;
class Process
{
public:
    Process();
    ~Process();
    void Loop();

private:
    
    int debug_pmi_HZ_;

    lcm::LCM lcm_;

    base::EventLoop loop_;

    Manager manager_;

    FMS fms_;

    SendPDM send_pdm_;

    void PublishManagerInfo(); 

    void PublishToFmsInfo();

    void PublishToFusionInfo();
};
}//namespace communication
}//namespace platoon
#endif