#ifndef _FMS_H
#define _FMS_H
#include <vector>

#include "modules/communication/datadefine.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"
namespace platoon
{
namespace communication
{
class FMS
{
public:
    FMS();
    const ToFMSInfo& GetToFmsInfo() const;
    void UpdateToFmsInfo();
    void UpdateFmsOrder();

private:
    std::vector<double> m_enqueue_point_;
    std::vector<double> m_dequeue_point_;
    FmsOrder m_fms_order_;
    FmsOrder hmi_order_;
    bool m_hmi_fms_valid_;
    ToFMSInfo m_to_fms_info_;

    int hmi_plnumber_;
    int fms_plnumber_;
    int final_plnumber_;

    //below data need to read from other datacontanier ,from other threads
    FMSPreFormationInfo fms_pre_info_;
    bool fms_pre_info_isupdate_;

    FMSApplyResultInfo  fms_back_info_;
    bool fms_back_info_isupdate_;

    PlatoonManagerInfo  manager_info_;
    bool manager_info_isupdate_;

    EgoPlanningMsg      planning_info_;
    bool planning_info_isupdate_;

    VehicleGpsData ego_vehicle_gps_data_;
    bool ego_gps_isupdate_;

    HmiFmsInfo hmi_fms_info_;
    bool hmi_info_isupdate_;

    int vehicle_ID_;
    bool ID_isupdate_;

private:
    void UpdateInfo();
    void UpdatePoint();
    ApplyResult CalApplyResult();
    void CalApplyOrder();
    bool CalIfDisBand();
    void ResetApplyResult();
    void CalFmsOrder();
    void ChoseOrder();
    void ChosePloNumber();

    //print function
    void PrintApplyOrder(const ApplyOrder& order);
    void PrintApplyBack(const ApplyResult& result);
    void PrintToFmsInfo();
    void PrintFmsPreInfo();
    void PrintFmsBackInfo();


};

}//communication
}//namespace platoon




#endif