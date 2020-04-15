#ifndef _UDP_DATACONTAINER_H
#define _UDP_DATACONTAINER_H

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

    static UDPDataContainer* GetInstance()
    {
        return instance_;
    }

    void DecreaseTtl()
    {
        platoon_vehicles_data_.decreaseTTL();
        v2x_other_vehicles_data_.decreaseTTL();
    }

private:
    static UDPDataContainer* instance_;

    UDPDataContainer(){}

    UDPDataContainer(const UDPDataContainer&)=delete;

    UDPDataContainer& operator =(const UDPDataContainer&)=delete;
    
};

UDPDataContainer* UDPDataContainer::instance_ = new UDPDataContainer();

}// namespace communication
}// namespace platoon


#endif