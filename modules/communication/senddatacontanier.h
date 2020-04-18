#ifndef _SEND_DATA_CONTANIER_H_
#define _SEND_DATA_CONTANIER_H_

#include "modules/template/templateDataContainer.h"
#include "modules/communication/datadefine.h"
#include "include/protocol/lcmDataNameTypeDefine.h"


namespace platoon
{
namespace communication
{
class SendDataContanier
{  
public:
    templateDataContainer_Map_Mutex<WorldModelObject> worldmodle_other_vehicle_data_;
    
    // manager data
    templateDataContainer_Mutex<PlatoonManagerInfo> manager_data_;

    templateDataContainer_Mutex<FmsOrder> fms_order_;

    static SendDataContanier* GetInstance();

    void DecreaseTtl();
    
private:
    static SendDataContanier* instance_;

    SendDataContanier(){}

    SendDataContanier(const SendDataContanier&)=delete;

    SendDataContanier& operator=(SendDataContanier&)=delete;
    
};



}//namespace communication
}//namespace platoon

#endif