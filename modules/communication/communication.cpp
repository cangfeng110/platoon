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
    if (ConfigData::GetInstance()->test_with_log_)
    {
        lcm_.subscribe("V2X_OTHER_VEHICLE_INFO", &communication::HandleLogV2xInfo, this);
    }
    if (ConfigData::GetInstance()->print_log_)
    {
        lcm_.subscribe("PLATOON_MANAGER_INFO", &communication::HandleLogManagerInfo, this);
    }
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
    if (!ConfigData::GetInstance()->print_log_)
    {
        loop_.runEvery(20, std::bind(&communication::PublishManagerInfo, this));
    }
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
        if (FmsData::GetInstance()->hmi_fms_info.getData().platoon_number != msg->platoon_number)
        {
            std::cout << "asdf platoon number changed : " << msg->platoon_number << std::endl;
            DataContainer::GetInstance()->platoon_vehicles_data_.getData().clear();
        }
            
    }
    else
    {
        std::cout << "asdf HMI FMS order changed : " << int(msg->fms_order) << std::endl;
        std::cout << "asdf safe distance changed : " << msg->safe_distance << std::endl;
        std::cout << "asdf platoon number changed : " << msg->platoon_number << std::endl;
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
        if (ConfigData::GetInstance()->is_protocol_2_)
        {
            handler_.BroastEgoVehicleInfo(1);
        }
        else
        {
            handler_.BroastEgoVehicleInfo();
        } 
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
    int result = -1;
    if (ConfigData::GetInstance()->is_protocol_2_)
    {
        result = handler_.DecodeV2xVehicleInfo(1);
    }
    else
    {
        result = handler_.DecodeV2xVechileInfo();
    }
    if((result > 0) && DataContainer::GetInstance()->v2x_other_vehicles_data_.isUpToDate()) 
    {
        for (auto temp : DataContainer::GetInstance()->v2x_other_vehicles_data_.getData()) 
        {
            const VehicleData &data = temp.second.getData();
            int publish_v2x_flag = lcm_.publish("V2X_OTHER_VEHICLE_INFO", &data);
            //std::cout << "publish v2x flag is : " << publish_v2x_flag << std::endl;
        }   
    } 
}


void communication::PublishToFmsInfo()
{
    //only receive fms pre info, this fuction need to send info to fms
    if (FmsData::GetInstance()->fms_pre_info_.isUpToDate())
    {
        ToFMSInfo temp = fms_.GetToFmsInfo();
        sendMessageViaLcm<ToFMSInfo>("PLATOON_APPLY_INFO", temp);
    }
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
/*
 * function : test with log, handler V2X_OTHER_VEHICLE_INFO, simulate UDP communication 
 * like handler Decode function
*/
void communication::HandleLogV2xInfo(const lcm::ReceiveBuffer* rbuf, 
                          const std::string& channel,
                          const VehicleData* msg)
{
    assert(channel == "V2X_OTHER_VEHICLE_INFO");

    VehicleData v2x_other_vehicle_data = *msg; 
    int key = v2x_other_vehicle_data.vehicle_id;

    if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) 
    {
        const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                            ego_vehicle_gps_data.heading,
                                                            ego_vehicle_gps_data.longitude,ego_vehicle_gps_data.latitude,
                                                            ego_vehicle_gps_data.height,
                                                            v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                            v2x_other_vehicle_data.altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                ego_vehicle_gps_data.heading, v2x_other_vehicle_data.heading);
    } 
    else 
    {
        v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
        v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
    }
    DataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

    std::string if_platoon = "No";
    
    /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        if_platoon = "Yes";
        DataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
    }
    else if (FmsData::GetInstance()->fms_pre_info_.isUpToDate()) 
    {
        int ego_platoon_number = FmsData::GetInstance()->fms_pre_info_.getData().platoonnumber();
        if (ego_platoon_number == v2x_other_vehicle_data.platoon_number) 
        {
            if_platoon = "Yes";
            DataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }
    }

    if (m_debug_flags & DEBUG_V2xVehicleInfo) 
    {
        using namespace std;
        cout << "-----------Display other vehicle info--------------" << endl;
        cout << "other vehicle id is : " << key << endl;
        cout << "if a platoon vehicle : " << if_platoon << endl;
        printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
        printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
        printf("other vehicle gps time is : %f\n", v2x_other_vehicle_data.gps_time);
        printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
        printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
        printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
        printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
        printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
        printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
        printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
        printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
    }
}

/**
 * function : print manangerinfo to check log
*/

void print_drive_mode(const DriveMode& mode);

void communication::HandleLogManagerInfo(const lcm::ReceiveBuffer* rbuf, 
                              const std::string& channel,
                              const PlatoonManagerInfo* msg)
{
    assert(channel == "PLATOON_MANAGER_INFO");
    PlatoonManagerInfo platoon_manager_info = *msg;
    if (m_debug_flags & DEBUG_ManagerInfo)
    {
        using namespace std;
        cout << "+++++++++++++Display Manager ifno+++++++++++++" << endl;
        printf (" desire_drive_mode is : ");
        print_drive_mode(DriveMode(platoon_manager_info.desire_drive_mode));
        printf ("platoon number is : %d\n",platoon_manager_info.platoon_number);
        printf ("vehicle sequence is : %d\n", platoon_manager_info.vehicle_sequence);
        printf ("hmi safe distance is : %f\n",platoon_manager_info.safe_distance);
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

} // namesapce communication

} // namespace platoon
