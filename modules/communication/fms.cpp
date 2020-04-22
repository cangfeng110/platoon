#include "modules/communication/fms.h"

#include <math.h>
#include <iostream>
#include <stdio.h>

//#include "modules/communication/datacontainer.h"
#include "modules/communication/configdata.h"
//#include "modules/communication/fmsdata.h"
#include "modules/communication/highfredatacontainer.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/senddatacontanier.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "modules/customfunction/functiontool.h"

namespace platoon
{
namespace communication
{

FMS::FMS() : m_fms_order_(F_Invalid)
{
    m_enqueue_point_.reserve(2);
    m_dequeue_point_ .reserve(2);
    m_hmi_fms_valid_ = ConfigData::GetInstance()->hmi_fms_valid_;
}

/**
 * update this class need date from other datacontanier
 * 
*/
void FMS::UpdateInfo()
{
    fms_pre_info_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(fms_pre_info_isupdate_);
    fms_back_info_ = LowFreDataContanier::GetInstance()->fms_apply_result_.getData(fms_back_info_isupdate_);
    planning_info_ = LowFreDataContanier::GetInstance()->planning_data_.getData(planning_info_isupdate_);
    hmi_fms_info_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(hmi_info_isupdate_);
    manager_info_ = SendDataContanier::GetInstance()->manager_data_.getData(manager_info_isupdate_);
    ego_vehicle_gps_data_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(ego_gps_isupdate_);
}

/**
 * update reference point when need to cal apply info or cal if disband
 * 
*/
void FMS::UpdatePoint()
{
    if (fms_pre_info_isupdate_)
    {
        m_enqueue_point_.clear();
        m_dequeue_point_.clear();
        const RefPoint& enqueue_point = fms_pre_info_.startnode();
        const RefPoint& dequeue_point = fms_pre_info_.endnode();
        m_enqueue_point_.push_back(enqueue_point.posx());
        m_enqueue_point_.push_back(enqueue_point.posy());
        m_dequeue_point_.push_back(dequeue_point.posx());
        m_dequeue_point_.push_back(dequeue_point.posy());
    }
}

ApplyResult FMS::CalApplyResult()
{
    if (fms_back_info_isupdate_)
    {
        if (fms_back_info_.applyresult())
        {
            if (fms_back_info_.applyinfo() == FmsApplyOrder(BeJoiner))
                return AllowJoin;
            else if (fms_back_info_.applyinfo() == FmsApplyOrder(BeLeader))
                return AllowLeader;
        }
        else
        {
            if (fms_back_info_.applyinfo() == FmsApplyOrder(BeJoiner))
                return RefuseJoiner;
            else if (fms_back_info_.applyinfo() == FmsApplyOrder(BeLeader))
                return RefuseLeader;
            else 
                return NoAnser;
        }
    }
    else
    {
        return NoAnser;
    }
}
/**
 * only in Auto mode need to call this function
*/
void FMS::CalApplyOrder()
{
    UpdatePoint();
    if (m_enqueue_point_.size() < 2) //no enqueue point
    {
            m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
    }  
    else 
    {
        if (!planning_info_isupdate_)
        {
            m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
            return;
        } 
        if (DriveMode(planning_info_.actual_drive_mode) == Auto)
        {   
            if (ego_gps_isupdate_)
            {   
                double relative_x, relative_y;
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (relative_x, relative_y,
                                                                ego_vehicle_gps_data_.heading,
                                                                ego_vehicle_gps_data_.longitude, ego_vehicle_gps_data_.latitude,
                                                                ego_vehicle_gps_data_.height,
                                                                m_enqueue_point_[0], m_enqueue_point_[1],
                                                                ego_vehicle_gps_data_.height);
                double dis = sqrt((relative_x * relative_x + relative_y * relative_y));
                if (dis < ConfigData::GetInstance()->enqueue_threshold_)
                {
                    ApplyResult result = CalApplyResult();
                    if (result == AllowJoin || result == AllowLeader || result == RefuseJoiner)
                        m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
                    else if (result == RefuseLeader)
                        m_to_fms_info_.set_applyinfo(FmsApplyOrder(BeJoiner));
                    else if (result == NoAnser)
                    {
                        if (manager_info_isupdate_)
                        {
                            if (manager_info_.vehicle_sequence == 1)
                            {
                                m_to_fms_info_.set_applyinfo(FmsApplyOrder(BeLeader));
                            }
                            else
                            {
                                m_to_fms_info_.set_applyinfo(FmsApplyOrder(BeJoiner));
                            }
                        }
                        else
                        {
                           m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
                        } 
                    }      
                }
                else
                {
                    m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply)); //
                }
            }
            else
            {
                m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply)); // ego gps is invalid
            }
            
        }
        else
        {
            m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
        }
    }
}

/**
 * true present need disband
 * false present don't need disband
*/
bool FMS::CalIfDisBand()
{   
    double ref_longtitude, ref_latitude, ref_altitude, ref_heading;

    if (!planning_info_isupdate_)
    {
        return false;
    }
    if (planning_info_.actual_drive_mode == Leader)
    {
        if (!ego_gps_isupdate_)
            return false;
        ref_longtitude = ego_vehicle_gps_data_.longitude;
        ref_latitude = ego_vehicle_gps_data_.latitude;
        ref_altitude = ego_vehicle_gps_data_.height;
        ref_heading = ego_vehicle_gps_data_.heading;
    }
    else 
    {
        if (!manager_info_isupdate_)
            return false;
        if (manager_info_.leader_vehicle.vehicle_id < 0)
            return false;
        ref_longtitude = manager_info_.leader_vehicle.longitude;
        ref_latitude = manager_info_.leader_vehicle.latitude;
        ref_altitude = manager_info_.leader_vehicle.altitude;
        ref_heading = manager_info_.leader_vehicle.heading;
    }

    UpdatePoint();

    if (m_dequeue_point_.size() < 2) // no dequeue point
    {
           return false; 
    }  
    double relative_x, relative_y;
    platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (relative_x, relative_y,
                                                        ref_heading,
                                                        ref_longtitude, ref_latitude,
                                                        ref_altitude,
                                                        m_dequeue_point_[0], m_dequeue_point_[1],
                                                        ref_altitude);
    double dis = sqrt((relative_x * relative_x + relative_y * relative_y));
    if (dis < ConfigData::GetInstance()->dequeue_threshold_)
    {
        return true;
    }
    else 
        return false;
}

/**
 * when the apply result has be responed, this function is  recalled
 * */
void FMS::ResetApplyResult()
{
    fms_back_info_.set_applyinfo(FmsApplyOrder(0));
    fms_back_info_.set_applyresult(false);
    LowFreDataContanier::GetInstance()->fms_apply_result_.setData(fms_back_info_);
}

void FMS::CalFmsOrder()
{
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        if (hmi_info_isupdate_)
        {
            m_fms_order_ = FmsOrder(hmi_fms_info_.fms_order);
        }
    }
    else
    {
        if (!planning_info_isupdate_)
            return;
        DriveMode ego_drive_mode = DriveMode(planning_info_.actual_drive_mode);
        if (ego_drive_mode == Auto)
        {
            ApplyResult result = CalApplyResult();
            if (result == AllowLeader)
            {
                m_fms_order_ = F_Leader;
                ResetApplyResult();
            }
            else if (result == AllowJoin)
            {
                m_fms_order_ = F_Enqueue;
                ResetApplyResult();
            }   
        }
        else if (ego_drive_mode == Enqueue || ego_drive_mode == KeepQueue
                 || ego_drive_mode == Abnormal || ego_drive_mode == Leader
                 || ego_drive_mode == LeaderWait)
        {
            if (CalIfDisBand())
                m_fms_order_ = F_DisBand;  
        } 
    } 
    SendDataContanier::GetInstance()->fms_order_.setData(m_fms_order_);
}

/**
 * update fms order 20 HZ
*/
void FMS::UpdateFmsOrder()
{
    UpdateInfo();
    CalFmsOrder();
    if (ConfigData::GetInstance()->debug_FmsPreInfo_)
    {
        PrintFmsPreInfo();
    }
    if (ConfigData::GetInstance()->debug_FmsBackInfo_)
    {
        PrintFmsBackInfo();
    }
    
}

/**
 * update to fms info 20 HZ
*/
void FMS::UpdateToFmsInfo()
{
    UpdateInfo();
    
    m_to_fms_info_.set_vehicleid(ConfigData::GetInstance()->vehicle_license_);

    CalApplyOrder();

    if (planning_info_isupdate_)
    {
        FmsDriveMode ego_drive_mode = FmsDriveMode(planning_info_.actual_drive_mode);
        m_to_fms_info_.set_actualdrivemode(ego_drive_mode);
    }
    
    if (manager_info_isupdate_)
    {
        int vehicle_squence = manager_info_.vehicle_sequence;
        int platoon_number = manager_info_.platoon_number;
        m_to_fms_info_.set_vehiclesquence(vehicle_squence);
        m_to_fms_info_.set_platoonnumber(platoon_number);
    }

    if (fms_pre_info_isupdate_)
    {
        std::string  order_id =fms_pre_info_.id();
        m_to_fms_info_.set_fmsmessageid(order_id); 
        if (ConfigData::GetInstance()->debug_ToFmsInfo_)
        {
            PrintToFmsInfo();
        }
    }
}

const ToFMSInfo& FMS::GetToFmsInfo() const
{
    return m_to_fms_info_;
}

//
void FMS::PrintApplyOrder(const ApplyOrder& order)
{ 
    printf("Apply Order is : ");
    switch (order)
    {
        case NoApply:
            printf("NoApply\n");
            break;
        case BeJoiner:
            printf("BeJonier\n");
            break;
        case BeLeader:
            printf("BeLeader\n");
            break;
        default:
            break;
    }
}
void FMS::PrintApplyBack(const ApplyResult& result)
{
    printf("Apply Back is : ");
    switch(result)
    {
        case NoAnser:
            printf("NoAnser\n");
            break;
        case RefuseJoiner:
            printf("RefuseJoiner\n");
            break;
        case RefuseLeader:
            printf("RefuseLeader\n");
            break;
        case AllowJoin:
            printf("AllowJoin\n");
            break;
        case AllowLeader:
            printf("AllowLeader\n");
            break;
        default:
            break;
    }
}

void print_drive_mode(const DriveMode& mode);

void FMS::PrintToFmsInfo()
{
    printf("To FMS Info\n");
    PrintApplyOrder(ApplyOrder(m_to_fms_info_.applyinfo()));
    printf("vehicle actual drive mode is : ");
    platoon::communication::print_drive_mode(DriveMode(m_to_fms_info_.actualdrivemode()));
    printf("vehicle platoon number is : %d\n", m_to_fms_info_.platoonnumber());
    printf("vehicle sequence is : %d\n", m_to_fms_info_.vehiclesquence());
    std::cout << "vehicle license is : " << m_to_fms_info_.vehicleid() << std::endl;
    std::cout << "serial number is : " << m_to_fms_info_.fmsmessageid() << std::endl;
}

void FMS::PrintFmsPreInfo()
{
    if (fms_pre_info_isupdate_)
    {
        using namespace std;
        cout << "FMS Pre Info" << endl;
        cout << "vehicle license is : " << fms_pre_info_.vehicleid() << endl;
        cout << "platoon number is : " << int(fms_pre_info_.platoonnumber()) << endl;
        cout << "platoon member size is : " << fms_pre_info_.platoonmember_size() << endl;
        printf("formation long is : %f\n", fms_pre_info_.startnode().posx());
        printf("formation lat is : %f\n", fms_pre_info_.startnode().posy());
        printf("leaving long is : %f\n", fms_pre_info_.endnode().posx());
        printf("leaving lat is : %f\n", fms_pre_info_.endnode().posy());
    }
    else 
    {
        std::cout << " FMS Pre Info is gone !" << std::endl;
    }
}

void FMS::PrintFmsBackInfo()
{
    using namespace std;
    if (fms_back_info_isupdate_)
    {
        cout << "FMS Back Info" << endl;
        cout << "vehicle license is : " << fms_back_info_.vehicleid() << endl;
        cout << "vehicle platoon number is : " << fms_back_info_.platoonnumber() << endl;
        PrintApplyBack(ApplyResult(CalApplyResult()));
    }
    else 
    {
        cout << "FMS Apply Back is gone !" << std::endl;
    }
}



}//namespace communication
}//namespace platoon

