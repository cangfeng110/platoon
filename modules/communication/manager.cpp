#include "modules/communication/manager.h"

namespace platoon {

namespace communication {

Manager::Manager () {
    actual_drive_mode = Manual;
    desire_drive_mode = Manual;
    //XXX init platoon_manager_info, ego_planning_msg
}

Manager::~Manager () {
}

void Manager::ProcessCommand (const void* msg)
{
    switch (actual_drive_mode)
    {
        case Manual:
            return;//XXX
            break;
        case Auto:
            break;
        case Leader:
            break;
        case Enqueue:
            break;
        case Queue:
            break;
        case Dequeue:
            break;
        default:
            break;
    }
}

void Manager::CalculateID ()
{
}

PlatoonManagerInfo& Manager::GetPlatoonManagerInfo ()
{
    return platoon_manager_info;
}

EgoPlanningMsg& Manager::GetEgoPlanningMsg ()
{
    return ego_planning_msg;
}

} // namesapce manager

} // namespace platoon
