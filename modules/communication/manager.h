#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "include/base/NonCopyable.h"
#include "protocol/lcmDataNameTypeDefine.h"
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
        F_Disband = 4,
    };

    void SetFmsInfo (const FmsInfo& fms_info);
    void UpdatePlatoonManagerInfo ();

private:
    DriveMode actual_drive_mode;
    DriveMode desire_drive_mode;
    int _ID;
    std::vector<VehicleData> other_vehicles;
    WorldModle m_worldmodle_;
    FmsInfo m_fms_info;

    float THW ();
    float TimeToFront ();
    void CalculateID ();
    void ProcessCommand ();

    int m_debug_flags;
    int m_debug_thw_HZ;
};

} // namespace communication

} // namespace platoon

#endif // __MANAGER_H__
