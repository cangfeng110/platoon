#ifndef _FMS_DATA_
#define _FMS_DATA_

#include "modules/template/templateDataContainer.h"
#include "modules/communication/datadefine.h"

#include "include/proto/ProtoClassNameyTypeDefine.h"
#include "include/proto/DataMessageProto.pb.h"
#include "include/protocol/lcmDataNameTypeDefine.h"


namespace platoon
{
namespace communication
{
class FmsData
{

public:

    templateDataContainer<FMSPreFormationInfo> fms_pre_info_;

    templateDataContainer<FMSApplyResultInfo> fms_apply_result_;

    templateDataContainer<HmiFmsInfo> hmi_fms_info;

    templateDataContainer<FmsOrder> fms_order_;

    static FmsData* GetInstance();

    void DecreaseTtl();

private:
    FmsData(){}

    FmsData(const FmsData& ){}

    FmsData& operator=(const FmsData& ) {} 

    static FmsData* instance_;

};

}//namespace communication
}//namespace platoon
#endif