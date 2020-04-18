#ifndef _UDP_DATACONTAINER_H_
#define _UDP_DATACONTAINER_H_

#include "modules/template/templateDataContainer.h"
#include "include/protocol/lcmDataNameTypeDefine.h"

namespace platoon
{
namespace communication
{
class UDPDataContainer
{
public:
    templateDataContainer_Map_Mutex<VehicleData>   platoon_vehicles_data_;

    templateDataContainer_Map_Mutex<VehicleData>   v2x_other_vehicles_data_;

    static UDPDataContainer* GetInstance();

    void DecreaseTtl();

private:
    static UDPDataContainer* instance_;

    UDPDataContainer(){}

    UDPDataContainer(const UDPDataContainer&)=delete;

    UDPDataContainer& operator =(const UDPDataContainer&)=delete;
    
};



}// namespace communication
}// namespace platoon


#endif