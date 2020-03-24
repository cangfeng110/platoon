#ifndef _FMS_HANDLER_H_
#define _FMS_HANDLER_H_

#include <memory>
#include <iostream>

#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"
#include "include/proto/DataMessageProto.pb.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"
#include "modules/communication/fmsdata.h"

namespace platoon
{
namespace communication
{
using atd::ipc::RxMsgPtr;
using atd::ipc::sendMessageViaLcm;

class FmsHandler
{
private:
    std::shared_ptr<FMSPreFormationInfo> pre_info_ptr_;
    
    std::shared_ptr<FMSApplyResultInfo> apply_result_ptr_;
public:
    FmsHandler();

    bool ReceiveFmsPreInfo();
   
    bool ReceiveFmsApplyBack();

};


}
}

#endif