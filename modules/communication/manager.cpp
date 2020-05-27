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
        case SubLeader:
            printf("SubLeader\n");
            break;
         case CutIN:
            printf("CutIN\n");
            break;
        default:
            break;
    }
}


Manager::Manager () : actual_drive_mode_(Manual), desire_drive_mode_(Notset), 
                      ID_(0), m_fms_order_(F_Invalid), m_safe_distance_(10.0),
                      hmi_safe_dis_(10.0), fms_safe_dis_(10.0),platoon_number_(0)
{
    other_vehicles_.reserve (100);
    vehicle_status_.reserve(5);
    m_debug_flags_ = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_thw_HZ_ = ConfigData::GetInstance ()->GetDebugThwHZ ();
    if (m_debug_thw_HZ_ <= 0)
        m_debug_thw_HZ_ = 25;
    m_debug_StateFlow_ = ConfigData::GetInstance()->debug_StateFlow_;
}


void Manager::UpdateInfo()
{
    ego_gps_info_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(ego_gps_isupdate_);
    ego_vcu_info_ = HighFreDataContainer::GetInstance()->ego_vehicle_vcu_data_.getData(ego_vcu_isupdate_);  
    platoon_vehicles_info_ = UDPDataContainer::GetInstance()->platoon_vehicles_data_.getData(platoon_info_isupdate_);
    v2x_vehicles_info_ = UDPDataContainer::GetInstance()->v2x_other_vehicles_data_.getData(v2x_info_isupdate_);
    planning_info_ = LowFreDataContanier::GetInstance()->planning_data_.getData(plan_info_isupdate_);
    hmi_fms_info_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(hmi_isupdate_);
    fms_pre_info_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(pre_info_isupdate_);
    m_fms_order_ = SendDataContanier::GetInstance()->fms_order_.getData(fms_order_isupdate_);
    platoon_number_ = SendDataContanier::GetInstance()->platoon_number_.getData(plnumber_isupdate_);
    if (!fms_order_isupdate_)
    {
        m_fms_order_ = F_Invalid;
    }
    // if (hmi_isupdate_)
    // {
    //     m_safe_distance_ = hmi_fms_info_.safe_distance;
    // }
    if (plan_info_isupdate_)
    {
        actual_drive_mode_ = DriveMode(planning_info_.actual_drive_mode);
    }
    if (!plnumber_isupdate_)
    {
        platoon_number_ = 0;
    }

    // std::cout << "gps is update : " << ego_gps_isupdate_ << std::endl
    //           << "vcu is update : " << ego_vcu_isupdate_ << std::endl
    //           << "platoon info is update : " << platoon_info_isupdate_ << std::endl
    //           << " planning info is update : " <<  plan_info_isupdate_ << std::endl
    //           << "hmi_fms_info is update : " << hmi_isupdate_ <<std::endl
    //           << "m_fms_order_ is update : " << m_fms_order_ << std::endl;
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
    int leader_map_index = 1;
    int leader_id = FindLeader(leader_map_index);   
    // to check leader vehicle's connection and gps status
    if (platoon_vehicles_info_[leader_id].isUpToDate()) // connection
    {
        const VehicleData& leader_info = platoon_vehicles_info_[leader_id].getData();
        if (leader_info.gps_status == 0) //leader gps status is not valid
        {
            return true;
        } 
    }
    else 
    {
        return true; // leader vehicle is dis connnection
    }
    //to check front vehicles from second to ego
    for (int i = leader_map_index + 1; i < ID_; i++)
    {
        int vehicle_id = platoon_id_map_[i];
        //forth step: check other vehicle connection
        bool if_disconnect = !(platoon_vehicles_info_[vehicle_id].isUpToDate());
        if (if_disconnect) //if is disconnect, the data in map[vehicle_id] is not valid, blow check is not need to do
        {
            if (i == ID_ - 1) // if front vehicle disconnet , abnormal 
            {
                if (m_debug_StateFlow_)
                {
                    std::cout << "Current Abnormal, front vehicles disconnected : " << vehicle_id << std::endl;
                }
                return true;
            }
            else // other vehicle dont't need to check disconnect 
            {
                continue;
            }
        }   
        const VehicleData& temp = platoon_vehicles_info_[vehicle_id].getData();
        bool if_abnormal = (DriveMode(temp.actual_drive_mode) == Abnormal) ? true : false;
        bool if_manual = (DriveMode(temp.actual_drive_mode) == Manual) ? true : false;
        if (if_abnormal || if_manual)
        {
            if (m_debug_StateFlow_)
            {  
                if (if_abnormal)
                {
                    std::cout << "Current Abnormal, front vehicles abnormal : " << vehicle_id << std::endl;
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
 * function: to clear fms order 
 * when vehicle from abnormal to auto /  to manual , this fuc is caled
 * 
 * */
void Manager::ResetFmsOrder()
{
    hmi_fms_info_.fms_order = int8_t(F_Invalid);
    hmi_fms_info_.safe_distance = m_safe_distance_;
    hmi_fms_info_.platoon_number = platoon_number_;
    LowFreDataContanier::GetInstance()->hmi_fms_info_.setData(hmi_fms_info_);
    SendDataContanier::GetInstance()->fms_order_.setData(F_Invalid);
}
/**
 * functin TransLicensToId and license_map is to help function IsAllJoinPlatoon
 * no use
*/
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

/**
 * function: to just if all vehicle join the platoon
 * true preset all vehicle join the platoon, the leader vehicle can changed to Leader;
 * false preset there is some vehicle not join in the platoon
*/

bool Manager::IsAllJoinPlatoon()
{
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        for (auto temp : platoon_id_map_)
        {
            int vehicle_id = temp.second;
            if (platoon_vehicles_info_[vehicle_id].isUpToDate())
            {
                if(DriveMode(platoon_vehicles_info_[vehicle_id].getData().actual_drive_mode != KeepQueue))
                {
                    if (m_debug_StateFlow_)
                    {
                        std::cout << "Refuse to be Leader " << vehicle_id << " don't keep" 
                                    << std::endl;
                    }
                    return false;
                }
            }
            else
            {
                if (m_debug_StateFlow_)
                {
                    std::cout << "Refuse to be Leader " << vehicle_id << " disconnect" 
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
            //int vehicle_id = TransLicensToId(fms_pre_info_.platoonmember(i));
            int vehicle_id = license_map_[fms_pre_info_.platoonmember(i)];
            if (vehicle_id == ConfigData::GetInstance()->vehicle_id_) //skip ego vehicle. 
                continue;
            if (platoon_vehicles_info_[vehicle_id].isUpToDate())//
            {
                if(DriveMode(platoon_vehicles_info_[vehicle_id].getData().actual_drive_mode != KeepQueue))
                {
                    if (m_debug_StateFlow_)
                    {
                        std::cout << "Refuse to be Leader " << vehicle_id << " don't keep" 
                                    << std::endl;
                    }
                    return false;
                }
            }
            else
            {
                if (m_debug_StateFlow_)
                {
                    std::cout << "Refuse to be Leader " << vehicle_id << " disconnect" 
                                << std::endl;
                }
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
    if (ID_ <= platoon_id_map_.size())
    {
        int after_id = platoon_id_map_[ID_ + 1];
        if (platoon_vehicles_info_[after_id].isUpToDate())
        {
            DriveMode after_mode = DriveMode(platoon_vehicles_info_[after_id].getData().actual_drive_mode);
            if (after_mode == Manual || after_mode == Auto 
                || after_mode == Leader || after_mode == LeaderWait
                || after_mode == Dequeue)
                return true;
            else 
            {
                if (ConfigData::GetInstance()->debug_StateFlow_)
                {
                    std::cout << "Refuse to be Leave, after vehicle " <<  int(after_id)  
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
    else if (ID_ > platoon_id_map_.size()) //present ego is the last vehicle
       return true;
}
/**
 * get front drive mode, leader vehicle mustn't call this function
*/
DriveMode Manager::FrontMode()
{
    //leader vehicle don't have front vehicle
    if (ID_ > 1)
    {
        int front_id = platoon_id_map_[ID_ - 1];
        if (platoon_vehicles_info_[front_id].isUpToDate())
        {
            DriveMode front_drive_mode = (DriveMode)platoon_vehicles_info_[front_id].getData().actual_drive_mode;
            return front_drive_mode;
        }
        else
        {
            return Notset;
        }  
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
    if (!plan_info_isupdate_)
    {
        if (m_debug_StateFlow_)
        {
            if (debug_count % m_debug_thw_HZ_ == 0)
                printf("ERROR : plan info is gone!\n\n");
        }
        return;
    }
    DriveMode old_drive_mode = desire_drive_mode_;

    float thw_dis = THWDis ();
    float front_dis = FrontDis ();
    float threshold_dis = CalThreshold();

    if (m_debug_StateFlow_)
    {
        if (debug_count % m_debug_thw_HZ_ == 0)
        {
            printf ("begin: thw dis is : %f, front dis is : %f\n safe distance is : %f, threshold dis is: %f\n", 
                    thw_dis , front_dis, m_safe_distance_, threshold_dis); 
            printf ("acutla drive mode is : ");
            print_drive_mode(actual_drive_mode_);
            printf("cut_in flag : %d\n", planning_info_.cut_in);
            printf("ID is : %d\n",ID_);
            printf("m_fms_order is : %d\n", m_fms_order_);
        }
    }
    // this is only for test, clear status
    if (m_fms_order_ == F_Reset)
    {
        desire_drive_mode_ = Notset;
        ResetFmsOrder();
        if (m_debug_StateFlow_)
        {
            if(debug_count % m_debug_thw_HZ_)
                printf("Clear Status\n");
        }
        return;
    }
    switch (actual_drive_mode_)
    {
        case Manual:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Manual\n");
            }
            //when vehicle is manual, desire must be manual,  not leader need reset commond
            if (ID_ > 1)
            {
                ResetFmsOrder();
            }     
            desire_drive_mode_ = Manual;
            break;
        case SubLeader:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN SubLeader\n");
            }
            if (m_fms_order_ == F_Dequeue)
            {
                if (IsAllowDequeue())
                {
                    desire_drive_mode_ = Auto;
                    // from Leader/SubLeader to Auto, present leaving, need reset
                    ResetFmsOrder();
                }
            }
            // when ego vehicle cut in is disapper, subplatoon should join the major platoon
            else if (planning_info_.cut_in == 0)
            {
                m_fms_order_ = F_Enqueue;
                SendDataContanier::GetInstance()->fms_order_.setData(F_Enqueue);
            }
        case Auto:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Auto\n");
            }
            //last desire mode is notset/manual/abnormal, when actual goto auto, desire should goto auto.
            if (desire_drive_mode_ == Notset || desire_drive_mode_ == Manual || desire_drive_mode_ == Abnormal)
            {
                desire_drive_mode_ = Auto;
            }
            if (m_fms_order_ == F_Leader)
            {
                desire_drive_mode_ = LeaderWait;
            }
            else if (m_fms_order_ == F_Enqueue)
            {
                if(ID_ <= 1)
                {
                    std::cerr << "ERROR, this vehicle should not receive formation order\n";
                    break;
                }
                if(IfAbnormal())
                    break;
                if (IsAllowEnqueue())
                {
                    if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
                    {
                        break;
                    }   
                    if (front_dis <= thw_dis * ConfigData::GetInstance()->formation_threshold_)
                    {
                        desire_drive_mode_ = Enqueue;
                    }
                }
            }
            break;
        case LeaderWait:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN LeaderWait\n");
            }
            if (m_fms_order_ == F_Dequeue)
            {
                if (IsAllowDequeue())
                {
                    desire_drive_mode_ = Auto;
                    // from Leader/SubLeader to Auto, present leaving, need reset
                    ResetFmsOrder();
                }    
            } 
            else if (IsAllJoinPlatoon())
            {
                desire_drive_mode_ = Leader;
            }
            break;
        case Leader:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Leader\n");
            }
            if (m_fms_order_ == F_Dequeue)
            {
                if (IsAllowDequeue())
                {
                    desire_drive_mode_ = Auto;
                    // from Leader/SubLeader to Auto, present leaving, need reset
                    ResetFmsOrder();
                }
            }
            else if (m_fms_order_ == F_DisBand)
            {
                desire_drive_mode_ = Auto;
                // from Leader/SubLeader to Auto, present leaving, need reset
                ResetFmsOrder();
            }
            break;
        case Enqueue: //add abnormal and dequeue
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Enqueue\n");
            }
            if (IfAbnormal())
            {
                desire_drive_mode_ = Abnormal;
            }  
            else if (m_fms_order_ == F_DisBand)
            {
                desire_drive_mode_ = Dequeue;
            }
            else if (m_fms_order_ == F_Dequeue)
            {
                if (IsAllowDequeue())
                {
                    desire_drive_mode_ = Dequeue;
                }
            }
            else
            {   
                if (ID_ <= 1)
                {
                    std::cerr << "ERROR, this vehicle should not go to keep \n";
                    break;
                }
                if (IsAllowKeep())
                {
                    if (fabs(threshold_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
                    {
                        break;
                    }
                    if (front_dis <= threshold_dis)
                    {
                        desire_drive_mode_ = KeepQueue;
                    }
                }
            }
            break;
        case Dequeue:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Dequeue\n");
            }
            // when actual mode goto leaving, present leaving, need reset
            ResetFmsOrder();
            if (IfAbnormal())
            {
                desire_drive_mode_ = Abnormal;
            }
            else
            {
                if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
                {
                    break;
                }  
                if (front_dis >= thw_dis * ConfigData::GetInstance()->leave_threshold_)
                {
                    desire_drive_mode_ = Auto;
                }      
            }
            break;
        case KeepQueue:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN KeepQueue\n");
            }
            if (IfAbnormal())
            {
                desire_drive_mode_ = Abnormal;
            }
            else
            {
                if (m_fms_order_ == F_Dequeue)
                {
                    if (IsAllowDequeue())
                    {
                        desire_drive_mode_ = Dequeue;
                    }
                }
                else if (m_fms_order_ == F_DisBand)
                {
                    desire_drive_mode_ = Dequeue;
                }
                else
                {// if front vehicle is enqueue, behind vehicle should to enqueue too, 
                 // this statiution happen when subleader goto formation
                    DriveMode front_drive_mode = FrontMode();
                    if (front_drive_mode == Enqueue)
                    {
                        desire_drive_mode_ = Enqueue;
                    }     
                }
            }
            break;
        case CutIN:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN CutIN\n");
            }
            // leader vehicle can't go to cut in
            if (ID_ == 1)
                break;
            desire_drive_mode_ = CutIN;
            if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
            {
                break;
            }  
            if (front_dis >= thw_dis * ConfigData::GetInstance()->to_auto_threshold_)
            {
                desire_drive_mode_ = SubLeader;
                /**
                 * from Cut_IN to Subleader present a task is over , need to clear fms enqueue oreder,
                 * invoid repeat enqueue.
                */
                ResetFmsOrder();
            }
            else 
            {   
                if(planning_info_.cut_in == 0)// cut in disappear
                {
                    if (m_fms_order_ == F_DisBand || m_fms_order_ == F_Dequeue) //if fms order is F_Dequeue , no back to enqueue;
                    {
                        desire_drive_mode_ = Dequeue;
                    }
                    else if (m_fms_order_ == F_Enqueue)
                    {   
                        if (fabs(threshold_dis - INVALID_FLOAT) <= Epslion)
                        {
                            break;
                        }
                        if (front_dis <= threshold_dis)
                        {
                            if (IsAllowKeep())
                            {
                                desire_drive_mode_ = KeepQueue;
                            }    
                            else
                            {
                                desire_drive_mode_ = Enqueue;
                            }
                        }
                        else
                        {
                            if (IsAllowEnqueue())
                            {
                                desire_drive_mode_ = Enqueue;
                            }   
                        }     
                    }
                }
            }
            break;
        case Abnormal: 
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Abnormal\n\n");
            }
            //leader vechile can't go abnormal 
            if (ID_ == 1 )
                break;
            desire_drive_mode_ = Abnormal;
            if (fabs(thw_dis - INVALID_FLOAT) <= Epslion || fabs(front_dis - INVALID_FLOAT) <= Epslion)
            {
                break;
            }  
            if (front_dis >= thw_dis * ConfigData::GetInstance()->to_auto_threshold_)
            {
                desire_drive_mode_ = Auto;
                /**
                 * from abnormal to auto present a task is over , need to clear fms enqueue oreder,
                 * invoid repeat enqueue.
                */
                ResetFmsOrder();
            }
            else 
            {   
                if(!IfAbnormal())
                {
                    if (m_debug_StateFlow_)
                    {
                        std::cout << "In Abnormal but current is normal" << std::endl;
                        std::cout << "fms order is " << int(m_fms_order_) << std::endl;
                    }
                    if (m_fms_order_ == F_DisBand || m_fms_order_ == F_Dequeue) //if fms order is F_Dequeue , no back to enqueue;
                    {
                        desire_drive_mode_ = Dequeue;
                    }
                    else if (m_fms_order_ == F_Enqueue)
                    {   
                        if (fabs(threshold_dis - INVALID_FLOAT) <= Epslion)
                        {
                            break;
                        }
                        if (front_dis <= threshold_dis)
                        {
                            if (IsAllowKeep())
                            {
                                desire_drive_mode_ = KeepQueue;
                            }    
                            else
                            {
                                desire_drive_mode_ = Enqueue;
                                if (m_debug_StateFlow_)
                                    std::cout << "refuse keep" << std::endl;
                            }
                        }
                        else
                        {
                            if (IsAllowEnqueue())
                            {
                                desire_drive_mode_ = Enqueue;
                            }   
                            else
                            {
                                if (m_debug_StateFlow_)
                                    std::cout << "refuse formation" << std::endl;
                            }
                        }     
                    }
                }
            }
            break;
        default:
            if (m_debug_StateFlow_)
            {
                if (debug_count % m_debug_thw_HZ_ == 0)
                    printf("IN Default\n");
            }
            //ignore command
            break;
    }
    if (desire_drive_mode_ != old_drive_mode)
    {
        printf ("desire_drive_mode changed : ");
        print_drive_mode(desire_drive_mode_);
    }
}


void Manager::UpdatePlatoonManagerInfo ()
{
    UpdateInfo();
    if (ConfigData::GetInstance()->test_with_log_)
    {
         CalculateID ();
    }
    else
    {
        if (IsCalID())
        {
            CalculateID ();
        }
        else if (m_debug_flags_ & DEBUG_CalculateID)
        {
            printf("id will be not chaned; vehicle sequence is : %d \n\n", ID_);
        }  
    }
    
    // the id only can change in the Auto/manual mode
    
    // assign vehicle ID to datacontanier
    SendDataContanier::GetInstance()->vehicle_ID_.setData(ID_);

    CalSafeDis();
    ProcessCommand ();
    m_worldmodle_.GetWorldmodleVehiles();

    if (ID_ == 0)
    {
        if (m_debug_flags_ & DEBUG_ManagerInfo)
        {
            printf("++++++No Manager Info++++++\n");
        }
        return;//no ego_vehicle_location
    }
    //update manager info / to_fusion info / 
    PlatoonManagerInfo platoon_manager_info;
    ToFusionData to_fusion_info;
    
    //update manager info
    platoon_manager_info.desire_drive_mode = int8_t(desire_drive_mode_);
    platoon_manager_info.platoon_number = platoon_number_;
    platoon_manager_info.vehicle_sequence = ID_;
    platoon_manager_info.vehicle_num = platoon_id_map_.size() + 1; //include ego vehicle
    platoon_manager_info.leader_frenet_dis = 1.0e10;
    platoon_manager_info.front_frenet_dis = 1.0e10;
    //safe distance
    platoon_manager_info.safe_distance = m_safe_distance_;
    //init leader/front vehicle id 
    platoon_manager_info.leader_vehicle.vehicle_id = -1;
    platoon_manager_info.front_vehicle.vehicle_id = -1;
    //init to_fusion_info 
    to_fusion_info.ego_drive_mode = int(desire_drive_mode_);
    to_fusion_info.vehicle_id = -1;
    
    if (m_debug_flags_ & DEBUG_ManagerInfo)
    {
        using namespace std;
        cout << "+++++++++++++Display Manager info+++++++++++++" << endl;
        printf (" desire_drive_mode is : ");
        print_drive_mode(DriveMode(platoon_manager_info.desire_drive_mode));
        printf("send data platoon number is : %d\n", platoon_number_);
        printf ("platoon number is : %d\n",platoon_manager_info.platoon_number);
        printf ("vehicle number is : %d\n",platoon_manager_info.vehicle_num);
        printf ("vehicle sequence is : %d\n", platoon_manager_info.vehicle_sequence);
        printf ("safe distance is : %f\n",platoon_manager_info.safe_distance);
    }
    /**
     * updata leader front vehicle info
    */
    if (ID_ > 1)
    {
        // only when ID >= 2, there has front / leader vehicle 
        //int leader_id = platoon_id_map_[1];
        int leader_map_index = 1;
        int leader_id = FindLeader(leader_map_index);
        int front_id = platoon_id_map_[ID_ - 1];
        // may be one paltoon can divided into more platoon
        platoon_manager_info.vehicle_sequence = ID_ - leader_map_index + 1;
        //this judge is for subplatoon
        if (desire_drive_mode_ == SubLeader || actual_drive_mode_ == CutIN)
            platoon_manager_info.vehicle_sequence = 1;

        bool leader_valid = platoon_vehicles_info_[leader_id].isUpToDate();
        bool front_valid = platoon_vehicles_info_[front_id].isUpToDate();

        if (leader_valid)
        {
            VehicleData leader_vehicle = platoon_vehicles_info_[leader_id].getData();
            if (ego_gps_isupdate_)
            {
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (leader_vehicle.relative_x, leader_vehicle.relative_y,
                                                                    ego_gps_info_.heading,
                                                                    ego_gps_info_.longitude, ego_gps_info_.latitude,
                                                                    ego_gps_info_.height,
                                                                    leader_vehicle.longitude, leader_vehicle.latitude,
                                                                    leader_vehicle.altitude);
                platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (leader_vehicle.relative_heading,
                                                                    ego_gps_info_.heading,
                                                                    leader_vehicle.heading);
            }
            platoon_manager_info.leader_vehicle = leader_vehicle;
            platoon_manager_info.leader_frenet_dis = m_worldmodle_.GetFrenetDis(leader_id);
            // judge if gps status is useful
            if (platoon_manager_info.leader_vehicle.gps_status == 0)
                platoon_manager_info.leader_vehicle.vehicle_id = -1;

            if (m_debug_flags_ & DEBUG_ManagerInfo)
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
                printf ("leader vehicle frent dis is(m) : %f\n",platoon_manager_info.leader_frenet_dis);
                printf ("leader vehicle relative_x is : %f\n",platoon_manager_info.leader_vehicle.relative_x);
                printf ("leader vehicle relative_y is : %f\n",platoon_manager_info.leader_vehicle.relative_y);
                printf ("leader vehicle drive mode is : ");
                print_drive_mode(DriveMode(platoon_manager_info.leader_vehicle.actual_drive_mode));
                printf("\n");
            }
        }
            
        if (front_valid)
        {
            VehicleData front_vehicle = platoon_vehicles_info_[front_id].getData();
            if (ego_gps_isupdate_)
            {
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (front_vehicle.relative_x, front_vehicle.relative_y,
                                                                    ego_gps_info_.heading,
                                                                    ego_gps_info_.longitude, ego_gps_info_.latitude,
                                                                    ego_gps_info_.height,
                                                                    front_vehicle.longitude, front_vehicle.latitude,
                                                                    front_vehicle.altitude);
                platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth (front_vehicle.relative_heading,
                                                                    ego_gps_info_.heading,
                                                                    front_vehicle.heading);
            }
            platoon_manager_info.front_vehicle = front_vehicle;
            platoon_manager_info.front_frenet_dis = m_worldmodle_.GetFrenetDis(front_id);
            // judge if gps status is useful
            if (platoon_manager_info.front_vehicle.gps_status == 0)
                platoon_manager_info.front_vehicle.vehicle_id = -1;

            to_fusion_info.vehicle_id = front_vehicle.vehicle_id;
            to_fusion_info.vehicle_length = front_vehicle.vehicle_length;
            to_fusion_info.vehicle_height = front_vehicle.vehicle_height;
            to_fusion_info.vehicle_width = front_vehicle.vehicle_width;
            to_fusion_info.desire_drive_mode = front_vehicle.desire_drive_mode;
            to_fusion_info.actual_drive_mode = front_vehicle.actual_drive_mode;
            to_fusion_info.cut_in_flag = front_vehicle.cut_in_flag;
            to_fusion_info.longitude = front_vehicle.longitude;
            to_fusion_info.latitude = front_vehicle.latitude;
            to_fusion_info.altitude = front_vehicle.altitude;
            to_fusion_info.heading = front_vehicle.heading;
            to_fusion_info.gps_status = front_vehicle.gps_status;
            to_fusion_info.gps_time = front_vehicle.gps_time;
            to_fusion_info.relative_x = front_vehicle.relative_x;
            to_fusion_info.relative_y = front_vehicle.relative_y;
            to_fusion_info.relative_heading = front_vehicle.relative_heading;
            to_fusion_info.longtitude_acc = front_vehicle.longtitude_acc;
            to_fusion_info.lateral_acc = front_vehicle.lateral_acc;
            to_fusion_info.speed = front_vehicle.speed;
            to_fusion_info.steering_wheel_angle = front_vehicle.steering_wheel_angle;
            to_fusion_info.yaw_rate = front_vehicle.yaw_rate;
            to_fusion_info.desire_long_acc = front_vehicle.desire_long_acc;
            // v3.0 new data
            to_fusion_info.platoon_number = front_vehicle.platoon_number;
            to_fusion_info.vehicle_sequence = front_vehicle.vehicle_sequence;


            if (m_debug_flags_ & DEBUG_ManagerInfo)
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
                printf ("front vehicle frent dis is(m) : %f\n",platoon_manager_info.front_frenet_dis);
                printf ("front vehicle relative_x is : %f\n",platoon_manager_info.front_vehicle.relative_x);
                printf ("front vehicle relative_y is : %f\n",platoon_manager_info.front_vehicle.relative_y);
                printf ("front vehicle drive mode is : ");
                print_drive_mode(DriveMode(platoon_manager_info.front_vehicle.actual_drive_mode));
                printf("front safe distance is : %f\n\n", platoon_manager_info.front_vehicle.safedistance);
                
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
        if (platoon_vehicles_info_[vehicle_id].isUpToDate())
        {
            vehicle_status_.push_back(vehicle_id);
            if (m_debug_flags_ & DEBUG_V2XCommunication)
            {
                struct timeval tv;
                gettimeofday (&tv, NULL);
                printf ("\nV %d: has data %ld.%ld\n\n", vehicle_id, tv.tv_sec, tv.tv_usec);
            }
        }
        else
        {
            vehicle_status_.push_back(0 - vehicle_id);
            if (m_debug_flags_ & DEBUG_V2XCommunication)
            {
                struct timeval tv;
                gettimeofday (&tv, NULL);
                printf ("V %d: lost>500 %ld.%ld\n\n", vehicle_id, tv.tv_sec, tv.tv_usec);
            }
        }
    }
    // add ego vehicel id in the vector end
    vehicle_status_.push_back(ConfigData::GetInstance()->vehicle_id_);
    
    platoon_manager_info.vehicle_communication_status = vehicle_status_;
    //  
    SendDataContanier::GetInstance()->manager_data_.setData (platoon_manager_info);
    SendDataContanier::GetInstance()->to_fusion_data_.setData(to_fusion_info);
}
/**
 * function: judge if need to recalid
 * true present need to cal id
 * false present don't need;
*/
bool Manager::IsCalID()
{
    if (actual_drive_mode_ == Manual)
        return true;
    if (actual_drive_mode_ == Auto)
    {
        if (platoon_id_map_.size() < platoon_vehicles_info_.size())//this present have new vehicle ,need to cal id
        {
            //std::cout << "Need, id map size is smaller than platon map" << std::endl;
            return true;
        }
        else
        {
            for (auto temp : platoon_id_map_)
            {
                int vehicle_id = temp.second;
                // is communication normal
                bool is_con_normal = platoon_vehicles_info_[vehicle_id].isUpToDate();
                if (!is_con_normal) //if platoon map does't has this vehicle
                {   // to find is in the v2x map, 
                    is_con_normal = v2x_vehicles_info_[vehicle_id].isUpToDate();
                    if (!is_con_normal)
                    {
                        //std::cout << " Don't Need cal id " << "vehicle: " << vehicle_id << "is disconnect" << std::endl;
                        return false;
                    }    
                    else
                    {
                        //std::cout << "the platoon number is changed" << std::endl;
                    }
                }
            }
            //std::cout << "Communication Normal, Need to cal id" << std::endl;
            return true;
        }
    }
    return false; 
}
/**
 * function: to find leader vehicle, from ego to front, the first cut-in/subleader is leader
 * if don't have cut-in status/subleader, the first vehicle is leader
*/
int Manager::FindLeader(int& map_index)
{
    if (ID_ <= 1 )
        return -1;
    for (int i = ID_ - 1; i > 1; --i)
    {
        int vehicle_id = platoon_id_map_[i];
        if (!platoon_info_isupdate_)
            return -1;
        if (platoon_vehicles_info_[vehicle_id].isUpToDate())
        {
            const DriveMode& temp = DriveMode(platoon_vehicles_info_[vehicle_id].getData().actual_drive_mode);
            if ( temp == SubLeader || temp == CutIN)
            {
                map_index = i;
                return vehicle_id;
            }  
        }
    }
    map_index = 1;
    return platoon_id_map_[1];
}

/**
 * function: to judge if ego can goto enqueue
 * true present allow
 * false present refuse
*/
bool Manager::IsAllowEnqueue()
{
    DriveMode front_drive_mode = FrontMode();
    switch (front_drive_mode)
    {
        case Leader:
        case LeaderWait:
        case SubLeader:
        case KeepQueue:
        case Enqueue:
            return true;
            break;
        default:
            return false;
            break;
    }
}
/**
 * function: to judge if ego can goto keepqueue
 * true present allow
 * false present refuse
*/
bool Manager::IsAllowKeep()
{
    DriveMode front_drive_mode = FrontMode();
    switch (front_drive_mode)
    {
        case Leader:
        case LeaderWait:
        case SubLeader:
        case KeepQueue:
            return true;
            break;
        default:
            return false;
            break;
    }
}
/**
 * cal safe distance 
 * when need to increase safedis, fms send the safedis to last vehicle
 * when need to decrease safedis, fms send the safedis to leader vehicle
*/
void Manager::CalSafeDis()
{
    //std::cout << "cal safe dis" << std::endl;
    if (ID_ == 1)
    {
        //std::cout << "2222" << std::endl;
        if (pre_info_isupdate_ && fabs(fms_safe_dis_ - fms_pre_info_.safe_distance()) > 0.1)
        {
            //std::cout << "fms dis is changed" << std::endl;
            fms_safe_dis_ = fms_pre_info_.safe_distance();
            m_safe_distance_ = fms_safe_dis_;
        }
        else if (hmi_isupdate_ && fabs(hmi_safe_dis_ - hmi_fms_info_.safe_distance) > 0.1)
        {// hmi safe dis is changed
            //std::cout << "hmi dis is changed" << std::endl;
            hmi_safe_dis_ = hmi_fms_info_.safe_distance;
            m_safe_distance_ = hmi_safe_dis_;
        }
    } 
    else
    {
        int leader_vehicle_id = platoon_id_map_[1];
        if (platoon_info_isupdate_)
        {
            if (platoon_vehicles_info_[leader_vehicle_id].isUpToDate())
            {
                m_safe_distance_ = platoon_vehicles_info_[leader_vehicle_id].getData().safedistance;
            }
            // printf("leader safe dis is : %f\n", leader_safe_dis);
            // printf("m_safe dis is : %f\n", m_safe_distance_);
        }
    }
}

} // namesapce communication
} // namespace platoon
