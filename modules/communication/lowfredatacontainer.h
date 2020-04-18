#ifndef _LOW_FRE_DATACONTAINER_H
#define _LOW_FRE_DATACONTAINER_H   

#include "modules/communication/datadefine.h"
#include "modules/template/templateDataContainer.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"


namespace platoon
{
namespace communication
{
class LowFreDataContanier
{
public:
    templateDataContainer_Mutex<EgoPlanningMsg>     planning_data_;

    templateDataContainer_Mutex<FMSPreFormationInfo> fms_pre_info_;

    templateDataContainer_Mutex<FMSApplyResultInfo> fms_apply_result_;

    templateDataContainer_Mutex<HmiFmsInfo> hmi_fms_info_;

    static LowFreDataContanier* GetInstance();

    void DecreaseTtl();

private:
    static LowFreDataContanier* instance_;

    LowFreDataContanier(){}

    LowFreDataContanier(const LowFreDataContanier&)=delete;

    LowFreDataContanier& operator =(const LowFreDataContanier&)=delete;
   
};
}//namespace communication
}//namespace platoon

#endif