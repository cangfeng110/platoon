#include "modules/communication/manager.h"
#include <algorithm>
#include "modules/common/functiontool.h"

namespace platoon {

namespace communication {

Manager::Manager () {
    actual_drive_mode = Manual;
    desire_drive_mode = Notset;
    _ID = 0;
    other_vehicles.reserve (100);
}

Manager::~Manager () {
}

float Manager::THW ()
{
    if (!DataContainer::GetInstance ()->ego_vehicle_vcu_data_.isUpToDate ())
    {
        return 0.0;//XXX invalid THW
    }
    const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
    float speed = ego_vehicle_vcu_data.fSpeed;
    float thw;
    if (speed < 0.0)
    {
        return 0.0;//XXX invalid THW
    }
    if (speed < 14.0)
    {
        thw = 1.5 - (14.0 - speed) / 20.0;
    }
    else
    {
        thw = 1.5 + (speed - 14.0) / 9.0;
    }
    if (thw < 1.0)
    {
        thw = 1.0;
    }
    if (thw > 2.0)
    {
        thw = 2.0;
    }
    return thw + 15 / speed;
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
    float thw = THW ();
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
                if (_ID > 1 && (front_drive_mode == Leader || front_drive_mode == KeepQueue))
                {
                    if (thw == 0.0)
                    {
                        break;//no speed data. ignore command
                    }
                    if (thw <= 1.2)
                    {
                        desire_drive_mode = Enqueue;
                    }
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
            if (msg->fms_order == F_Dequeue)
            {
                if (thw == 0.0)
                {
                    break;//no speed data. ignore command
                }
                if (thw >= 2.0)
                {
                    desire_drive_mode = Auto;
                }
            }
            else
            {
                if (thw <= 1.5)
                {
                    desire_drive_mode = KeepQueue;
                }
            }
            break;
        case Dequeue:
            if (msg->fms_order == F_Dequeue)
            {
                if (thw == 0.0)
                {
                    break;//no speed data. ignore command
                }
                if (thw >= 2.0)
                {
                    desire_drive_mode = Auto;
                }
            }
            break;
        case KeepQueue:
            if (msg->fms_order == F_Dequeue)
            {
                int i;
                for (i = _ID; i < other_vehicles.size; i++)
                {
                    if (other_vehicles[i].actual_drive_mode == KeepQueue || other_vehicles[i].actual_drive_mode == Enqueue)
                    {
                        break;
                    }
                }
                if (i == other_vehicles.size)//XXX at the end of platoon
                {
                    desire_drive_mode = Dequeue;
                }
            }
            break;
        case Abnormal:
        default:
            //ignore command
            break;
    }
}

static bool
compare_relative_x (VehicleData& a, VehicleData& b)
{
    return (a.relative_x > b.relative_x);
}

void Manager::CalculateID ()
{
    _ID = 0;
    if (!DataContainer::GetInstance ()->ego_vehicle_gps_data_.isUpToDate ())
    {
        return;
    }
    int i = 0;
    const VehicleGpsData ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
    if (DataContainer::GetInstance ()->v2x_other_vehicle_data_.isUpToDate ())
    {
        other_vehicles.clear ();
        for (auto map_it : DataContainer::GetInstance ()->v2x_other_vehicle_data_.getData ())
        {
            VehicleData v2x_other_vehicle_data = map_it.second.getData ();
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                                 ego_vehicle_location.heading,
                                                                 ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                 ego_vehicle_location.height,
                                                                 v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                 v2x_other_vehicle_data.altitude);
            other_vehicles.push_back (v2x_other_vehicle_data);
        }
        std::sort (other_vehicles.begin (), other_vehicles.end (), compare_relative_x);
        for (i = 0; i < other_vehicles.size (); i++)
        {
            if (other_vehicles[i].relative_x < 0.0)
            {
                break;
            }
        }
    }
    _ID = i + 1;//if no v2x_other_vehicle_data, _ID = 1
}

void Manager::UpdatePlatoonManagerInfo ()
{
    if (_ID == 0)
    {
        return;//no ego_vehicle_location
    }
    PlatoonManagerInfo platoon_manager_info;
    platoon_manager_info.desire_drive_mode = desire_drive_mode;
    platoon_manager_info.vehicle_num = 0;
    platoon_manager_info.leader_frenet_dis = 0.0;
    platoon_manager_info.front_frenet_dis = 0.0;
    double x;
    double y;
    EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    actual_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
    switch (actual_drive_mode)
    {
        case Manual:
            break;
        case Auto:
            break;
        case Leader://no leader, no front
            break;
        case KeepQueue:
            //leader_vehicle
            {
                int i;
                for (i = _ID - 2; i >= 0; i--)
                {
                    if (other_vehicles[i].actual_drive_mode == Leader)
                    {
                        platoon_manager_info.leader_vehicle = other_vehicles[i];
                        platoon_manager_info.vehicle_num = _ID - (i + 1);
                        platoon_manager_info.leader_frenet_dis = sqrt (other_vehicles[i].relative_x * other_vehicles[i].relative_x + other_vehicles[i].relative_y * other_vehicles[i].relative_y);
                        break;
                    }
                }
            }
            //fall through
        case Enqueue:
        case Dequeue:
            //front_vehicle
            platoon_manager_info.front_vehicle = other_vehicles[_ID - 2];//_ID surely >= 2
            platoon_manager_info.front_frenet_dis = sqrt (other_vehicles[_ID - 2].relative_x * other_vehicles[_ID - 2].relative_x + other_vehicles[_ID - 2].relative_y * other_vehicles[_ID - 2].relative_y);
            break;
        case Abnormal:
        default:
            break;
    }
    DataContainer::GetInstance ()->manager_data_.setData (platoon_manager_info);
}

} // namesapce manager

} // namespace platoon
