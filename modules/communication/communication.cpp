#include "modules/communication/communication.h"

#include <string>
#include <iostream>

#include "include/protocol/ProtocolChannel.h"
#include "include/base/EventLoop.h"
#include "include/base/Channel.h"
#include "modules/common/functiontool.h"
#include <sys/time.h>

namespace platoon {

namespace communication {

communication::communication(): lcm_("udpm://239.255.76.67:7667?ttl=1"),loop_("communication") {
    // Logger
    // if(CommuConfig::GetInstance()->debug_) {
    //     base::Logging::setLogLevel(base::Logging::Debug);
    // }
    //Lcm
    if(!lcm_.good()) {
        LDIE << "lcm init error";
    }
    
    //receive ego gps/vcu information from lcm
    lcm_.subscribe("VCU_VEHICLE_INFO", &communication::HandleEgoVehicleVcuInfo, this);
    lcm_.subscribe("localization_out_2_map", &communication::HandleEgoVehicleGpsInfo, this);
    lcm_.subscribe("FMS_INFO", &communication::HandleFmsInfo, this);
    lcm_.subscribe("EGO_PLANNINGMSG_FOR_PLATOON", &communication::HandlePlanningInfo, this);
//    lcm_.subscribe("vehicle_info_for_test", &communication::HandleTestVehicleInfo, this);
    
    // lcm channel
    lcm_channel_.reset(new platoon::base::Channel(&loop_, lcm_.getFileno(), "lcm"));
    lcm_channel_->setReadCallback(std::bind(&lcm::LCM::handle, &lcm_));
    lcm_channel_->enableReading();

    // v2x channel
    v2x_channel_.reset(new platoon::base::Channel(&loop_, handler_.GetFileno(), "v2xComm"));
    v2x_channel_->setReadCallback(std::bind(&communication::ReceiveV2xOtherVehicleInfo, this));
    v2x_channel_->enableReading();

    // decrease ttl
    loop_.runEvery(100, std::bind(&DataContainer::DecreaseTtl, DataContainer::GetInstance()));

    // broad ego vehicle vcu info to ibox, 50Hz
    loop_.runEvery(1000 / (ConfigData::GetInstance ()->broadcast_HZ_), std::bind(&communication::BroastEgoVehicleInfo, this));

    //publish worldmodel vehilces info, 10Hz
    //loop_.runEvery(100, std::bind(&communication::PublishWorldmodelInfo, this));

    loop_.runEvery(20, std::bind(&communication::PublishManagerInfo, this));

    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_gps_HZ = ConfigData::GetInstance ()->GetDebugGpsHZ ();
    m_debug_vcu_HZ = ConfigData::GetInstance ()->GetDebugVcuHZ ();
    m_debug_pmi_HZ = ConfigData::GetInstance ()->GetDebugPmiHZ ();
}

//
//
//
communication::~communication() {
    LINFO << "loop end";
}

//
//
//
void communication::Loop() {
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
    //std::cout << "receive ego vehicle gps info." << std::endl;
    static int gps_count = 0;
    gps_count++;
    if (gps_count % m_debug_gps_HZ == 0)
    {
        printf ("asdf localization_out_2_map received %d\n\n", gps_count);
    }
    DataContainer::GetInstance()->ego_vehicle_gps_data_.setData(*msg);
}

//
//function:receive ego vcu info and save to datacontainer
//

void communication::HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                     const std::string &channel,
                                     const VehicleVcuData *msg) {
    assert(channel == "VCU_VEHICLE_INFO");
    //std::cout << "receive ego vcu info." << std::endl;
    static int vcu_count = 0;
    vcu_count++;
    if (vcu_count % m_debug_vcu_HZ == 0)
    {
        printf ("asdf VCU_VEHICLE_INFO received %d\n\n", vcu_count);
    }
    DataContainer::GetInstance()->ego_vehicle_vcu_data_.setData(*msg);
}

void communication::HandleFmsInfo(const lcm::ReceiveBuffer *rbuf,
                              const std::string &channel,
                              const FmsInfo *msg)
{
    assert(channel == "FMS_INFO");
    manager_.SetFmsInfo (*msg);
}

void communication::HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                                        const std::string &channel,
                                        const EgoPlanningMsg *msg) 
{
    assert(channel == "EGO_PLANNINGMSG_FOR_PLATOON");
    EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    if (msg->actual_drive_mode != ego_planning_msg.actual_drive_mode)
    {
        printf ("asdf actual_drive_mode changed: %d\n", msg->actual_drive_mode);
    }
    DataContainer::GetInstance()->planning_data_.setData(*msg);
}
//
//function: broast ego vehicle gps info to ibox
//
void communication::BroastEgoVehicleInfo() {
    struct timeval tv;
    if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
        gettimeofday (&tv, NULL);
    if(DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()){
        if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
            printf ("broadcast ego gps %ld.%ld\n", tv.tv_sec, tv.tv_usec);
        handler_.BroastEgoVehicleInfo();
    } else {
        if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
            printf ("ego gps gone %ld.%ld\n", tv.tv_sec, tv.tv_usec);
    }
}

void communication::PublishManagerInfo() {
    static int pmi_count = 0;
    manager_.UpdatePlatoonManagerInfo ();
    if (DataContainer::GetInstance ()->manager_data_.isUpToDate ()) {
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
void communication::ReceiveV2xOtherVehicleInfo() {
    if(handler_.DecodeV2xVechileInfo() > 0) {
        if(DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
            for (auto temp : DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()) {
                const VehicleData &data = temp.second.getData();
                //std::cout << "========= display ==========" << std::endl;                
                // std::cout << "iVehicleID       : " << data.iVehicleID    << std::endl
                //           << "fVehicleLength   : " << data.fVehicleLength << std::endl
                //           << "fVehicleWidth    : " << data.fVehicleWidth << std::endl
                //           << "iDriveModeStatus : " << (int)data.iDriveModeStatus << std::endl
                //           << "fBrakePedalAngle : " << data.fBrakePedalAngle    << std::endl
                //           << "dLongitude       : " << data.dLongitude          << std::endl
                //           << "dLatitude        : " << data.dLatitude           << std::endl
                //           << "ego_longitude    :  "<< DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().longitude <<std::endl
                //           << "ego_latitude     :  "<< DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().latitude << std::endl
                //           << "fLongituAcc      : " << data.fLongituAcc         << std::endl
                //           << "fLateralAcc      : " << data.fLateralAcc         << std::endl
                //           << "fHeading         : " << data.fHeading            << std::endl
                //           << "fSpeed           : " << data.fSpeed              << std::endl
                //           << "fSteeringAngle   : " << data.fSteeringAngle      << std::endl
                //           << "fYawRate         : " << data.fYawRate            << std::endl
                //           << "iGpsState        : " << (int)data.iGpsState      << std::endl
                //           << "iGpsTime         : " << data.iGpsTime            << std::endl
                //           << "iShiftPosition   : " << (int)data.iShiftPosition << std::endl;
/*
                 printf("long:%f\nlat:%f\nalt:%f\nlong:%f\nlat:%f\nalt%f\nheading%f\n",data.longitude,data.latitude,data.altitude,
                            DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().longitude,
                            DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().latitude,
                            DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().height,
                            DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().heading);
*/
                int publish_v2x_flag = lcm_.publish("V2X_OTHER_VEHICLE_INFO", &data);
                //std::cout << "publish v2x flag is : " << publish_v2x_flag << std::endl;
            }   
        }
    }
}

#define INVALID_FLOAT 1E10;
void communication::HandleTestVehicleInfo (const lcm::ReceiveBuffer *rbuf,
                                           const std::string &channel,
                                           const VehicleData *msg)
{
    VehicleData v2x_other_vehicle_data;
    v2x_other_vehicle_data.vehicle_id = msg->vehicle_id;
    v2x_other_vehicle_data.vehicle_length = msg->vehicle_length;
    v2x_other_vehicle_data.vehicle_width = msg->vehicle_width;
    v2x_other_vehicle_data.vehicle_height = msg->vehicle_height;
    v2x_other_vehicle_data.desire_drive_mode = msg->desire_drive_mode;
    v2x_other_vehicle_data.actual_drive_mode = msg->actual_drive_mode;
    v2x_other_vehicle_data.cut_in_flag = msg->cut_in_flag;
    v2x_other_vehicle_data.longitude = msg->longitude;
    v2x_other_vehicle_data.latitude = msg->latitude;
    v2x_other_vehicle_data.altitude = msg->altitude;
    v2x_other_vehicle_data.heading = msg->heading;
    v2x_other_vehicle_data.gps_status = msg->gps_status;
    v2x_other_vehicle_data.gps_time = msg->gps_time;
    v2x_other_vehicle_data.relative_x = msg->relative_x;
    v2x_other_vehicle_data.relative_y = msg->relative_y;
    v2x_other_vehicle_data.relative_heading = msg->relative_heading;
    v2x_other_vehicle_data.longtitude_acc = msg->longtitude_acc;
    v2x_other_vehicle_data.lateral_acc = msg->lateral_acc;
    v2x_other_vehicle_data.speed = msg->speed;
    v2x_other_vehicle_data.steering_wheel_angle = msg->steering_wheel_angle;
    v2x_other_vehicle_data.yaw_rate = msg->yaw_rate;
    v2x_other_vehicle_data.desire_long_acc = msg->desire_long_acc;
    
    int key = v2x_other_vehicle_data.vehicle_id;

    if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
        const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                ego_vehicle_gps_data.heading,
                ego_vehicle_gps_data.longitude,ego_vehicle_gps_data.latitude,
                ego_vehicle_gps_data.height,
                v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                v2x_other_vehicle_data.altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                ego_vehicle_gps_data.heading, v2x_other_vehicle_data.heading);
    } else {
        v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
        v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
    }
    DataContainer::GetInstance()->v2x_other_vehicle_data_.setData(key, v2x_other_vehicle_data);
}
//
//function:publish worldmodel info to channel
//
/* void communication::PublishWorldmodelInfo() {
    if (DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
        //LINFO << "publish worldmodle vehicle info to lcm";
        WorldModelObjects temp = handler_.GetWorldmodleVehiles();
        std::cout << "###########Publish publish worldmodle vehicle info to lcm#######" << std::endl
                  <<"remote vehilce number is : " << temp.vehicle_num << std::endl;
        if(temp.vehicle_num > 0){
            for(auto it : temp.vehicles){
                std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
                std::cout << "vehicle ID : " << it.vehicle_id << std::endl 
                          <<"vehicle frenet dis is : " << it.frenet_lon_distance << std::endl
                          << "history trajectory point is : " << it.point_num << std::endl;
                // for(int i = 0; i < it.hisTrajectory.size(); i++) {
                //     std::cout << "the "  << i << " trajectory relative x is :" << it.hisTrajectory[i].relative_x << std::endl;
                //     std::cout << "the "  << i << " trajectory relative y is :" << it.hisTrajectory[i].relative_y << std::endl;
                //     std::cout << "the "  << i << " trajectory relative heading is :" << it.hisTrajectory[i].relative_heading << std::endl;
                // }
                std::cout << "########################################################" << std::endl;
            } 
        }        
        lcm_.publish("WORLDMODEL_OTHER_OBJECTS_INFO", &handler_.GetWorldmodleVehiles());
    } else {
//        LDEBUG << " worldmodel info is not update ";
    }
        
} */

} // namesapce communication

} // namespace platoon
