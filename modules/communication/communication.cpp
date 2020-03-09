#include "modules/communication/communication.h"

#include <string>
#include <iostream>

#include "protocol/ProtocolChannel.h"
#include "include/base/EventLoop.h"
#include "include/base/Channel.h"
#include "modules/common/functiontool.h"
#include <sys/time.h>

namespace platoon {

namespace communication {

communication::communication(): lcm_("udpm://239.255.76.67:7667?ttl=1"),loop_("communication") {
  
    //Lcm
    if(!lcm_.good()) {
        LDIE << "lcm init error";
    }
    
    //receive ego gps/vcu information from lcm
    lcm_.subscribe("VCU_VEHICLE_INFO", &communication::HandleEgoVehicleVcuInfo, this);
    lcm_.subscribe("localization_out_2_map", &communication::HandleEgoVehicleGpsInfo, this);
    lcm_.subscribe("FMS_INFO", &communication::HandleFmsInfo, this);
    lcm_.subscribe("EGO_PLANNINGMSG_FOR_PLATOON", &communication::HandlePlanningInfo, this);

    lcm_.subscribe("V2X_OTHER_VEHICLE_INFO", &communication::HandleV2xVehicelInfo, this);
    lcm_.subscribe("PLATOON_MANAGER_INFO", &communication::HandleManagerInfo, this);

    // lcm channel
    lcm_channel_.reset(new platoon::base::Channel(&loop_, lcm_.getFileno(), "lcm"));
    lcm_channel_->setReadCallback(std::bind(&lcm::LCM::handle, &lcm_));
    lcm_channel_->enableReading();

    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
    m_debug_gps_HZ = ConfigData::GetInstance ()->GetDebugGpsHZ ();
    m_debug_vcu_HZ = ConfigData::GetInstance ()->GetDebugVcuHZ ();
    m_debug_pmi_HZ = ConfigData::GetInstance ()->GetDebugPmiHZ ();
    m_debug_thw_HZ = ConfigData::GetInstance()->GetDebugThwHZ();
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
    static int gps_count = 0;
    gps_count++;
    if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo) {
        if (gps_count % m_debug_gps_HZ == 0){
        using namespace std;
        cout << "*********Display ego vehicle gps info*********" << endl;
        cout << "ego vehicle long is : " << msg->longitude << endl;
        cout << "ego vehicle lat is : " << msg->latitude << endl;
        cout << "ego vehicle alt is : " << msg->height << endl << endl;
    }
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
    if (m_debug_flags & DEBUG_BroadcastEgoVehicleInfo) {
        if (vcu_count % m_debug_vcu_HZ == 0){
            using namespace std;
            cout << "////////Display ego vehicle vcu info////////" << endl;
            cout << "ego vehicle speed is(km/h) : " << msg->fSpeed << endl << endl;
    }
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

    //EgoPlanningMsg ego_planning_msg = DataContainer::GetInstance ()->planning_data_.getData ();
    static int plan_count = 0;
    plan_count++;
    if(m_debug_flags & DEBUG_CalculateID){
        if (plan_count % m_debug_thw_HZ == 0) {
        using namespace std;
        cout << "########Display planning info#######" << endl;
        cout << "ego vehicle actual mode is : " << int(msg->actual_drive_mode) << endl << endl;
    }
    }
    
    
    DataContainer::GetInstance()->planning_data_.setData(*msg);
}

/*-----------------------*/

void communication:: HandleV2xVehicelInfo(const lcm::ReceiveBuffer* rbuf,
                                const std::string& channel,
                                const VehicleData* msg)
{
    static int v2x_count = 0;
    v2x_count++;
    if (m_debug_flags & DEBUG_V2xVehicleInfo) {
        if (v2x_count % m_debug_vcu_HZ == 0){
        using namespace std;
        cout << "=========Display other vehicle info=========" << endl;
        cout << "other vehicle id is : " << msg->vehicle_id << endl;
        cout << "other vehicle longitude is : " << msg->longitude << endl;
        cout << "other vehicle latitude is : " << msg->latitude << endl;
        cout << "other vehicle speed is(km/h): " << msg->speed * 3.6 << endl;
        cout << "other vehicle relative_x is : " << msg->relative_x << endl << endl;
    }
    }
}

/*-----------------------*/

void communication::HandleManagerInfo(const lcm::ReceiveBuffer* rbuf,
                        const std::string& channel,
                        const PlatoonManagerInfo* msg)
{
    static int pmi_count = 0;
    pmi_count++;
    if (m_debug_flags & DEBUG_TimeToFront) {
        if (pmi_count % m_debug_pmi_HZ == 0) {
        using namespace std;
        cout << "------------Display manager info------------" << endl;
        cout << "desire drive mode is : " << int(msg->desire_drive_mode) << endl;
        cout << "front vehicel number is : " << int(msg->vehicle_num) << endl;
        cout << "leader vehicel id is : " << msg->leader_vehicle.vehicle_id  << endl;
        cout << "leader vehicel long is : " << msg->leader_vehicle.longitude << endl;
        cout << "leader vehicle lat is : " << msg->leader_vehicle.latitude << endl;
        cout << "leader vheicle speed is :" << msg->leader_vehicle.speed * 3.6 << endl;
        cout << "leader vehicle relative x is : " << msg->leader_vehicle.relative_x << endl;
        cout << "leader vehicle frenet dis is : " << msg->leader_frenet_dis << endl;
    
        cout << "front vehicel id is : " << msg->front_vehicle.vehicle_id  << endl;
        cout << "front vehicel long is : " << msg->front_vehicle.longitude << endl;
        cout << "front vehicle lat is : " << msg->front_vehicle.latitude << endl;
        cout << "front vheicle speed is :" << msg->front_vehicle.speed * 3.6 << endl;
        cout << "front vehicle relative x is : " << msg->front_vehicle.relative_x << endl;
        cout << "front vehicle frenet dis is : " << msg->front_frenet_dis << endl<< endl;
    }
    }
}



} // namesapce communication

} // namespace platoon

