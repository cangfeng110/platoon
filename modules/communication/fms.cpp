#include "modules/communication/fms.h"

#include "modules/communication/datacontainer.h"
#include "modules/communication/configdata.h"
#include "modules/communication/fmsdata.h"
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
 * update reference point when need to cal apply info or cal if disband
 * 
*/
void FMS::UpdatePoint()
{
    if (FmsData::GetInstance()->fms_pre_info_.isUpToDate())
    {
        m_enqueue_point_.clear();
        m_dequeue_point_.clear();
        const RefPoint& enqueue_point = FmsData::GetInstance()->fms_pre_info_.getData().startnode();
        const RefPoint& dequeue_point = FmsData::GetInstance()->fms_pre_info_.getData().endnode();
        m_enqueue_point_.push_back(enqueue_point.posx());
        m_enqueue_point_.push_back(enqueue_point.posy());
        m_dequeue_point_.push_back(dequeue_point.posx());
        m_dequeue_point_.push_back(dequeue_point.posy());
    }
}

ApplyResult FMS::CalApplyResult()
{
    if (FmsData::GetInstance()->fms_apply_result_.isUpToDate())
    {
        const FMSApplyResultInfo& temp = FmsData::GetInstance()->fms_apply_result_.getData();
        if (temp.applyresult())
        {
            if (temp.applyinfo() == FmsApplyOrder(BeJoiner))
                return AllowJoin;
            else if (temp.applyinfo() == FmsApplyOrder(BeLeader))
                return AllowLeader;
        }
        else
        {
            if (temp.applyinfo() == FmsApplyOrder(BeJoiner))
                return NoJoiner;
            else if (temp.applyinfo() == FmsApplyOrder(BeLeader))
                return NoLeader;
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
void FMS::CalApplyInfo()
{
    UpdatePoint();
    if (m_enqueue_point_.size() < 2) //no enqueue point
    {
            m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
    }  
    else 
    {
        if (!DataContainer::GetInstance()->planning_data_.isUpToDate())
        {
            m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
            return;
        } 
        const EgoPlanningMsg& plan_info = DataContainer::GetInstance()->planning_data_.getData();
        if (DriveMode(plan_info.actual_drive_mode) == Auto)
        {   
            if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate())
            {   
                double relative_x, relative_y;
                const VehicleGpsData& ego_vehicle_location = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (relative_x, relative_y,
                                                                ego_vehicle_location.heading,
                                                                ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                ego_vehicle_location.height,
                                                                m_enqueue_point_[0], m_enqueue_point_[1],
                                                                ego_vehicle_location.height);
                if (abs(relative_x) < ConfigData::GetInstance()->enqueue_threshold_)
                {
                    ApplyResult result = CalApplyResult();
                    if (result == AllowJoin || result == AllowLeader || result == NoJoiner)
                        m_to_fms_info_.set_applyinfo(FmsApplyOrder(NoApply));
                    else if (result == NoLeader)
                        m_to_fms_info_.set_applyinfo(FmsApplyOrder(BeJoiner));
                    else if (result == NoAnser)
                        m_to_fms_info_.set_applyinfo(FmsApplyOrder(BeLeader));
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

    if (!DataContainer::GetInstance()->planning_data_.isUpToDate())
    {
        return false;
    }
    const EgoPlanningMsg& plan_info = DataContainer::GetInstance()->planning_data_.getData();
    if (plan_info.actual_drive_mode == Leader)
    {
        if (!DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate())
            return false;
        ref_longtitude = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().longitude;
        ref_latitude = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().latitude;
        ref_altitude = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().height;
        ref_heading = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().heading;
    }
    else 
    {
        if (!DataContainer::GetInstance()->manager_data_.isUpToDate())
            return false;
        const PlatoonManagerInfo& manager_info = DataContainer::GetInstance()->manager_data_.getData();
        if (manager_info.leader_vehicle.vehicle_id < 0)
            return false;
        ref_longtitude = manager_info.leader_vehicle.longitude;
        ref_latitude = manager_info.leader_vehicle.latitude;
        ref_altitude = manager_info.leader_vehicle.altitude;
        ref_heading = manager_info.leader_vehicle.heading;
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
    if (abs(relative_x) < ConfigData::GetInstance()->dequeue_threshold_)
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
    FMSApplyResultInfo result;
    result = FmsData::GetInstance()->fms_apply_result_.getData();
    result.set_applyinfo(FmsApplyOrder(0));
    result.set_applyresult(false);
    FmsData::GetInstance()->fms_apply_result_.setData(result);
}

void FMS::CalFmsOrder()
{
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        m_fms_order_ = FmsOrder(FmsData::GetInstance()->hmi_fms_info.getData().fms_order);
    }
    else
    {
        if (!DataContainer::GetInstance()->planning_data_.isUpToDate())
            return;
        DriveMode ego_drive_mode = DriveMode(DataContainer::GetInstance()->planning_data_.getData().actual_drive_mode);
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
                 || ego_drive_mode == Abnormal || ego_drive_mode == Leader)
        {
            if (CalIfDisBand())
                m_fms_order_ = F_DisBand;  
        } 
    } 
    FmsData::GetInstance()->fms_order_.setData(m_fms_order_);
}

/**
 * update fms order 20 HZ
*/
void FMS::UpdateFmsOrder()
{
    CalFmsOrder();
}

/**
 * update to fms info 20 HZ
*/
void FMS::UpdateToFmsInfo()
{
    
    m_to_fms_info_.set_vehicleid(ConfigData::GetInstance()->vehicle_license_);

    CalApplyInfo();

    if (DataContainer::GetInstance()->planning_data_.isUpToDate())
    {
        FmsDriveMode ego_drive_mode = FmsDriveMode(DataContainer::GetInstance()->planning_data_.getData().actual_drive_mode);
        m_to_fms_info_.set_actualdrivemode(ego_drive_mode);
    }

    if (DataContainer::GetInstance()->manager_data_.isUpToDate())
    {
        int vehicle_squence = DataContainer::GetInstance()->manager_data_.getData().vehicle_sequence;
        int platoon_number = DataContainer::GetInstance()->manager_data_.getData().platoon_number;
        m_to_fms_info_.set_vehiclesquence(vehicle_squence);
        m_to_fms_info_.set_platoonnumber(platoon_number);
    }

    if (FmsData::GetInstance()->fms_apply_result_.isUpToDate())
    {
        std::string  order_id = FmsData::GetInstance()->fms_apply_result_.getData().id();
        m_to_fms_info_.set_fmsmessageid(order_id); 
    }
}

const ToFMSInfo& FMS::GetToFmsInfo() const
{
    return m_to_fms_info_;
}


}//namespace communication
}//namespace platoon

