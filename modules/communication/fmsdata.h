#ifndef _FMS_DATA_
#define _FMS_DATA_

#include "include/proto/VehicleFleetProto.pb.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"
#include "modules/template/templateDataContainer.h"

namespace platoon
{
namespace communication
{
class FmsData
{
public:
    templateDataContainer<FMSPreFormationInfo> fms_pre_info_;

    templateDataContainer<FMSApplyResultInfo> fms_apply_info_;

    static FmsData* GetInstance();

    void DecreaseTtl(){};


private:
    FmsData(){}

    FmsData(const FmsData& ){} = delete;

    FmsData& operator=(const FmsData& ) {} = delete; 

    static FmsData* instance_;

};

}//namespace communication
}//namespace platoon
#endif