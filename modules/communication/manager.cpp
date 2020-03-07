#include "modules/communication/manager.h"
#include <algorithm>
#include <math.h>
#include "modules/common/functiontool.h"

namespace platoon {

namespace communication {

#define Epslion 1e-8

Manager::Manager () {
    actual_drive_mode = Manual;
    desire_drive_mode = Notset;
    _ID = 0;
    other_vehicles.reserve (100);
    m_fms_info.fms_order = F_Invalid;
    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_thw_HZ = ConfigData::GetInstance ()->GetDebugThwHZ ();
    if (m_debug_thw_HZ <= 0)
        m_debug_thw_HZ = 25;
}

Manager::~Manager () {
}

void Manager::SetFmsInfo (const FmsInfo& msg)
{
    if (m_fms_info.fms_order != msg.fms_order)
    {
        printf ("asdf FMS_INFO fms_order changed: %d\n", msg.fms_order);
    }
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
        if (m_debug_flags & DEBUG_TimeToFront)
            printf ("_ID : %d\n", _ID);
        return 0.0;//invalid
    }
    VehicleData front_vehicle = other_vehicles[_ID - 2];
    const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
    float speed = ego_vehicle_vcu_data.fSpeed;
    if (m_debug_flags & DEBUG_TimeToFront)
        printf ("speed %f\n", speed);
    if (speed < 0.001)
    {
        return 0.0;
    }

    if (m_debug_flags & DEBUG_TimeToFront)
        printf ("relative_x: %f\n", front_vehicle.relative_x);
    return fabs(front_vehicle.relative_x - 17.0) / speed;
}

void Manager::ProcessCommand ()
{
    if (!DataContainer::GetInstance ()->planning_data_.isUpToDate ())
    {
        return;
    }
    DriveMode old = desire_drive_mode;
    EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    actual_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
    float thw = THW ();
    float time_to_front = TimeToFront ();
    static int debug_count = 0;
    debug_count++;
    if (debug_count % m_debug_thw_HZ == 0)
    {
        printf ("asdf thw: %f, time_to_front: %f\n", thw, time_to_front);
    }
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
                if(_ID <= 1)
                    break;
                VehicleData front_vehicle = other_vehicles[_ID - 2];
                DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode;
                if (front_drive_mode == Leader || front_drive_mode == KeepQueue || front_drive_mode == Enqueue)
                {
                    if (fabs(thw - 0.0) < Epslion) 
                    {
                        break;//no speed data. ignore command
                    }
                    if (time_to_front <= 1.2 * thw)
                    {
                        desire_drive_mode = Enqueue;
                    }
                }
            }
            break;
        case Leader:
            if (m_fms_info.fms_order == F_Dequeue || m_fms_info.fms_order == F_DisBand)
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
            if (time_to_front >= thw)
            {
                desire_drive_mode = Auto;
            }
            break;
        case KeepQueue:
            if (m_fms_info.fms_order == F_Dequeue)
            {
                int i = _ID - 1;
                if (i < other_vehicles.size ())
                {
                    if(other_vehicles[i].actual_drive_mode == Manual ||
                       other_vehicles[i].actual_drive_mode == Auto ||
                       other_vehicles[i].actual_drive_mode == Leader)
                       desire_drive_mode = Dequeue;
                } 
                else if(i == other_vehicles.size ()) 
                    desire_drive_mode = Dequeue;
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
    if (desire_drive_mode != old)
    {
        printf ("asdf desire_drive_mode changed: %d\n", desire_drive_mode);
    }
}

static bool
compare_relative_x (const VehicleData& a,  const VehicleData& b)
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
        if (m_debug_flags & DEBUG_CalculateID)
            printf ("other_vehicles: %ld\n", other_vehicles.size ());
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
#if 1
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
    if (_ID >= 2)
    {
        platoon_manager_info.front_vehicle = other_vehicles[_ID - 2];//_ID surely >= 2
        int front_id = other_vehicles[_ID - 2].vehicle_id;
        platoon_manager_info.front_frenet_dis = m_worldmodle_.GetFrenetDis(front_id);
    }
#else
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
#endif
    DataContainer::GetInstance ()->manager_data_.setData (platoon_manager_info);
}

} // namesapce manager

} // namespace platoon
