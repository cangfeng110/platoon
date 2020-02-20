#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "include/base/NonCopyable.h"
#include "protocol/lcmDataNameTypeDefine.h"

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
    };

    void CalculateID ();
    void ProcessCommand (const void* msg);//XXX
    PlatoonManagerInfo& GetPlatoonManagerInfo ();
    EgoPlanningMsg& GetEgoPlanningMsg ();

private:
    DriveMode actual_drive_mode;
    DriveMode desire_drive_mode;
    PlatoonManagerInfo platoon_manager_info;
    EgoPlanningMsg ego_planning_msg;
    int following;//target
    int is_leader;//target
};

} // namespace communication

} // namespace platoon

#endif // __MANAGER_H__
