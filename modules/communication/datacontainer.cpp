#include "modules/communication/datacontainer.h"

namespace platoon {

namespace communication {

DataContainer* DataContainer::instance_ = new DataContainer();

DataContainer* DataContainer::GetInstance() {
    return instance_;
}

void DataContainer::DecreaseTtl() {
    ego_vehicle_gps_data_.decreaseTTL();
    ego_vehicle_vcu_data_.decreaseTTL();
    v2x_other_vehicles_data_.decreaseTTL();
    platoon_vehicles_data_.decreaseTTL();
    worldmodle_other_vehicle_data_.decreaseTTL();
    manager_data_.decreaseTTL();
    //planning_data_.decreaseTTL();
} 

} // namespace communication
} // namespace platoon
