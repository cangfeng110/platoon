#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "include/base/NonCopyable.h"
#include "protocol/lcmDataNameTypeDefine.h"
#include "modules/communication/datacontainer.h"

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
        Queue = 4,
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
    };

    void CalculateID ();
    void ProcessCommand (const FmsInfo* msg);
    PlatoonManagerInfo& GetPlatoonManagerInfo ();
    EgoPlanningMsg& GetEgoPlanningMsg ();

private:
    DriveMode actual_drive_mode;
    DriveMode desire_drive_mode;
    int _ID;
    PlatoonManagerInfo platoon_manager_info;
    EgoPlanningMsg ego_planning_msg;
    int following;//target
    int is_leader;//target
};

} // namespace communication

} // namespace platoon

#endif // __MANAGER_H__
