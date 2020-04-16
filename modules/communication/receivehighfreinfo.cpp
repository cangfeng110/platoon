#include "modules/communication/receivehighfreinfo.h"
#include "configdata.h"
namespace platoon
{
namespace communication
{

ReceiveHighFreInfo::ReceiveHighFreInfo() : lcm_("udpm://239.255.76.67:7667?ttl=1"), loop_("ReceiveHighFreInfo")
{
    if (!lcm_.good())
    {
        LDIE << " high fre lcm init error! ";
    }

    lcm_.subscribe("localization_out_2_map", &ReceiveHighFreInfo::HandleEgoVehicleGpsInfo, this);
    lcm_.subscribe("VCU_VEHICLE_INFO", &ReceiveHighFreInfo::HandleEgoVehicleVcuInfo, this);

    lcm_channel_.reset(new base::Channel(&loop_, lcm_.getFileno(), "ReceivHighFreLcm"));
    lcm_channel_->setReadCallback(std::bind(&lcm::LCM::handle, &lcm_));
    lcm_channel_->enableReading();

    loop_.runEvery(100, std::bind(&HighFreDataContainer::DecreaseTtl, HighFreDataContainer::GetInstance()));

    debug_gps_hz_ = ConfigData::GetInstance()->GetDebugGpsHZ();
    debug_vcu_hz_ = ConfigData::GetInstance()->GetDebugVcuHZ();
}

ReceiveHighFreInfo::~ReceiveHighFreInfo()
{
    LINFO << " Receive High Frequency lcm info is end ";
}

void ReceiveHighFreInfo::Loop()
{
    LINFO << " Receive High Frequency lcm info is start " ;
    loop_.loop();
    LINFO << " Receive High Frequency lcm info is end ";
}


//
//function:receive ego gps info and save to datacontainer
//
void ReceiveHighFreInfo::HandleEgoVehicleGpsInfo(const lcm::ReceiveBuffer *rbuf,
                                    const std::string &channel,
                                    const VehicleGpsData *msg)
{
    assert(channel == "localization_out_2_map");
    static int gps_count = 0;
    gps_count++;
    if (gps_count % debug_gps_hz_ == 0)
    {
        printf ("asdf localization_out_2_map received %d\n\n", gps_count);
    }
    HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.setData(*msg);
}

//
//function:receive ego vcu info and save to datacontainer
//
void ReceiveHighFreInfo::HandleEgoVehicleVcuInfo(const lcm::ReceiveBuffer *rbuf,
                                     const std::string &channel,
                                     const VehicleVcuData *msg) 
{
    assert(channel == "VCU_VEHICLE_INFO");
    static int vcu_count = 0;
    vcu_count++;
    if (vcu_count % debug_vcu_hz_ == 0)
    {
        printf ("asdf VCU_VEHICLE_INFO received %d\n\n", vcu_count);
    }
    HighFreDataContainer::GetInstance()->ego_vehicle_vcu_data_.setData(*msg);
}

}//namespace communication
}//namespace platoon