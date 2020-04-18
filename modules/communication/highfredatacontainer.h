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

    static HighFreDataContainer* GetInstance();
   
    void DecreaseTtl();
   

private:
    static HighFreDataContainer* instance_;

    HighFreDataContainer(){}

    HighFreDataContainer(const HighFreDataContainer&)=delete;
    
    HighFreDataContainer& operator=(const HighFreDataContainer&)=delete; 
};

}//namespace communication
}//namespace platoon
#endif