#include "modules/communication/manager.h"
#include <algorithm>
#include "modules/common/functiontool.h"

namespace platoon {

namespace communication {

Manager::Manager () {
    actual_drive_mode = Manual;
    desire_drive_mode = Notset;
    _ID = 0;
    //XXX init platoon_manager_info
}

Manager::~Manager () {
}

void Manager::ProcessCommand (const FmsInfo* msg)
{
    if (!DataContainer::GetInstance ()->planning_data_.isUpToDate ())
    {
        return;
    }
    PlatoonManagerInfo platoon_manager_info = DataContainer::GetInstance ()->manager_data_.getData ();//XXX check isUpToDate?
    EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    actual_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
    switch (actual_drive_mode)
    {
        case Manual:
            //ignore command
            break;
        case Auto:
            if (msg->fms_order == F_Leader)
            {
                desire_drive_mode = Leader;
            }
            else if (msg->fms_order == F_Enqueue)
            {
                DriveMode front_drive_mode = (DriveMode)platoon_manager_info.front_vehicle.actual_drive_mode;
                if (_ID > 1 && (front_drive_mode == Leader || front_drive_mode == Queue))//XXX && THW
                {
                    desire_drive_mode = Enqueue;
                }
            }
            break;
        case Leader:
            if (msg->fms_order == F_Dequeue)
            {
                desire_drive_mode = Auto;
            }
            break;
        case Enqueue:
        case Dequeue:
            if (msg->fms_order == F_Dequeue)
            {
                desire_drive_mode = Auto;
            }
            //XXX find front
            break;
        case Queue:
            if (msg->fms_order == F_Dequeue)
            {
                desire_drive_mode = Auto;
            }
            //XXX find front
            //XXX find leader
            break;
        default:
            //ignore command
            break;
    }
}

void Manager::CalculateID ()
{
    if (!DataContainer::GetInstance ()->ego_vehicle_gps_data_.isUpToDate ())
    {
        return;
    }
    const VehicleGpsData ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
    double relative_x[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double relative_y;
    int i = 1;
    if (DataContainer::GetInstance ()->v2x_other_vehicle_data_.isUpToDate ())
    {
        for (auto map_it : DataContainer::GetInstance ()->v2x_other_vehicle_data_.getData ())//XXX check size
        {
            const VehicleData v2x_other_vehicle_data = map_it.second.getData ();
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (relative_x[i], relative_y,
                                                                 ego_vehicle_location.heading,
                                                                 ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                 ego_vehicle_location.height,
                                                                 v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                 v2x_other_vehicle_data.altitude);
            i++;
        }
        std::sort (relative_x, relative_x + i, std::greater<double>());
        for (i = 0; i < 5; i++)
        {
            if (relative_x[i] == 0.0)
            {
                _ID = i + 1;
                break;
            }
        }
    }
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
