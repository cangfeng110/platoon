#include "modules/communication/communication.h"

#include <string>
#include <iostream>

#include "protocol/ProtocolChannel.h"
#include "include/base/EventLoop.h"
#include "include/base/Channel.h"



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
    lcm_.subscribe("INERTIAL_DEBUG", &communication::HandleEgoVehicleGpsInfo, this);

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
    loop_.runEvery(20, std::bind(&communication::BroastEgoVehicleVcuInfo, this));
    
    //broad ego vehicle gps info to ibox, 100Hz
    //loop_.runEvery(10, std::bind(&communication::BroastEgoVehicleGpsInfo, this));

    //publish worldmodel vehilces info, 50Hz
    loop_.runEvery(20, std::bind(&communication::PublishWorldmodelInfo, this));
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
    assert(channel == "INERTIAL_DEBUG");
    std::cout << "receive ego vehicle gps info.";
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
    DataContainer::GetInstance()->ego_vehicle_vcu_data_.setData(*msg);   
}
//
//function: broast ego vehicle gps info to ibox
//
void communication::BroastEgoVehicleGpsInfo() {
    if(DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()){
        //std::cout << " broast ego vehicle gps info to ibox";
        handler_.BroastEgoVehicleGpsInfo();
    } else {
        LDEBUG <<"ego vehicle gps out of date";
    }
}
//
//function: broast ego vehicle vcu info to ibox
//
void communication::BroastEgoVehicleVcuInfo() {
    if(DataContainer::GetInstance()->ego_vehicle_vcu_data_.isUpToDate()){
        //std::cout << " broast ego vehicle vcu info to ibox";
        handler_.BroastEgoVehicleVcuInfo();
    } else {
        LDEBUG <<"ego vehicle vcu out of date";
    }
}
//
//function: receive other vehicle info from ibox
//
void communication::ReceiveV2xOtherVehicleInfo() {
    if(handler_.DecodeV2xVechileInfo() > 0) {
        if(DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
            for (auto temp : DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()) {
                //std::cout << "========= display ==========" << std::endl;
                const VehicleData &data = temp.second.getData();
                // std::cout << "iVehicleID       : " << data.iVehicleID    << std::endl
                //           << "fVehicleLength   : " << data.fVehicleLength << std::endl
                //           << "fVehicleWidth    : " << data.fVehicleWidth << std::endl
                //           << "iDriveModeStatus : " << (int)data.iDriveModeStatus << std::endl
                //           << "fBrakePedalAngle : " << data.fBrakePedalAngle    << std::endl
                //           << "dLongitude       : " << data.dLongitude          << std::endl
                //           << "dLatitude        : " << data.dLatitude           << std::endl
                //           << "ego_longitude    :  "<< DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().fLongitude <<std::endl
                //           << "ego_latitude     :  "<< DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().fLatitude << std::endl
                //           << "fLongituAcc      : " << data.fLongituAcc         << std::endl
                //           << "fLateralAcc      : " << data.fLateralAcc         << std::endl
                //           << "fHeading         : " << data.fHeading            << std::endl
                //           << "fSpeed           : " << data.fSpeed              << std::endl
                //           << "fSteeringAngle   : " << data.fSteeringAngle      << std::endl
                //           << "fYawRate         : " << data.fYawRate            << std::endl
                //           << "iGpsState        : " << (int)data.iGpsState      << std::endl
                //           << "iGpsTime         : " << data.iGpsTime            << std::endl
                //           << "iShiftPosition   : " << (int)data.iShiftPosition << std::endl;
                // printf("%f\n%f\n%f\n%f\n",data.dLongitude,data.dLatitude,DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().fLongitude,
                //            DataContainer::GetInstance()->ego_vehicle_gps_data_.getData().fLatitude);
                lcm_.publish("V2X_OTHER_VEHICLE_INFO", &(temp.second.getData()));
            }   
        }
    }
}
//
//function:publish worldmodel info to channel
//
void communication::PublishWorldmodelInfo() {
    if (DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
        LINFO << "publish worldmodle vehicle info to lcm";
        WorldModelObjects temp = handler_.GetWorldmodleVehiles();
        std::cout << "###########Publish publish worldmodle vehicle info to lcm#######" << std::endl
                  <<"remote vehilce number is : " << temp.nVehicleNum << std::endl;
        if(temp.nVehicleNum > 0){
            for(auto it : temp.vehicles){
                std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
                for(int i = 0; i < it.hisTrajectory.size(); i++) {
                    std::cout << "the i trajectory relative x is :" << it.hisTrajectory[i].relative_x;
                    std::cout << "the i trajectory relative y is :" << it.hisTrajectory[i].relative_y;
                    std::cout << "the i trajectory relative heading is" << it.hisTrajectory[i].relative_heading;
                }
                std::cout << "vehicle ID" << it.nObjectID << std::endl 
                          <<"vehicle frenet dis is: " << it.frenet_lon_distance << std::endl
                          << "history trajectory point is : " << it.pointNum << std::endl;
            } 
        }        
        lcm_.publish("WORLDMODEL_OTHER_OBJECTS_INFO", &handler_.GetWorldmodleVehiles());
    } else {
        LDEBUG << "publish worldmodel info is not update";
    }
        
}

} // namesapce communication

} // namespace platoon
