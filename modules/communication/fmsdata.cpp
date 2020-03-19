#include "modules/communication/fmsdata.h"

namespace platoon
{
namespace communication
{
    FmsData* FmsData::instance_ = new FmsData(); 

    FmsData* FmsData::GetInstance()
    {
        return instance_;
    }

    void FmsData::DecreaseTtl()
    {
        //fms_pre_info_.decreaseTTL();
        //fms_apply_info_.decreaseTTL();
    }


}//namespace communication
}//namespace platoon