#ifndef _SEND_PDM_H_
#define _SEND_PDM_H_

#include "modules/communication/udp.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/base/EventLoop.h"
#include "include/base/Logging.h"

namespace platoon
{
namespace communication
{

class SendPDM
{
public:
    SendPDM();

    int BroastEgoVehicleInfo2();

    int BroastEgoVehicleInfo3();

    int SilBroastEgoVehicleInfo();

private:
    static int send_number_;
    // debug paraments from config data
    int m_debug_flags_;

    //below info is from other datacontainer, from other threads
    int platoon_number_;

    VehicleGpsData ego_gps_info_;
    bool gps_isupdate_;

    VehicleVcuData ego_vcu_info_;
    bool vcu_isupdate_;

    EgoPlanningMsg planning_info_;
    bool plan_isupdate_;

    PlatoonManagerInfo manager_info_;
    bool manager_isupdate_;
    //
    Udp sudp_;
    std::string send_ip_;
    int send_port_;

    void UpdateInfo();
};

}//namespace communication
}//namespace platoon
#endif