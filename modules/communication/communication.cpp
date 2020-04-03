#include "modules/communication/communication.h"

#include <string>
#include <iostream>
#include <sys/time.h>

#include "modules/communication/datadefine.h"
#include "modules/customfunction/functiontool.h"
#include "modules/communication/fmsdata.h"
#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"

#include "include/protocol/ProtocolChannel.h"
#include "include/base/Channel.h"

INITIALIZE_EASYLOGGINGPP

namespace platoon 
{

namespace communication 
{

communication::communication(): lcm_("udpm://239.255.76.67:7667?ttl=1"),loop_("communication") 
{
    //Lcm
    if(!lcm_.good()) {
        LDIE << "lcm init error";
    }
    
    //receive ego gps/vcu information from lcm
    lcm_.subscribe("VCU_VEHICLE_INFO", &communication::HandleEgoVehicleVcuInfo, this);
    lcm_.subscribe("localization_out_2_map", &communication::HandleEgoVehicleGpsInfo, this);
    lcm_.subscribe("FMS_INFO", &communication::HandleHmiFmsInfo, this);
    lcm_.subscribe("EGO_PLANNINGMSG_FOR_PLATOON", &communication::HandlePlanningInfo, this);
    
    // lcm channel
    lcm_channel_.reset(new platoon::base::Channel(&loop_, lcm_.getFileno(), "lcm"));
    lcm_channel_->setReadCallback(std::bind(&lcm::LCM::handle, &lcm_));
    lcm_channel_->enableReading();

    // v2x channel
    v2x_channel_.reset(new platoon::base::Channel(&loop_, handler_.GetFileno(), "v2xComm"));
    if (ConfigData::GetInstance()->is_sil_test_)
    {
        v2x_channel_->setReadCallback(std::bind(&communication::SilReceiveV2xOtherVehicleInfo, this));
        v2x_channel_->enableReading();
    } 
    else
    {
        v2x_channel_->setReadCallback(std::bind(&communication::ReceiveV2xOtherVehicleInfo, this));
        v2x_channel_->enableReading();
    }
    
    // decrease ttl
    loop_.runEvery(100, std::bind(&DataContainer::DecreaseTtl, DataContainer::GetInstance()));

    // broad ego vehicle vcu info to ibox, 50Hz
    if (ConfigData::GetInstance()->is_sil_test_)
    {
        loop_.runEvery(1000 / (ConfigData::GetInstance()->broadcast_HZ_), std::bind(&communication::SilBroastEgoVehicleInfo, this));
    }
    else
    {
        loop_.runEvery(1000 / (ConfigData::GetInstance ()->broadcast_HZ_), std::bind(&communication::BroastEgoVehicleInfo, this));
    }
    
    //publish manager info to planning model
    loop_.runEvery(20, std::bind(&communication::PublishManagerInfo, this));
    
    //publish to fusion info
    loop_.runEvery(50, std::bind(&communication::PublishToFusionInfo, this));

    //receive fms info from lcm
    loop_.runEvery(1000, std::bind(&FmsHandler::ReceiveFmsPreInfo, fms_handler_));
    loop_.runEvery(1000, std::bind(&FmsHandler::ReceiveFmsApplyBack, fms_handler_));
    
    //publish to fms info
    to_fms_ptr_ = std::make_shared<ToFMSInfo>();
    loop_.runEvery(1000, std::bind(&communication::PublishToFmsInfo, this));

    // update to fms info and fms order
    loop_.runEvery(1000, std::bind(&FMS::UpdateFmsOrder, fms_));
    loop_.runEvery(1000, std::bind(&FMS::UpdateToFmsInfo, fms_));

    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_gps_HZ = ConfigData::GetInstance ()->GetDebugGpsHZ ();
    m_debug_vcu_HZ = ConfigData::GetInstance ()->GetDebugVcuHZ ();
    m_debug_pmi_HZ = ConfigData::GetInstance ()->GetDebugPmiHZ ();
    m_debug_plan_HZ = ConfigData::GetInstance()->debug_plan_HZ_;
    m_debug_hmi_HZ = ConfigData::GetInstance()->debug_hmi_HZ_;
}

//
//
//
communication::~communication() 
{
    LINFO << "loop end";
}

//
//
//
void communication::Loop() 
{
    LINFO << "loop start";
    loop_.loop();
    LINFO << "loop end";
}
//
//function:receive ego gps info and save to datacontainer
//
void communication::HandleEgoVehicleGpsInfo(const lcm::ReceiveBuffer *rbuf,
                                    const std::string &channel,
                                    const VehicleGpsData *msg)
{
    assert(channel == "localization_out_2_map");
    static int gps_count = 0;
    gps_count++;
    if (gps_count % m_debug_gps_HZ == 0)
    {
        printf ("asdf localization_out_2_map received %d\n\n", gps_count);
    }
    DataContainer::GetInstance()->ego_vehicle_gps_data_.setData(*msg);
    /**
     * this if is to judge if ege gps is changed, in sil gps will send old data if simulink model is stopped
     * to avoid calculated id error
     * */ 
    // const VehicleGpsData& old_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
    // if (fabs(old_data.longitude - msg->longitude) <= Epslion && fabs(old_data.latitude - msg->latitude) <= Epslion)
    // {
    //     printf("gps data is not changed, don't use this gps data\n");
    // }
    // else
    // {
    //     DataContainer::GetInstance()->ego_vehicle_gps_data_.setData(*msg);
    // } 
}

//
//function:receive ego vcu info and save to datacontainer
//
void communication::HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                     const std::string &channel,
                                     const VehicleVcuData *msg) 
{
    assert(channel == "VCU_VEHICLE_INFO");
    static int vcu_count = 0;
    vcu_count++;
    if (vcu_count % m_debug_vcu_HZ == 0)
    {
        printf ("asdf VCU_VEHICLE_INFO received %d\n\n", vcu_count);
    }
    DataContainer::GetInstance()->ego_vehicle_vcu_data_.setData(*msg);
}

void communication::HandleHmiFmsInfo(const lcm::ReceiveBuffer *rbuf,
                              const std::string &channel,
                              const HmiFmsInfo *msg)
{
    assert(channel == "FMS_INFO");
    static int hmi_count = 0;
    hmi_count++;
    if (hmi_count % m_debug_hmi_HZ == 0)
    {
        printf("asdf reveive HMI info : %d\n", hmi_count);
    }
    if (FmsData::GetInstance()->hmi_fms_info.isUpToDate())
    {
        if ( FmsData::GetInstance()->hmi_fms_info.getData().fms_order != msg->fms_order)
            std::cout << "asdf HMI FMS order changed : " << int(msg->fms_order) << std::endl;
        if (FmsData::GetInstance()->hmi_fms_info.getData().safe_distance != msg->safe_distance)
            std::cout << "asdf safe distance changed : " << msg->safe_distance << std::endl;
    }
    else
    {
        std::cout << "asdf HMI FMS order changed : " << int(msg->fms_order) << std::endl;
        std::cout << "asdf safe distance changed : " << msg->safe_distance << std::endl;
    }
    FmsData::GetInstance()->hmi_fms_info.setData(*msg);
}

void communication::HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                                        const std::string &channel,
                                        const EgoPlanningMsg *msg) 
{
    assert(channel == "EGO_PLANNINGMSG_FOR_PLATOON");
    static int plan_count = 0;
    plan_count++;
    if (plan_count % m_debug_plan_HZ == 0)
    {
        printf("asdf reveive plan info : %d\n\n", plan_count);
    }
    if (DataContainer::GetInstance ()->planning_data_.isUpToDate())
    {
        EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
        if (msg->actual_drive_mode != ego_planning_msg.actual_drive_mode)
        {
            printf ("asdf actual_drive_mode changed: %d\n", msg->actual_drive_mode);
        }
    }
    else 
    {
        printf ("asdf actual_drive_mode changed: %d\n", msg->actual_drive_mode);
    }
    DataContainer::GetInstance()->planning_data_.setData(*msg);
}
//
//function: broast ego vehicle gps info to ibox
//
void communication::BroastEgoVehicleInfo() 
{
    struct timeval tv;
    if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
        gettimeofday (&tv, NULL);
    if(DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate())
    {
        if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
            printf ("broadcast ego gps %ld.%ld\n", tv.tv_sec, tv.tv_usec);
        handler_.BroastEgoVehicleInfo();
    } 
    else 
    {
        if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
            printf ("ego gps gone %ld.%ld\n", tv.tv_sec, tv.tv_usec);
    }
}



void communication::PublishManagerInfo() 
{
    static int pmi_count = 0;
    manager_.UpdatePlatoonManagerInfo ();
    if (DataContainer::GetInstance ()->manager_data_.isUpToDate ()) 
    {
        const PlatoonManagerInfo& data = DataContainer::GetInstance ()->manager_data_.getData ();
        lcm_.publish ("PLATOON_MANAGER_INFO", &data);
        pmi_count++;
        if (pmi_count % m_debug_pmi_HZ == 0)
        {
            printf ("asdf PLATOON_MANAGER_INFO published %d\n\n", pmi_count);
        }
    }
}

//
//function: receive other vehicle info from ibox
//
void communication::ReceiveV2xOtherVehicleInfo() 
{
    if(handler_.DecodeV2xVechileInfo() > 0) 
    {
        if(DataContainer::GetInstance()->v2x_other_vehicles_data_.isUpToDate()) 
        {
            for (auto temp : DataContainer::GetInstance()->v2x_other_vehicles_data_.getData()) 
            {
                const VehicleData &data = temp.second.getData();
                int publish_v2x_flag = lcm_.publish("V2X_OTHER_VEHICLE_INFO", &data);
                //std::cout << "publish v2x flag is : " << publish_v2x_flag << std::endl;
            }   
        }
    }
}


void communication::PublishToFmsInfo()
{
    ToFMSInfo temp = fms_.GetToFmsInfo();
    sendMessageViaLcm<ToFMSInfo>("PLATOON_APPLY_INFO", temp);
}

void communication::SilBroastEgoVehicleInfo()
{
    struct timeval tv;
    if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
        gettimeofday (&tv, NULL);
    if(DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate())
    {
        if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
            printf ("broadcast ego gps %ld.%ld\n", tv.tv_sec, tv.tv_usec);
        handler_.SilBroastEgoVehicleInfo();
    } 
    else 
    {
        if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
            printf ("ego gps gone %ld.%ld\n", tv.tv_sec, tv.tv_usec);
    }

}

void communication::SilReceiveV2xOtherVehicleInfo()
{
    if(handler_.SilDecodeV2xVechileInfo() > 0) 
    {
        if(DataContainer::GetInstance()->v2x_other_vehicles_data_.isUpToDate()) 
        {
            for (auto temp : DataContainer::GetInstance()->v2x_other_vehicles_data_.getData()) 
            {
                const VehicleData &data = temp.second.getData();
                int publish_v2x_flag = lcm_.publish("V2X_OTHER_VEHICLE_INFO", &data);
                //std::cout << "publish v2x flag is : " << publish_v2x_flag << std::endl;
            }   
        }
    }
}

void communication::PublishToFusionInfo()
{
    static int count = 0;
    if (DataContainer::GetInstance ()->manager_data_.isUpToDate ()) 
    {
        const VehicleData& temp = DataContainer::GetInstance ()->manager_data_.getData ().front_vehicle;
        ToFusionData to_fusion;
        to_fusion.vehicle_id = temp.vehicle_id;
        to_fusion.vehicle_length = temp.vehicle_length;
        to_fusion.vehicle_height = temp.vehicle_height;
        to_fusion.vehicle_width = temp.vehicle_width;
        to_fusion.desire_drive_mode = temp.desire_drive_mode;
        to_fusion.actual_drive_mode = temp.actual_drive_mode;
        to_fusion.cut_in_flag = temp.cut_in_flag;
        to_fusion.longitude = temp.longitude;
        to_fusion.latitude = temp.latitude;
        to_fusion.altitude = temp.altitude;
        to_fusion.heading = temp.heading;
        to_fusion.gps_status = temp.gps_status;
        to_fusion.gps_time = temp.gps_time;
        to_fusion.relative_x = temp.relative_x;
        to_fusion.relative_y = temp.relative_y;
        to_fusion.relative_heading = temp.relative_heading;
        to_fusion.longtitude_acc = temp.longtitude_acc;
        to_fusion.lateral_acc = temp.lateral_acc;
        to_fusion.speed = temp.speed;
        to_fusion.steering_wheel_angle = temp.steering_wheel_angle;
        to_fusion.yaw_rate = temp.yaw_rate;
        to_fusion.desire_long_acc = temp.desire_long_acc;
        // v3.0 new data
        to_fusion.platoon_number = temp.platoon_number;
        to_fusion.vehicle_sequence = temp.vehicle_sequence;

        lcm_.publish ("TO_FUSION_INFO", &to_fusion); 

        count++;
        if (count % m_debug_pmi_HZ == 0)
        {
            printf ("asdf TOFUSIONINFO published %d\n\n", count);
        }       
    }
}

} // namesapce communication

} // namespace platoon
