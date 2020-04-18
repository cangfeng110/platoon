#include "modules/communication/manager.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <sys/time.h>

#include "modules/customfunction/functiontool.h"
#include "modules/communication/datadefine.h"
#include "modules/communication/highfredatacontainer.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/udpdatacontainer.h"
#include "modules/communication/senddatacontanier.h"



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

Manager::Manager () : actual_drive_mode_(Manual), desire_drive_mode_(Notset), 
                      ID_(0), m_fms_order_(F_Invalid), m_safe_distance_(10.0)
{
    other_vehicles_.reserve (100);
    vehicle_status_.reserve(5);
    m_debug_flags_ = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_thw_HZ_ = ConfigData::GetInstance ()->GetDebugThwHZ ();
    if (m_debug_thw_HZ_ <= 0)
        m_debug_thw_HZ_ = 25;
    hmi_fms_valid_ = ConfigData::GetInstance()->hmi_fms_valid_;
    m_debug_StateFlow_ = ConfigData::GetInstance()->debug_StateFlow_;
}

Manager::~Manager () 
{
}

void Manager::UpdateInfo()
{
    ego_gps_info_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(ego_gps_isupdate_);
    ego_vcu_info_ = HighFreDataContainer::GetInstance()->ego_vehicle_vcu_data_.getData(ego_vcu_isupdate_);
    platoon_vehicles_info_ = UDPDataContainer::GetInstance()->platoon_vehicles_data_.getData(platoon_info_isupdate_);
    planning_info_ = LowFreDataContanier::GetInstance()->planning_data_.getData(plan_info_isupdate_);
    hmi_fms_info_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(hmi_isupdate_);
    fms_pre_info_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(pre_info_isupdate_);
    m_fms_order_ = SendDataContanier::GetInstance()->fms_order_.getData(fms_order_isupdate_);
    if (!fms_order_isupdate_)
    {
        m_fms_order_ = F_Invalid;
    }
    if (hmi_isupdate_)
    {
        m_safe_distance_ = hmi_fms_info_.safe_distance;
    }
        
}

float Manager::THWDis ()
{
    if (!ego_vcu_isupdate_)
    {
        return INVALID_FLOAT;
    }
    float speed = ego_vcu_info_.fSpeed;
    float thw;
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
    if(ID_ <= 1)
    {
        return INVALID_FLOAT;
    }
    float front_dis = INVALID_FLOAT;
    int front_id = platoon_id_map_[ID_ - 1];
    if (platoon_vehicles_info_[front_id].isUpToDate())
    {
        VehicleData front_vehicle = platoon_vehicles_info_[front_id].getData();
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (front_vehicle.relative_x, front_vehicle.relative_y,
                                                                    ego_gps_info_.heading,
                                                                    ego_gps_info_.longitude, ego_gps_info_.latitude,
                                                                    ego_gps_info_.height,
                                                                    front_vehicle.longitude, front_vehicle.latitude,
                                                                    front_vehicle.altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (front_vehicle.relative_heading,
                                                                    ego_gps_info_.heading,
                                                                    front_vehicle.heading);
        float vehicle_length = ConfigData::GetInstance()->vehicle_length_;
        front_dis = front_vehicle.relative_x - vehicle_length;
    }
    else
    {
        if (plan_info_isupdate_)
            front_dis =planning_info_.front_obstacle_distance;
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
    ID_ = 0;
    other_vehicles_.clear();
    platoon_id_map_.clear();
    if (!ego_gps_isupdate_)
    {
        return;
    }
    if (platoon_info_isupdate_)
    {
        //other_vehicles.clear ();
        for (auto map_it : platoon_vehicles_info_)
        {
            VehicleData v2x_other_vehicle_data = map_it.second.getData ();
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                                 ego_gps_info_.heading,
                                                                 ego_gps_info_.longitude, ego_gps_info_.latitude,
                                                                 ego_gps_info_.height,
                                                                 v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                 v2x_other_vehicle_data.altitude);
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (v2x_other_vehicle_data.relative_heading,
                                                                   ego_gps_info_.heading,
                                                                   v2x_other_vehicle_data.heading);
            other_vehicles_.push_back (v2x_other_vehicle_data);
        }
        std::sort (other_vehicles_.begin (), other_vehicles_.end (), compare_relative_x);
        
        /**
         * first traversal storage other vehicle's platoon id in the map 
         **/
        for (int i = 0; i < other_vehicles_.size (); ++i)
        {
            if (other_vehicles_[i].relative_x > 0.0)
            {
                int temp_key = i + 1;
                platoon_id_map_[temp_key] = other_vehicles_[i].vehicle_id;
            } 
            else if (other_vehicles_[i].relative_x < 0.0)
            {
                int temp_key = i + 2;
                platoon_id_map_[temp_key] = other_vehicles_[i].vehicle_id;
            }
        }
        // calculate ego platoon id
        for (ID_ = 0; ID_ < other_vehicles_.size(); ++ID_)
        {
            if (other_vehicles_[ID_].relative_x < 0.0)
            {
                break;
            }
        }  
    }
    ++ID_;//if no platoon_vehicles_data or ego is the first _ID = 1
    if (m_debug_flags_ & DEBUG_CalculateID)
    {
        printf(" cal_id is be execute, ego vehicle squence is : %d\n\n", ID_);
    }
}
float Manager::CalThreshold()
{
    if (ego_vcu_isupdate_)
    {
        double threshold_dis = ego_vcu_info_.fSpeed * ConfigData::GetInstance ()->keep_mode_threshold_ 
                                  + m_safe_distance_;
        // the min follow dis is 10, so the threshold should bigger than 10
        threshold_dis = std::max(threshold_dis, 12.0);
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
    if (ID_ <= 1)
    {
       return false;
    }
    //first step: to judge ego cut in flag, if cut in flag true, abnormal
    if (!plan_info_isupdate_)
    {
        if (ConfigData::GetInstance()->debug_StateFlow_)
        {
            std::cout << "Current Abnormal, No plan info" << std::endl;
        }
        return true;
    }       
    else 
    {
        if (planning_info_.cut_in == 1)
        {
            if (ConfigData::GetInstance()->debug_StateFlow_)
            {
                std::cout << "Current Abnormal, ego cut in flag true " << std::endl;
            }
            return true;
        }
            
    }
    //to judge ego vehicle gps status if abnormal
    if (!ego_gps_isupdate_)
    {
        if (ConfigData::GetInstance()->debug_StateFlow_)
        {
            std::cout << "Current Abnormal, No gps info" << std::endl;
        }
        return true;
    }
    else
    {
        if (ego_gps_info_.position_valid_flag_for_motorcade == 0)
        {
            if (ConfigData::GetInstance()->debug_StateFlow_)
            {
                std::cout << "Current Abnormal, gps error" << std::endl;
            }
            return true;
        }
    }
    
    // second step: to judge if platoon vehicle map has data, if no date, abnormal
    if (!platoon_info_isupdate_)
    {
        if (ConfigData::GetInstance()->debug_StateFlow_)
        {
            std::cout << "Current Abnormal, platoon map is empty" << std::endl;
        }
        return true;
    }
        
    //to check front vehicles
    for (int i = 1; i < ID_; i++)
    {
        int vehicle_id = platoon_id_map_[i];
        //forth step: check "vehicle_id" vehicel status
        bool if_disconnect = !(platoon_vehicles_info_[vehicle_id].isUpToDate());
        if (if_disconnect) //if is disconnect, the data in map[vehicle_id] is not valid, blow check is not need to do
        {
            if (m_debug_StateFlow_)
            {
                std::cout << "Current Abnormal, front vehicles disconnected : " << vehicle_id << std::endl;
            }
            return true;
        }   
        const VehicleData& temp = platoon_vehicles_info_[vehicle_id].getData();

        bool if_gps_error = (temp.gps_status == 0) ? true : false;
        if (if_gps_error)
        {
            if (m_debug_StateFlow_)
            {
                std::cout << "Current Abnormal, front vehicles gps error : " << vehicle_id << std::endl;
            }
            return true;
        }    
        bool if_abnormal = false;
        bool if_cut_in =  false;
        bool if_manual = false;
        if (i > 1)//leader vehicle don't need to check cut_in_flag and anbormal status
        {
            if_abnormal = (DriveMode(temp.actual_drive_mode) == Abnormal) ? true : false;
            if_cut_in = (temp.cut_in_flag == 1) ? true : false;
            if_manual = (DriveMode(temp.actual_drive_mode) == Manual) ? true : false;
        }
        if (if_abnormal || if_cut_in || if_manual)
        {
            if (m_debug_StateFlow_)
            {  
                if (if_abnormal)
                {
                    std::cout << "Current Abnormal, front vehicles abnormal : " << vehicle_id << std::endl;
                }
                if (if_cut_in)
                {
                    std::cout << "Current Abnormal, front vehicles cut in : " << vehicle_id << std::endl;
                }   
                if (if_manual)
                {
                    std::cout << "Current Abnormal, front vehicles manual : " << vehicle_id << std::endl;
                }
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
        hmi_fms_info_.fms_order = int8_t(F_Invalid);
        LowFreDataContanier::GetInstance()->hmi_fms_info_.setData(hmi_fms_info_);
        SendDataContanier::GetInstance()->fms_order_.setData(F_Invalid);
    } 
    else 
    {
        SendDataContanier::GetInstance()->fms_order_.setData(F_Invalid);
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
        if (!platoon_info_isupdate_)
        {
            return true;
        } 
        for (auto temp : platoon_vehicles_info_)
        {
            if (DriveMode(temp.second.getData().actual_drive_mode) != KeepQueue)
            {
                if (m_debug_StateFlow_)
                {
                    std::cout << "Refuse to be Leader " << int(temp.second.getData().vehicle_id) << " don't keep" 
                              << std::endl;
                }
                return false;
            }      
        }
    }
    else if (pre_info_isupdate_)
    {
        int size = fms_pre_info_.platoonmember_size();
        for (int i = 0; i < size; i++)
        {
            int vehicle_id = TransLicensToId(fms_pre_info_.platoonmember(i));
            auto it = platoon_vehicles_info_.find(vehicle_id);
            if (it != platoon_vehicles_info_.end())
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
/**
 * get front drive mode, leader vehicle mustn't call this function
*/
DriveMode Manager::FrontMode()
{
    //leader vehicle don't have front vehicle
    if (_ID > 1)
    {
        int front_id = platoon_id_map_[_ID - 1];
        const VehicleData& front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
        DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode;
        return front_drive_mode;
    }
    else
    {
        return Notset;
    }
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
        desire_drive_mode = Notset;
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
            //when vehicle is manual, desire must be manual,  not leader need reset commond
            if (_ID > 1)
            {
                ResetFmsOrder();
            }     
            desire_drive_mode = Manual;
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
               //ResetFmsOrder();
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
                DriveMode front_drive_mode = FrontMode();
                /* int front_id = platoon_id_map_[_ID - 1];
                const VehicleData& front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
                DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode; */
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
                DriveMode front_drive_mode = FrontMode();
                /* int front_id = platoon_id_map_[_ID - 1];
                const VehicleData& front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
                DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode; */
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
                 * invoid repeat enqueue, because in auto, id will be cal.
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
                        DriveMode front_drive_mode = FrontMode();
                       /*  int front_id = platoon_id_map_[_ID - 1];
                        const VehicleData& front_vehicle = DataContainer::GetInstance()->platoon_vehicles_data_.getData()[front_id].getData();
                        DriveMode front_drive_mode = (DriveMode)front_vehicle.actual_drive_mode; */
                        if ( front_drive_mode == Leader || front_drive_mode == Enqueue 
                              || front_drive_mode == KeepQueue || front_drive_mode == LeaderWait)
                        {
                            desire_drive_mode = Enqueue;
                        }     
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
        cout << "+++++++++++++Display Manager info+++++++++++++" << endl;
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
