#include "modules/communication/highfredatacontainer.h"

namespace platoon
{
namespace communication
{
HighFreDataContainer* HighFreDataContainer::instance_ = new HighFreDataContainer();

HighFreDataContainer* HighFreDataContainer::GetInstance()
{
    return instance_;
}

void HighFreDataContainer::DecreaseTtl()
{
    ego_vehicle_gps_data_.decreaseTTL();
    ego_vehicle_vcu_data_.decreaseTTL();
}

} // namespace communication
} // namespace platoon