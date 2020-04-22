#include "modules/communication/lowfredatacontainer.h"

namespace platoon
{
namespace communication
{
LowFreDataContanier* LowFreDataContanier::instance_ = new LowFreDataContanier();

LowFreDataContanier* LowFreDataContanier::GetInstance()
{
    return instance_;
}

void LowFreDataContanier::DecreaseTtl()
{
    //planning_data_.decreaseTTL();
}

} // namespace communication
} // namespace platoon