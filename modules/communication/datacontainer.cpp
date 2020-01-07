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
} 

} // namespace communication
} // namespace platoon
