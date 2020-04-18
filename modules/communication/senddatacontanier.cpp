#include "modules/communication/senddatacontanier.h"

namespace platoon
{
namespace communication
{
    SendDataContanier* SendDataContanier::instance_ = new SendDataContanier();

    SendDataContanier* SendDataContanier::GetInstance()
    {
        return instance_;
    }
    
    void SendDataContanier::DecreaseTtl()
    {
        worldmodle_other_vehicle_data_.decreaseTTL();
        manager_data_.decreaseTTL();
    }

}//namespace communication
}//namespace platoon