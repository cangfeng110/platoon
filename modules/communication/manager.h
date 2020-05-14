#ifndef __MANAGER_H__
#define __MANAGER_H__
#include "modules/communication/worldmodle.h"
#include "modules/communication/configdata.h"
#include "modules/communication/datadefine.h"
#include "include/base/NonCopyable.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "include/proto/ProtoClassNameyTypeDefine.h"


namespace platoon {
namespace communication {

class Manager : public base::NonCopyable {
public:
    Manager();

    //void SetFmsInfo (const FmsInfo& fms_info);
    void UpdatePlatoonManagerInfo ();

private:
//data member
    DriveMode actual_drive_mode_;
    DriveMode desire_drive_mode_;
    int ID_;
    float m_safe_distance_;
    float hmi_safe_dis_;
    float fms_safe_dis_;
    /**
     * this vector is to sort to calculated id 
    */
    std::vector<VehicleData> other_vehicles_;
    /* this vector storage communication status, 
     * if normal, storage vehicle id
     * else, storage -vehicel_id;
     */
    std::vector<int8_t> vehicle_status_;
    /** 
     * this map storae all the platoon vehicle id and platoon id 
     * the key is paltoon id
     * the value is vehicle id
     * when the actual mode is auto or manual, this map will be update, else no update 
    **/
    std::map<int,int> platoon_id_map_;
    /**
     * this map is help function IsAllJoinPlatoon
    */
    std::map<std::string, int> license_map_{
        {"沪YSG_14", 14},
        {"沪YSG_15", 15},
        {"沪YSG_16", 16},
        {"沪YSG_17", 17},
        {"沪YSG_18", 18},
        {"沪YSG_19", 19},
        {"沪YSG_20", 20},
        {"沪YSG_21", 21},
        {"沪YSG_22", 22},
    }; 

    WorldModle m_worldmodle_;

    //below info is from other datacontainer, from other threads
    VehicleGpsData ego_gps_info_;
    bool ego_gps_isupdate_;

    VehicleVcuData ego_vcu_info_;
    bool ego_vcu_isupdate_;

    EgoPlanningMsg planning_info_;
    bool plan_info_isupdate_;

    std::map<int, templateDataContainer<VehicleData>> platoon_vehicles_info_;
    bool platoon_info_isupdate_;

    std::map<int, templateDataContainer<VehicleData>> v2x_vehicles_info_;
    bool v2x_info_isupdate_;

    HmiFmsInfo hmi_fms_info_;
    bool hmi_isupdate_;

    FMSPreFormationInfo fms_pre_info_;
    bool pre_info_isupdate_;
    
    //below info is from senddatacontainer, from same thread
    FmsOrder m_fms_order_;
    bool fms_order_isupdate_;

    int platoon_number_;
    bool plnumber_isupdate_;

    int m_debug_flags_;
    int m_debug_thw_HZ_;
    bool m_debug_StateFlow_;
    
private:
//function member
    void UpdateInfo();
    float THWDis ();
    float FrontDis ();
    void CalculateID ();
    float CalThreshold();
    bool IfAbnormal ();
    void ResetFmsOrder();
    bool IsAllJoinPlatoon();
    bool IsAllowDequeue();
    DriveMode FrontMode();
    void ProcessCommand ();
    bool IsCalID();
    int FindLeader(int& map_index);
    bool IsAllowEnqueue();
    bool IsAllowKeep();
    void CalSafeDis();
    
};

} // namespace communication

} // namespace platoon

#endif // __MANAGER_H__
