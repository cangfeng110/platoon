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
    if(CommuConfig::GetInstance()->debug_) {
        base::Logging::setLogLevel(base::Logging::Debug);
    }
    //Lcm
    if(!lcm_.good()) {
        LDIE << "lcm init error";
    }

    //receive ego gps/vcu information from lcm
    lcm_.subscribe(PLATOON_CHANNEL_NAME_VCU_VEHICLE_INFO, &communication::HandleEgoVehicleVcuInfo, this);
    lcm_.subscribe(PLATOON_CHANNEL_NAME_INERTIAL, &communication::HandleEgoVehicleGpsInfo, this);

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
    // broadcast v2x message
    loop_.runEvery(20, std::bind(&communication::BroastEgoVehicleInfo, this));
}

//
//
//
void communication::ReceiveV2xOtherVehicleInfo() {
    if(handler_.DecodeV2xVechileInfo() > 0) {
        if(DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
            for (auto temp : DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()) {
                std::cout << "========= display ==========" << std::endl;
                const VehicleData &data = temp.second.getData();
                std::cout << "iVehicleID       : " << data.iVehicleID    << std::endl
                          << "fVehicleLength   : " << data.fVehicleLength << std::endl
                          << "fVehicleWidth    : " << data.fVehicleWidth << std::endl
                          << "iDriveModeStatus : " << (int)data.iDriveModeStatus << std::endl
                          << "fBrakePedalAngle : " << data.fBrakePedalAngle    << std::endl
                          << "dLongitude       : " << data.dLongitude          << std::endl
                          << "dLatitude        : " << data.dLatitude           << std::endl
                          << "fLongituAcc      : " << data.fLongituAcc         << std::endl
                          << "fLateralAcc      : " << data.fLateralAcc         << std::endl
                          << "fHeading         : " << data.fHeading            << std::endl
                          << "fSpeed           : " << data.fSpeed              << std::endl
                          << "fSteeringAngle   : " << data.fSteeringAngle      << std::endl
                          << "fYawRate         : " << data.fYawRate            << std::endl
                          << "iGpsState        : " << (int)data.iGpsState      << std::endl
                          << "iGpsTime         : " << data.iGpsTime            << std::endl
                          << "iShiftPosition   : " << (int)data.iShiftPosition << std::endl;
                lcm_.publish(PLATOON_CHANNEL_NAME_V2X_OTHER_VEHICLE_INFO, &(temp.second.getData()));
            }   
        }
    }
}


void communication::BroastEgoVehicleInfo()
{
    if(!DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate() ||
        !DataContainer::GetInstance()->ego_vehicle_vcu_data_.isUpToDate()){
        std::cout<<"ego vehicle out date"<<std::endl;
    } else {
        handler_.BroastEgoVehicleGpsInfo();
        handler_.BroastEgoVehicleVcuInfo();
    }
    
}

void communication::HandleEgoVehicleGpsInfo(const lcm::ReceiveBuffer *rbuf,
                                    const std::string &channel,
                                    const VehicleGpsData *msg)
{
    assert(channel == PLATOON_CHANNEL_NAME_INERTIAL);
    LDEBUG << "receive ego vehicle gps info.";
    DataContainer::GetInstance()->ego_vehicle_gps_data_.setData(*msg);
}


void communication::HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                     const std::string &channel,
                                     const VehicleVcuData *msg)
{
    assert(channel == PLATOON_CHANNEL_NAME_VCU_VEHICLE_INFO);
//    LDEBUG << "Receive from self team info.";
    DataContainer::GetInstance()->ego_vehicle_vcu_data_.setData(*msg);   
}

communication::~communication() {
    LINFO << "loop end";
}

void communication::Loop() {
    LINFO << "loop start";
    loop_.loop();
    LINFO << "loop end";
}

} // namesapce communication

} // namespace platoon
