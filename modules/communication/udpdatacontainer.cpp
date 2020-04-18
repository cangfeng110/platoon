#include "modules/communication/udpdatacontainer.h"

namespace platoon
{
namespace communication
{
UDPDataContainer* UDPDataContainer::instance_ = new UDPDataContainer();

UDPDataContainer* UDPDataContainer::GetInstance()
{
    return instance_;
}

void UDPDataContainer::DecreaseTtl()
{
    platoon_vehicles_data_.decreaseTTL();
    v2x_other_vehicles_data_.decreaseTTL();
}

} // namespace communication
} // namespace platoon