#ifndef _HIGH_FREQUENCY_DATACONTAINER_H_
#define _HIGH_FREQUENCY_DATACONTAINER_H_

#include "modules/template/templateDataContainer.h"
#include "include/protocol/lcmDataNameTypeDefine.h"

namespace platoon
{
namespace communication
{

class HighFreDataContainer
{
public:
    templateDataContainer_Mutex<VehicleGpsData>     ego_vehicle_gps_data_;

    templateDataContainer_Mutex<VehicleVcuData>     ego_vehicle_vcu_data_;

    static HighFreDataContainer* GetInstance()
    {
        return instance_;
    }

    void DecreaseTtl()
    {
     ego_vehicle_gps_data_.decreaseTTL();
     ego_vehicle_vcu_data_.decreaseTTL();   
    }

private:
    static HighFreDataContainer* instance_;

    HighFreDataContainer(){}

    HighFreDataContainer(const HighFreDataContainer&)=delete;
    
    HighFreDataContainer& operator=(const HighFreDataContainer&)=delete; 
};

HighFreDataContainer* HighFreDataContainer::instance_ = new HighFreDataContainer();

}//namespace communication
}//namespace platoon
#endif