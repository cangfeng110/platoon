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
        }
    }
    else
    {
        return NoAnser;
    }
}

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
                if (abs(relative_x) < 200)
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

bool FMS::CalIfDisBand()
{   
    double ref_longtitude, ref_latitude, ref_altitude;

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
    }

    const VehicleData& leader_info = DataContainer::GetInstance()->manager_data_.getData().leader_vehicle; 
    const VehicleGpsData& ego_vehicle_location = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
    UpdatePoint();

    if (m_dequeue_point_.size() < 2) // no dequeue point
    {
           return false; 
    }  
    double relative_x, relative_y;
    platoon::common::TransfromGpsAbsoluteToEgoRelaCoord (relative_x, relative_y,
                                                    leader_info.heading,
                                                    leader_info.longitude, leader_info.latitude,
                                                    leader_info.altitude,
                                                    m_dequeue_point_[0], m_dequeue_point_[1],
                                                    leader_info.altitude);
    if (abs(relative_x) < 200)
    {
        return true;
    }
    else 
        return false;
}


void FMS::CalFmsOrder()
{
    
}


}//namespace communication
}//namespace platoon

