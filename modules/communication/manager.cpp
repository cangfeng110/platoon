#include "modules/communication/manager.h"

#include <algorithm>
#include <math.h>
#include <sys/time.h>

#include "modules/customfunction/functiontool.h"
#include "modules/communication/datadefine.h"
#include "modules/communication/fmsdata.h"



namespace platoon {

namespace communication {

Manager::Manager () : actual_drive_mode(Manual), desire_drive_mode(Notset), 
                      _ID(0), m_fms_order_(F_Invalid), m_safe_distance_(0.0)
{
    other_vehicles.reserve (100);
    vehicle_status_.reserve(5);
    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_thw_HZ = ConfigData::GetInstance ()->GetDebugThwHZ ();
    if (m_debug_thw_HZ <= 0)
        m_debug_thw_HZ = 25;
    hmi_fms_valid_ = ConfigData::GetInstance()->hmi_fms_valid_;
}

Manager::~Manager () {
}

/* void Manager::SetFmsInfo (const FmsInfo& msg)
{
    if (m_fms_info.fms_order != msg.fms_order)
    {
        printf ("asdf FMS_INFO fms_order changed: %d\n", msg.fms_order);
    }
    m_fms_info = msg;
} */

float Manager::THWDis ()
{
    /* if (!DataContainer::GetInstance ()->ego_vehicle_vcu_data_.isUpToDate ())
    {
        return INVALID_FLOAT;
    } */
    const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
    float speed = ego_vehicle_vcu_data.fSpeed;
    float thw;
    if (speed < 0.001)
    {
        return INVALID_FLOAT;
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
    float thw_dis = thw * speed + 15;
    return thw_dis;
}

float Manager::FrontDis ()
{
    if(_ID <= 1)
    {
        if (m_debug_flags & DEBUG_TimeToFront)
            printf ("_ID : %d\n", _ID);
        return INVALID_FLOAT;
    }
    int front_id = platoon_id_map_[_ID - 1];
    VehicleData front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
    const VehicleGpsData ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
    platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (front_vehicle.relative_x, front_vehicle.relative_y,
                                                                 ego_vehicle_location.heading,
                                                                 ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                 ego_vehicle_location.height,
                                                                 front_vehicle.longitude, front_vehicle.latitude,
                                                                 front_vehicle.altitude);
    platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (front_vehicle.relative_heading,
                                                                   ego_vehicle_location.heading,
                                                                   front_vehicle.heading);
    float vehicle_length = ConfigData::GetInstance()->vehicle_length_;
    float front_dis = front_vehicle.relative_x - vehicle_length;
    if (front_dis <= 0)
        return INVALID_FLOAT;
    return front_dis;
    //VehicleData front_vehicle = other_vehicles[_ID - 2];
    /* const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
    float speed = ego_vehicle_vcu_data.fSpeed;
    if (m_debug_flags & DEBUG_TimeToFront)
        printf ("speed %f\n", speed);
    if (speed < 0.001)
    {
        return INVALID_FLOAT;
    }

    if (m_debug_flags & DEBUG_TimeToFront)
        printf ("relative_x: %f\n", front_vehicle.relative_x);
    return fabs(front_vehicle.relative_x - 17.0) / speed; */
}

void Manager::SetSafeDis(float safe_dis)
{
    m_safe_distance_ = safe_dis;
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
    const VehicleGpsData ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
    if (DataContainer::GetInstance ()->platoon_vehicles_data_.isUpToDate ())
    {
        other_vehicles.clear ();
        for (auto map_it : DataContainer::GetInstance ()->platoon_vehicles_data_.getData ())
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
        
        /**
         * first traversal storage other vehicle's platoon id in the map 
         **/
        for (int i = 0; i < other_vehicles.size (); i++)
        {
            if (other_vehicles[i].relative_x > 0.0)
            {
                int temp_key = i + 1;
                platoon_id_map_[temp_key] = other_vehicles[i].vehicle_id;
            } 
            else if (other_vehicles[i].relative_x < 0.0)
            {
                int temp_key = i + 2;
                platoon_id_map_[temp_key] = other_vehicles[i].vehicle_id;
            }
        }
        // calculate ego platoon id
        for (_ID = 0; _ID < other_vehicles.size(); _ID++)
        {
            if (other_vehicles[_ID].relative_x < 0.0)
            {
                break;
            }
        }  
    }
    ++_ID;//if no platoon_vehicles_data or ego is the first _ID = 1
}

/**
 * result: 
 * true present abnormal 
 * false present normal
*/
bool Manager::IfAbnormal()
{
    bool result = false;
    for (int i = 1; i < _ID; i++)
    {
        int vehicle_id = platoon_id_map_[i];
        const VehicleData temp = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[vehicle_id].getData();
        bool if_disconnect = !(DataContainer::GetInstance()->platoon_vehicles_data_.getData()[vehicle_id].isUpToDate());
        bool if_abnormal = (DriveMode(temp.actual_drive_mode) == Abnormal) ? true : false;
        bool if_cut_in = (temp.cut_in_flag == 1) ? true : false;
        if (if_disconnect || if_abnormal || if_cut_in)
            return true;
    }
    return result;
}

void Manager::ProcessCommand ()
{
    if (!DataContainer::GetInstance ()->planning_data_.isUpToDate ())
    {
        return;
    }
    DriveMode old_drive_mode = desire_drive_mode;
    EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    actual_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
    float thw_dis = THWDis ();
    float front_dis = FrontDis ();
    static int debug_count = 0;
    debug_count++;
    if (debug_count % m_debug_thw_HZ == 0)
    {
        printf ("asdf thw dis is : %f, front dis is : %f\n", thw_dis , front_dis);
    }

    /**
     * >>>>>>>>>>>
    */
    m_fms_order_ = F_DisBand;

    switch (actual_drive_mode)
    {
        case Manual:
            //ignore command
            break;
        case Auto:
            if (m_fms_order_ == F_Leader)
            {
                desire_drive_mode = Leader;
            }
            else if (m_fms_order_ == F_Enqueue)
            {
                if(_ID <= 1)
                    break;
                if(IfAbnormal())
                    break;

                int front_id = platoon_id_map_[_ID - 1];
                VehicleData front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
                DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode;
                if (front_drive_mode == Leader || front_drive_mode == KeepQueue || front_drive_mode == Enqueue)
                {
                    if (front_dis <= 1.2 * thw_dis)
                    {
                        desire_drive_mode = Enqueue;
                    }
                    /* if (fabs(thw - INVALID_FLOAT) < Epslion || fabs(time_to_front - INVALID_FLOAT) < Epslion)
                    {
                        break;
                    }
                    if (time_to_front <= 1.2 * thw)
                    {
                        desire_drive_mode = Enqueue;
                    } */
                }
            }
            break;
        case Leader:
            if (m_fms_order_ == F_Dequeue || m_fms_order_ == F_DisBand)
            {
                desire_drive_mode = Auto;
            }
            break;
        case Enqueue:
            const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
            float threshold_dis = ego_vehicle_vcu_data.fSpeed * ConfigData::GetInstance ()->keep_mode_threshold_ 
                                  + m_safe_distance_;
            if (front_dis <= threshold_dis)
            {
                desire_drive_mode = KeepQueue;
            }
            /* if (fabs(thw - INVALID_FLOAT) < Epslion || fabs(time_to_front - INVALID_FLOAT) < Epslion)
            {
                break;
            }
            if (time_to_front <= ConfigData::GetInstance ()->keep_mode_threshold_)
            {
                desire_drive_mode = KeepQueue;
            } */
            break;
        case Dequeue:
            if (front_dis >= thw_dis)
                desire_drive_mode = Auto;
            /* if (fabs(thw - INVALID_FLOAT) < Epslion || fabs(time_to_front - INVALID_FLOAT) < Epslion)
            {
                break;
            }
            if (time_to_front >= thw)
            {
                desire_drive_mode = Auto;
            } */
            break;
        case KeepQueue:
            if (m_fms_order_ == F_Dequeue)
            {
               /*  int i = _ID - 1;
                if (i < other_vehicles.size ())
                {
                    if(other_vehicles[i].actual_drive_mode == Manual ||
                       other_vehicles[i].actual_drive_mode == Auto ||
                       other_vehicles[i].actual_drive_mode == Leader)
                       desire_drive_mode = Dequeue;
                } 
                else if(i == other_vehicles.size ())  */
                if (_ID < platoon_id_map_.size())
                {
                    int after_id = platoon_id_map_[_ID + 1];
                    DriveMode after_mode = DriveMode(DataContainer::GetInstance()->platoon_vehicles_data_.getData()[after_id].getData().actual_drive_mode);
                    if (after_mode == Manual || after_mode == Auto || after_mode == Leader)
                        desire_drive_mode = Dequeue;
                }
                else if (_ID > platoon_id_map_.size())
                    desire_drive_mode = Dequeue;
            }
            else if (m_fms_order_ == F_DisBand)
            {
                desire_drive_mode = Dequeue;
            }
            break;
        case Abnormal:
        default:
            //ignore command
            break;
    }
    if (desire_drive_mode != old_drive_mode)
    {
        printf ("asdf desire_drive_mode changed: %d\n", desire_drive_mode);
    }
}



void Manager::UpdatePlatoonManagerInfo ()
{
    // the id only can change in the Auto mode
    if (actual_drive_mode == Auto)
    {
         CalculateID ();
    }
    ProcessCommand ();
    m_worldmodle_.GetWorldmodleVehiles();

    if (_ID == 0)
    {
        return;//no ego_vehicle_location
    }
    PlatoonManagerInfo platoon_manager_info;
    platoon_manager_info.desire_drive_mode = desire_drive_mode;
    platoon_manager_info.vehicle_num = _ID - 1;
    platoon_manager_info.leader_frenet_dis = 1.0e10;
    platoon_manager_info.front_frenet_dis = 1.0e10;
    //assign leader/front vehicle id 
    platoon_manager_info.leader_vehicle.vehicle_id = -1;
    platoon_manager_info.front_vehicle.vehicle_id = -1;
#if 1
    /* int i;
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
    } */
    if (_ID == 1) //ego is leader
        return;
    // ID >= 2
    int leader_id = platoon_id_map_[1];
    int front_id = platoon_id_map_[_ID - 1];
    bool leader_invalid = DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()[leader_id].isUpToDate();
    bool front_invalid = DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()[front_id].isUpToDate();
    if (leader_invalid)
        platoon_manager_info.leader_vehicle = DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()[leader_id].getData();
    if (front_invalid)
        platoon_manager_info.front_vehicle = DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()[front_id].getData();
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
