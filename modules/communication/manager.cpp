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

void Manager::SetFmsInfo (const FmsInfo& msg)
{
    m_fms_info = msg;
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
    if (speed < 0.001)
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

float Manager::TimeToFront ()
{
    if(_ID <= 1)
    {
        return 0.0;//invalid
    }
    VehicleData front_vehicle = other_vehicles[_ID - 2];
    if (front_vehicle.speed < 0.01)
    {
        return 0.0;
    }
    return front_vehicle.relative_x / front_vehicle.speed;
}

void Manager::ProcessCommand ()
{
    if (!DataContainer::GetInstance ()->planning_data_.isUpToDate ())
    {
        return;
    }
    //PlatoonManagerInfo platoon_manager_info = DataContainer::GetInstance ()->manager_data_.getData ();//XXX check isUpToDate?
    EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    actual_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
    float thw = THW ();
    float time_to_front = TimeToFront ();
    switch (actual_drive_mode)
    {
        case Manual:
            //ignore command
            break;
        case Auto:
            if (m_fms_info.fms_order == F_Leader)
            {
                desire_drive_mode = Leader;
            }
            else if (m_fms_info.fms_order == F_Enqueue)
            {
                DriveMode front_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
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
            if (m_fms_info.fms_order == F_Dequeue)
            {
                desire_drive_mode = Auto;
            }
            break;
        case Enqueue:
            if (time_to_front <= 1.5)
            {
                desire_drive_mode = KeepQueue;
            }
            break;
        case Dequeue:
            if (m_fms_info.fms_order == F_Dequeue)
            {
                if (time_to_front - thw < 0.01)
                {
                    desire_drive_mode = Auto;
                }
            }
            break;
        case KeepQueue:
            if (m_fms_info.fms_order == F_Dequeue)
            {
                int i;
                for (i = _ID; i < other_vehicles.size (); i++)
                {
                    if (other_vehicles[i].actual_drive_mode == KeepQueue || other_vehicles[i].actual_drive_mode == Enqueue)
                    {
                        break;
                    }
                }
                if (i == other_vehicles.size ())//XXX at the end of platoon
                {
                    desire_drive_mode = Dequeue;
                }
            }
            else if (m_fms_info.fms_order == F_DisBand)
            {
                desire_drive_mode = Dequeue;
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
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (v2x_other_vehicle_data.relative_heading,
                                                                   ego_vehicle_location.heading,
                                                                   v2x_other_vehicle_data.heading);
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
    CalculateID ();
    ProcessCommand ();
    m_worldmodle_.GetWorldmodleVehiles();

    if (_ID == 0)
    {
        return;//no ego_vehicle_location
    }
    PlatoonManagerInfo platoon_manager_info;
    platoon_manager_info.desire_drive_mode = desire_drive_mode;
    platoon_manager_info.vehicle_num = 0;
    platoon_manager_info.leader_frenet_dis = 1.0e10;
    platoon_manager_info.front_frenet_dis = 1.0e10;
    double x;
    double y;
    switch (desire_drive_mode)
    {
        case Manual:
            break;
        case Auto:
            break;
        case Leader://no leader, no front
            break;
        case KeepQueue:
            //leader_vehicle + front vehicle
            {
                int i;
                for (i = _ID - 2; i >= 0; i--)
                {
                    if (other_vehicles[i].actual_drive_mode == Leader)
                    {
                        platoon_manager_info.leader_vehicle = other_vehicles[i];
                        int leader_id = other_vehicles[i].vehicle_id; 
                        platoon_manager_info.leader_frenet_dis = m_worldmodle_.GetFrenetDis(leader_id);
                        platoon_manager_info.vehicle_num = _ID - (i + 1);
                        break;
                    }
                }
            }
            //fall through
        case Enqueue:
        case Dequeue:
            //front_vehicle
            {
                platoon_manager_info.front_vehicle = other_vehicles[_ID - 2];//_ID surely >= 2
                int front_id = other_vehicles[_ID - 2].vehicle_id;
                platoon_manager_info.front_frenet_dis = m_worldmodle_.GetFrenetDis(front_id);
            }
            break;
        case Abnormal:
        default:
            break;
    }
    DataContainer::GetInstance ()->manager_data_.setData (platoon_manager_info);
}

} // namesapce manager

} // namespace platoon
