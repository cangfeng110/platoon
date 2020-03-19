#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "include/base/NonCopyable.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/worldmodle.h"
#include "modules/communication/configdata.h"

namespace platoon {
namespace communication {

class Manager : public base::NonCopyable {
public:
    Manager();
    ~Manager();

    enum DriveMode
    {
        Manual = 0,
        Auto = 1,
        Leader = 2,
        Enqueue = 3,
        KeepQueue = 4,
        Dequeue = 5,
        Abnormal = 6,
        Notset = 7,
    };

    enum FmsOrder
    {
        F_Invalid = 0,
        F_Leader = 1,
        F_Enqueue = 2,
        F_Dequeue = 3,
        F_DisBand = 4,
    };

    //void SetFmsInfo (const FmsInfo& fms_info);
    void UpdatePlatoonManagerInfo ();

private:
//data member
    DriveMode actual_drive_mode;
    DriveMode desire_drive_mode;
    int _ID;
    float m_safe_distance_;
    /**
     * this vector is to sort to calculated id 
    */
    std::vector<VehicleData> other_vehicles;

    /* this vector storage communication status, 
     * if normal, storage vehicle id
     * else, storage -vehicel_id;
     */
    std::vector<int> vehicle_status_;

    /** 
     * this map storae all the platoon vehicle id and platoon id 
     * the key is paltoon id
     * the value is vehicle id
     * when the actual mode is auto or manual, this map will be update, else no update 
    **/
    std::map<int,int> platoon_id_map_;

    FmsOrder m_fms_order_;

    WorldModle m_worldmodle_;

    int m_debug_flags;
    int m_debug_thw_HZ;
    bool hmi_fms_valid_;
    
private:
//function member
    float THWDis ();
    float FrontDis ();
    void SetSafeDis(float safe_dis);
    void CalculateID ();
    bool IfAbnormal ();
    void ProcessCommand ();
};

} // namespace communication

} // namespace platoon

#endif // __MANAGER_H__
