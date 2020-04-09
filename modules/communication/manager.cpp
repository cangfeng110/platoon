#include "modules/communication/manager.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <sys/time.h>

#include "modules/customfunction/functiontool.h"
#include "modules/communication/datadefine.h"
#include "modules/communication/fmsdata.h"



namespace platoon {

namespace communication {

void print_drive_mode(const DriveMode& mode)
{
    switch (mode)
    {
        case Manual:
            printf("Manual\n");
            break;
        case Auto:
            printf("Auto\n");
            break;
        case LeaderWait:
            printf("LeaderWait\n");
            break;
        case Leader:
            printf("Leader\n");
            break;
        case Enqueue:
            printf("Enqueue\n");
            break;
        case KeepQueue:
            printf("KeepQueue\n");
            break;
        case Dequeue:
            printf("Dequeue\n");
            break;
        case Abnormal:
            printf("Abnormal\n");
            break;
        case Notset:
            printf("NotSet\n");
            break;
        default:
            break;
    }
}

int TransLicensToId(const std::string& license)
{
    if (license == "沪YSG_18")
        return 18;
    else if (license == "沪YSG_19")
        return 19;
    else if (license == "沪YSG_20")
        return 20;
    else if (license == "沪YSG_21")
        return 21;
    else if (license == "沪YSG_22")
        return 22;
    else if (license == "沪YSG_17")
        return 17;
    else if (license == "沪YSG_14")
        return 14;
    else 
        return -1;
}

Manager::Manager () : actual_drive_mode(Manual), desire_drive_mode(Notset), 
                      _ID(0), m_fms_order_(F_Invalid), m_safe_distance_(10.0)
{
    other_vehicles.reserve (100);
    vehicle_status_.reserve(5);
    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_thw_HZ = ConfigData::GetInstance ()->GetDebugThwHZ ();
    if (m_debug_thw_HZ <= 0)
        m_debug_thw_HZ = 25;
    hmi_fms_valid_ = ConfigData::GetInstance()->hmi_fms_valid_;
    m_debug_StateFlow = ConfigData::GetInstance()->debug_StateFlow_;
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
    if (!DataContainer::GetInstance ()->ego_vehicle_vcu_data_.isUpToDate ())
    {
        return INVALID_FLOAT;
    }
    const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
    float speed = ego_vehicle_vcu_data.fSpeed;
    float thw;
    /* if (speed < 0.001)
    {
        return INVALID_FLOAT;
    } */
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
        return INVALID_FLOAT;
    }
    float front_dis = INVALID_FLOAT;
    int front_id = platoon_id_map_[_ID - 1];
    if (DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].isUpToDate())
    {
        VehicleData front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
        const VehicleGpsData& ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
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
        front_dis = front_vehicle.relative_x - vehicle_length;
    }
    else
    {
        if (DataContainer::GetInstance()->planning_data_.isUpToDate())
            front_dis = DataContainer::GetInstance()->planning_data_.getData().front_obstacle_distance;
    }
    if (front_dis <= 0)
        return INVALID_FLOAT;
    return front_dis;
}


static bool
compare_relative_x (const VehicleData& a,  const VehicleData& b)
{
    return (a.relative_x > b.relative_x);
}

void Manager::CalculateID ()
{
    _ID = 0;
    other_vehicles.clear();
    platoon_id_map_.clear();
    if (!DataContainer::GetInstance ()->ego_vehicle_gps_data_.isUpToDate ())
    {
        return;
    }
    const VehicleGpsData& ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
    if (DataContainer::GetInstance ()->platoon_vehicles_data_.isUpToDate ())
    {
        //other_vehicles.clear ();
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
    if (m_debug_flags & DEBUG_CalculateID)
    {
        printf(" cal_id is be execute, ego vehicle squence is : %d\n\n", _ID);
    }
}
float Manager::CalThreshold()
{
    if (DataContainer::GetInstance()->ego_vehicle_vcu_data_.isUpToDate())
    {
        const VehicleVcuData& ego_vehicle_vcu_data = DataContainer::GetInstance ()->ego_vehicle_vcu_data_.getData ();
        float threshold_dis = ego_vehicle_vcu_data.fSpeed * ConfigData::GetInstance ()->keep_mode_threshold_ 
                                  + m_safe_distance_;
        return threshold_dis;
    }
    else
    {
        return INVALID_FLOAT;
    }
}
/**
 * result: 
 * true present abnormal 
 * false present normal
*/
bool Manager::IfAbnormal()
{
    bool result = false;
    //if ego is leader, don't need to check abnormal status, return false
    if (_ID <= 1)
    {
       return false;
    }
    //first step: to judge ego cut in flag, if cut in flag true, abnormal
    if (!DataContainer::GetInstance()->planning_data_.isUpToDate())
    {
        if (ConfigData::GetInstance()->debug_StateFlow_)
        {
            std::cout << "Current Abnormal, No plan info" << std::endl;
        }
        return true;
    }       
    else 
    {
        if (DataContainer::GetInstance()->planning_data_.getData().cut_in == 1)
        {
            if (ConfigData::GetInstance()->debug_StateFlow_)
            {
                std::cout << "Current Abnormal, ego cut in flag true " << std::endl;
            }
            return true;
        }
            
    }
    //to judge ego vehicle gps status if abnormal
    if (!DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate())
    {
        if (ConfigData::GetInstance()->debug_StateFlow_)
        {
            std::cout << "Current Abnormal, No gps info" << std::endl;
        }
        return true;
    }
    else
    {
        if (DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().position_valid_flag_for_motorcade == 1)
        {
            if (ConfigData::GetInstance()->debug_StateFlow_)
            {
                std::cout << "Current Abnormal, gps abnormal flag true " << std::endl;
            }
            return true;
        }
    }
    
    // second step: to judge if platoon vehicle map has data, if no date, abnormal
    if (!DataContainer::GetInstance()->platoon_vehicles_data_.isUpToDate())
    {
        if (ConfigData::GetInstance()->debug_StateFlow_)
        {
            std::cout << "Current Abnormal, platoon map is empty" << std::endl;
        }
        return true;
    }
        
    //if ego is leader, don't need to check abnormal, return false
    for (int i = 1; i < _ID; i++)
    {
        int vehicle_id = platoon_id_map_[i];
        //third step: if platoon map has "vehicle_id" vehicle, if don't have, abnormal 
        auto it = DataContainer::GetInstance()->platoon_vehicles_data_.getData().find(vehicle_id);
        if (it != DataContainer::GetInstance()->platoon_vehicles_data_.getData().end())
        {
            //forth step: check "vehicle_id" vehicel status
            const VehicleData& temp = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[vehicle_id].getData();
            bool if_disconnect = !(DataContainer::GetInstance()->platoon_vehicles_data_.getData()[vehicle_id].isUpToDate());
            bool if_abnormal = false;
            bool if_cut_in =  false;
            bool if_gps_error = false;
            if (i == 1)
            {//leader vehicle don't need to check cut_in_flag and anbormal status
                if_abnormal = false;
                if_cut_in = false;
            }
            else
            {
                if_abnormal = (DriveMode(temp.actual_drive_mode) == Abnormal) ? true : false;
                if_cut_in = (temp.cut_in_flag == 1) ? true : false;
            }
            if (i == 1 || i == _ID -1)
            {//only leader and front vehicle need to check if gps error
                if_gps_error = (temp.gps_status == 1) ? true : false;
            }
            
            if (if_disconnect || if_abnormal || if_cut_in || if_gps_error)
            {
                if (ConfigData::GetInstance()->debug_StateFlow_)
                {   if (if_disconnect)
                    {
                        std::cout << "Current Abnormal, front vehicles disconnected : " << vehicle_id << std::endl;
                    }
                    if (if_abnormal)
                    {
                        std::cout << "Current Abnormal, front vehicles abnormal : " << vehicle_id << std::endl;
                    }
                    if (if_cut_in)
                    {
                        std::cout << "Current Abnormal, front vehicles cut in : " << vehicle_id << std::endl;
                    }     
                    if (if_gps_error)
                    {
                        std::cout << "Current Abnormal, front vehicles gps error : " << vehicle_id << std::endl;
                    }
                }
                return true;
            }      
        }
        else
        {
            if (ConfigData::GetInstance()->debug_StateFlow_)
            {
                std::cout << "Current Abnormal, front vehicles not in platoon map : " << vehicle_id << std::endl;
            }
            return true;
        }
    }
    return result;
}
/**
 * when fms enqueue and leader order is be executabe , 
 * this function is be recall to clear fms order, avoid repate enqueue / leader
 * dequeue or disband order don't need to be cleared
 * */
void Manager::ResetFmsOrder()
{
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        HmiFmsInfo reset = FmsData::GetInstance()->hmi_fms_info.getData();
        reset.fms_order = int8_t(F_Invalid);
        FmsData::GetInstance()->hmi_fms_info.setData(reset);
        FmsData::GetInstance()->fms_order_.setData(F_Invalid);
    } 
    else 
    {
        FmsData::GetInstance()->fms_order_.setData(F_Invalid);
    }
}

/**
 * function: to just if all vehicle join the platoon
 * true preset all vehicle join the platoon, the leader vehicle can changed to Leader;
 * false preset there is some vehicle not join in the platoon
*/

bool Manager::IsAllJoinPlatoon()
{
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        if (!DataContainer::GetInstance()->platoon_vehicles_data_.isUpToDate())
        {
            return true;
        } 
        for (auto temp : DataContainer::GetInstance()->platoon_vehicles_data_.getData())
        {
            if (DriveMode(temp.second.getData().actual_drive_mode) != KeepQueue)
            {
                if (ConfigData::GetInstance()->debug_StateFlow_)
                {
                    std::cout << "Refuse to be Leader " << int(temp.second.getData().vehicle_id) << " don't keep" 
                              << std::endl;
                }
                return false;
            }      
        }
    }
    else if (FmsData::GetInstance()->fms_pre_info_.isUpToDate())
    {
        const FMSPreFormationInfo& temp = FmsData::GetInstance()->fms_pre_info_.getData();
        int size = temp.platoonmember_size();
        for (int i = 0; i < size; i++)
        {
            int vehicle_id = TransLicensToId(temp.platoonmember(i));
            auto it = DataContainer::GetInstance()->platoon_vehicles_data_.getData().find(vehicle_id);
            if (it != DataContainer::GetInstance()->platoon_vehicles_data_.getData().end())
            {
                if (DriveMode(it->second.getData().actual_drive_mode) != KeepQueue)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }   
        }
    }
    return true;
}
/**
 * function: to just if ego vechile can dequeue
 * true preset allow dequeue;
 * false preset refuse dequeue;
*/
bool Manager::IsAllowDequeue()
{
    if (_ID <= platoon_id_map_.size())
    {
        int after_id = platoon_id_map_[_ID + 1];
        auto it = DataContainer::GetInstance()->platoon_vehicles_data_.getData().find(after_id);
        if (it != DataContainer::GetInstance()->platoon_vehicles_data_.getData().end())
        {
            DriveMode after_mode = DriveMode(DataContainer::GetInstance()->platoon_vehicles_data_.getData()[after_id].getData().actual_drive_mode);
            if (after_mode == Manual || after_mode == Auto 
                || after_mode == Leader || after_mode == LeaderWait
                || after_mode == Dequeue)
                return true;
            else 
            {
                if (ConfigData::GetInstance()->debug_StateFlow_)
                {
                    std::cout << "Refuse to be Leave, after vehicle " <<  int(it->second.getData().vehicle_id)  
                              << "drive mode is " << int(after_mode)  << std::endl;
                }
                return false;
            }
                
        }
        else
        {
            return true;
        }
    }
    else if (_ID > platoon_id_map_.size())
       return true;
}

void Manager::ProcessCommand ()
{
    static int debug_count = 0;
    debug_count++;
    if (!DataContainer::GetInstance ()->planning_data_.isUpToDate ())
    {
        if (m_debug_StateFlow)
        {
            if (debug_count % m_debug_thw_HZ == 0)
                printf("ERROR : plan info is gone!\n\n");
        }
        return;
    }
    DriveMode old_drive_mode = desire_drive_mode;
    const EgoPlanningMsg& ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    actual_drive_mode = (DriveMode)ego_planning_msg.actual_drive_mode;
    float thw_dis = THWDis ();
    float front_dis = FrontDis ();
    float threshold_dis = CalThreshold();

    if (FmsData::GetInstance()->hmi_fms_info.isUpToDate())
    {
        m_safe_distance_ = FmsData::GetInstance()->hmi_fms_info.getData().safe_distance;
    }
    if (m_debug_StateFlow)
    {
        if (debug_count % m_debug_thw_HZ == 0)
        {
            printf ("asdf thw dis is : %f, front dis is : %f, safe distance is : %f, threshold dis is: %f\n", 
                    thw_dis , front_dis, m_safe_distance_, threshold_dis); 
            printf ("acutla drive mode is : ");
            print_drive_mode(actual_drive_mode);
        }
    }
    /**
     * >>>>>>>>>>>
    */
    if (FmsData::GetInstance()->fms_order_.isUpToDate())
    {
        m_fms_order_ = FmsData::GetInstance()->fms_order_.getData();
    }
    // this is only for test,clear status
    if (m_fms_order_ == F_Reset && desire_drive_mode != Auto)
    {
        desire_drive_mode = Auto;
        if (m_debug_StateFlow)
        {
            if(debug_count % m_debug_thw_HZ)
                printf("Clear Status\n");
        }
        return;
    }
    switch (actual_drive_mode)
    {
        case Manual:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Manual\n\n");
            }
            //when vehicle is manual, desire must be manual, reset commond
            desire_drive_mode = Manual;
            ResetFmsOrder();
            break;
        case Auto:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Auto\n\n");
            }
            if (desire_drive_mode == Notset || desire_drive_mode == Manual)
            {
                desire_drive_mode = Auto;
            }
            if (m_fms_order_ == F_Leader)
            {
                desire_drive_mode = LeaderWait;
                ResetFmsOrder();
            }
            else if (m_fms_order_ == F_Enqueue)
            {
                if(_ID <= 1)
                {
                    std::cerr << "ERROR, this vehicle should not receive formation order\n";
                    break;
                }
                if(IfAbnormal())
                    break;

                int front_id = platoon_id_map_[_ID - 1];
                const VehicleData& front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
                DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode;
                if (front_drive_mode == Leader || front_drive_mode == LeaderWait
                    || front_drive_mode == KeepQueue || front_drive_mode == Enqueue)
                {
                    if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
                    {
                        break;
                    }   
                    if (front_dis <= thw_dis * ConfigData::GetInstance()->formation_threshold_)
                    {
                        desire_drive_mode = Enqueue;
                        // 
                        //ResetFmsOrder();
                    }
                }
            }
            break;
        case LeaderWait:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN LeaderWait\n\n");
            }
            if (IsAllJoinPlatoon())
            {
                desire_drive_mode = Leader;
            }
            break;
        case Leader:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Leader\n\n");
            }
            if (m_fms_order_ == F_Dequeue)
            {
                if (IsAllowDequeue())
                {
                    desire_drive_mode = Auto;
                }
            }
            else if (m_fms_order_ == F_DisBand)
            {
                desire_drive_mode = Auto;
            }
            break;
        case Enqueue: //add abnormal and dequeue
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Enqueue\n\n");
            }
            if (IfAbnormal())
            {
                desire_drive_mode = Abnormal;
            }  
            else if (m_fms_order_ == F_DisBand)
            {
                desire_drive_mode = Dequeue;
            }
            else if (m_fms_order_ == F_Dequeue)
            {
                if (IsAllowDequeue())
                {
                    desire_drive_mode = Dequeue;
                }
            }
            else
            {   
                //assert((_ID -1) > 0);
                if (_ID <= 1)
                {
                    std::cerr << "ERROR, this vehicle should not go to keep \n";
                    break;
                }
                int front_id = platoon_id_map_[_ID - 1];
                const VehicleData& front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
                DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode;
                if ( _ID > 2 && front_drive_mode != KeepQueue)
                    break;
                if (fabs(threshold_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
                {
                    break;
                }
                if (front_dis <= threshold_dis)
                {
                    desire_drive_mode = KeepQueue;
                }
            }
            break;
        case Dequeue:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Dequeue\n\n");
            }
            if (IfAbnormal())
            {
                desire_drive_mode = Abnormal;
            }
            else
            {
                if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
                {
                    break;
                }  
                if (front_dis >= thw_dis * ConfigData::GetInstance()->leave_threshold_)
                    desire_drive_mode = Auto;
            }
            break;
        case KeepQueue:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN KeepQueue\n\n");
            }
            if (IfAbnormal())
            {
                desire_drive_mode = Abnormal;
            }
            else
            {
                if (m_fms_order_ == F_Dequeue)
                {
                    if (IsAllowDequeue())
                    {
                        desire_drive_mode = Dequeue;
                    }
                }
                else if (m_fms_order_ == F_DisBand)
                {
                    desire_drive_mode = Dequeue;
                }
            }
            break;
        case CutIN:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN CutIN\n");
            }
        case Abnormal: //if fms order is F_Disband , no back to enqueue;
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Abnormal\n\n");
            }
            //leader vechile can't goto abnormal or cut_in 
            if (desire_drive_mode == Leader || desire_drive_mode == LeaderWait || desire_drive_mode == Manual)
                break;
            desire_drive_mode = Abnormal;
            if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
            {
                break;
            }  
            if (front_dis >= thw_dis * ConfigData::GetInstance()->to_auto_threshold_)
            {
                desire_drive_mode = Auto;
                /**
                 * from abnormal to auto present a task is over , need to clear fms enqueue oreder,
                 * invoid repeat enqueue
                */
                ResetFmsOrder();
            }
            else 
            {  
                if(!IfAbnormal())
                {
                    if (m_fms_order_ == F_DisBand || m_fms_order_ == F_Dequeue)
                    {
                        desire_drive_mode = Dequeue;
                    }
                    else if (m_fms_order_ == F_Enqueue)
                    {
                        desire_drive_mode = Enqueue;
                    }
                }
            }
            break;
        default:
            if (m_debug_StateFlow)
            {
                if (debug_count % m_debug_thw_HZ == 0)
                    printf("IN Default\n\n");
            }
            //ignore command
            break;
    }
    if (desire_drive_mode != old_drive_mode)
    {
        printf ("asdf desire_drive_mode changed : ");
        print_drive_mode(desire_drive_mode);
    }
}


void Manager::UpdatePlatoonManagerInfo ()
{
    // the id only can change in the Auto/manual mode
    if (actual_drive_mode == Auto || actual_drive_mode == Manual)
    {
         CalculateID ();
    }
    else if (m_debug_flags & DEBUG_CalculateID)
    {
        printf("id will be not chaned; vehicle sequence is : %d \n\n", _ID);
    }
    ProcessCommand ();
    m_worldmodle_.GetWorldmodleVehiles();

    if (_ID == 0)
    {
        if (m_debug_flags & DEBUG_ManagerInfo)
        {
            printf("++++++No Manager Info++++++\n");
        }
        return;//no ego_vehicle_location
    }
    PlatoonManagerInfo platoon_manager_info;
    platoon_manager_info.desire_drive_mode = int8_t(desire_drive_mode);
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        if (FmsData::GetInstance()->hmi_fms_info.isUpToDate())
            platoon_manager_info.platoon_number = int8_t(FmsData::GetInstance()->hmi_fms_info.getData().platoon_number);
        else 
            platoon_manager_info.platoon_number = 0;
    }
    else
    {
        if ( FmsData::GetInstance()->fms_pre_info_.isUpToDate())
            platoon_manager_info.platoon_number = int8_t(FmsData::GetInstance()->fms_pre_info_.getData().platoonnumber());
        else 
            platoon_manager_info.platoon_number = 0;
    } 
    platoon_manager_info.vehicle_sequence = _ID;
    platoon_manager_info.vehicle_num = platoon_id_map_.size() + 1; //include ego vehicle
    platoon_manager_info.leader_frenet_dis = 1.0e10;
    platoon_manager_info.front_frenet_dis = 1.0e10;
    //assign leader/front vehicle id 
    platoon_manager_info.leader_vehicle.vehicle_id = -1;
    platoon_manager_info.front_vehicle.vehicle_id = -1;
    
    //safe distance
    platoon_manager_info.safe_distance = m_safe_distance_;

    if (m_debug_flags & DEBUG_ManagerInfo)
    {
        using namespace std;
        cout << "+++++++++++++Display Manager ifno+++++++++++++" << endl;
        printf (" desire_drive_mode is : ");
        print_drive_mode(DriveMode(platoon_manager_info.desire_drive_mode));
        printf ("platoon number is : %d\n",platoon_manager_info.platoon_number);
        printf ("vehicle number is : %d\n",platoon_manager_info.vehicle_num);
        printf ("vehicle sequence is : %d\n", platoon_manager_info.vehicle_sequence);
        printf ("hmi safe distance is : %f\n",platoon_manager_info.safe_distance);
        
    }
    /**
     * updata leader front vehicle info
    */
    if (_ID > 1)
    {
        // only when ID >= 2, there has front / leader vehicle  
        int leader_id = platoon_id_map_[1];
        int front_id = platoon_id_map_[_ID - 1];

        bool leader_valid = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[leader_id].isUpToDate();
        bool front_valid = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].isUpToDate();

        if (leader_valid)
        {
            VehicleData leader_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[leader_id].getData();
            if (DataContainer::GetInstance ()->ego_vehicle_gps_data_.isUpToDate ())
            {
                const VehicleGpsData& ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (leader_vehicle.relative_x, leader_vehicle.relative_y,
                                                                    ego_vehicle_location.heading,
                                                                    ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                    ego_vehicle_location.height,
                                                                    leader_vehicle.longitude, leader_vehicle.latitude,
                                                                    leader_vehicle.altitude);
                platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (leader_vehicle.relative_heading,
                                                                    ego_vehicle_location.heading,
                                                                    leader_vehicle.heading);
            }
            platoon_manager_info.leader_vehicle = leader_vehicle;
            platoon_manager_info.leader_frenet_dis = m_worldmodle_.GetFrenetDis(leader_id);
            if (m_debug_flags & DEBUG_ManagerInfo)
            {   
                printf ("leader vehicle platoon number is : %d\n", platoon_manager_info.leader_vehicle.platoon_number);
                printf ("leader vehicle seuqence is : %d\n", platoon_manager_info.leader_vehicle.vehicle_sequence);
                printf ("leader vehicle id is : %d\n", platoon_manager_info.leader_vehicle.vehicle_id);
                printf ("leader vehicle gps_time is : %f\n", platoon_manager_info.leader_vehicle.gps_time);
                printf ("leader vehicle longitude is : %f\n", platoon_manager_info.leader_vehicle.longitude);
                printf ("leader vehicle latitude is  : %f\n", platoon_manager_info.leader_vehicle.latitude);
                printf ("leader vehicle altitude is  : %f\n",platoon_manager_info.leader_vehicle.altitude);
                printf ("leader vehicle heading is(rad) : %f\n", platoon_manager_info.leader_vehicle.heading);
                printf ("leader vehicle speed is(km/h) : %f\n", platoon_manager_info.leader_vehicle.speed * 3.6);
                printf ("leader vehicle acc is : %f\n", platoon_manager_info.leader_vehicle.longtitude_acc);
                printf ("leader vehicle relative_x is : %f\n",platoon_manager_info.leader_vehicle.relative_x);
                printf ("leader vehicle relative_y is : %f\n\n",platoon_manager_info.leader_vehicle.relative_y);
            }
        }
            
        if (front_valid)
        {
            VehicleData front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
            if (DataContainer::GetInstance ()->ego_vehicle_gps_data_.isUpToDate ())
            {
                const VehicleGpsData& ego_vehicle_location = DataContainer::GetInstance ()->ego_vehicle_gps_data_.getData ();
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (front_vehicle.relative_x, front_vehicle.relative_y,
                                                                    ego_vehicle_location.heading,
                                                                    ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                    ego_vehicle_location.height,
                                                                    front_vehicle.longitude, front_vehicle.latitude,
                                                                    front_vehicle.altitude);
                platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (front_vehicle.relative_heading,
                                                                    ego_vehicle_location.heading,
                                                                    front_vehicle.heading);
            }
            platoon_manager_info.front_vehicle = front_vehicle;
            platoon_manager_info.front_frenet_dis = m_worldmodle_.GetFrenetDis(front_id);
            if (m_debug_flags & DEBUG_ManagerInfo)
            {
                printf ("front vehicle platoon number is : %d\n", platoon_manager_info.front_vehicle.platoon_number);
                printf ("front vehicle seuqence is : %d\n", platoon_manager_info.front_vehicle.vehicle_sequence);
                printf ("front vehicle id is : %d\n", platoon_manager_info.front_vehicle.vehicle_id);
                printf ("front vehicle gps_time is : %f\n", platoon_manager_info.front_vehicle.gps_time);
                printf ("front vehicle longitude is : %f\n", platoon_manager_info.front_vehicle.longitude);
                printf ("front vehicle latitude is  : %f\n", platoon_manager_info.front_vehicle.latitude);
                printf ("front vehicle altitude is  : %f\n",platoon_manager_info.front_vehicle.altitude);
                printf ("front vehicle heading is(rad) : %f\n", platoon_manager_info.front_vehicle.heading);
                printf ("front vehicle speed is(km/h) : %f\n", platoon_manager_info.front_vehicle.speed * 3.6);
                printf ("front vehicle acc is : %f\n", platoon_manager_info.front_vehicle.longtitude_acc);
                printf ("front vehicle relative_x is : %f\n",platoon_manager_info.front_vehicle.relative_x);
                printf ("front vehicle relative_y is : %f\n\n",platoon_manager_info.front_vehicle.relative_y);
            }
        }
    }     
    /**
     * judge other vehicle info if valid
    */
    vehicle_status_.clear();
    for (auto map_it : platoon_id_map_)
    {
        int vehicle_id = map_it.second;
        auto fd_it = DataContainer::GetInstance()->platoon_vehicles_data_.getData().find(vehicle_id);
        if (fd_it != DataContainer::GetInstance()->platoon_vehicles_data_.getData().end())
        {
            if (fd_it->second.isUpToDate())
            {
                vehicle_status_.push_back(vehicle_id);
                if (m_debug_flags & DEBUG_V2XCommunication)
                {
                    struct timeval tv;
                    gettimeofday (&tv, NULL);
                    printf ("\nV %d: has data %ld.%ld\n\n", vehicle_id, tv.tv_sec, tv.tv_usec);
                }
            }
            else
            {
                vehicle_status_.push_back(0 - vehicle_id);
                if (m_debug_flags & DEBUG_V2XCommunication)
                {
                    struct timeval tv;
                    gettimeofday (&tv, NULL);
                    printf ("V %d: lost>500 %ld.%ld\n\n", vehicle_id, tv.tv_sec, tv.tv_usec);
                }
            }
        }
        else
        {
            vehicle_status_.push_back(0 - vehicle_id);
            if (m_debug_flags & DEBUG_V2XCommunication)
            {
                struct timeval tv;
                gettimeofday (&tv, NULL);
                printf ("V %d: lost>500 %ld.%ld\n\n", vehicle_id, tv.tv_sec, tv.tv_usec);
            }
        }
    }
  /*for (auto map_it : DataContainer::GetInstance()->platoon_vehicles_data_.getData())
    {
        if (map_it.second.isUpToDate())
        {
            vehicle_status_.push_back(map_it.second.getData().vehicle_id);
            if (m_debug_flags & DEBUG_V2XCommunication)
            {
                struct timeval tv;
                gettimeofday (&tv, NULL);
                printf ("\nV %d: has data %ld.%ld\n\n", map_it.second.getData().vehicle_id, tv.tv_sec, tv.tv_usec);
            }
        }
        else
        {
            vehicle_status_.push_back(0 - map_it.second.getData().vehicle_id);
            if (m_debug_flags & DEBUG_V2XCommunication)
            {
                struct timeval tv;
                gettimeofday (&tv, NULL);
                printf ("V %d: lost>500 %ld.%ld\n\n", map_it.second.getData().vehicle_id, tv.tv_sec, tv.tv_usec);
            }
        }      
    } */
    // add ego vehicel id in the vector end
    vehicle_status_.push_back(ConfigData::GetInstance()->vehicle_id_);
    
    platoon_manager_info.vehicle_communication_status = vehicle_status_;
    
    //add to sensor fusion vehicle info
    DataContainer::GetInstance ()->manager_data_.setData (platoon_manager_info);
}

} // namesapce manager

} // namespace platoon
