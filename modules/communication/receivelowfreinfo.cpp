#include <sys/time.h>

#include "modules/communication/receivelowfreinfo.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/udpdatacontainer.h"
#include "modules/communication/configdata.h"
#include "modules/communication/datadefine.h"

namespace platoon
{
namespace communication
{
ReceiveLowFreInfo::ReceiveLowFreInfo() : lcm_("udpm://239.255.76.67:7667?ttl=1"), loop_("ReceiveLowFreInfo"), 
                                       actual_drive_mode_(7), fms_order_(0), safe_distance_(10.0),platoon_number_(0)
{
    if (!lcm_.good())
    {
        LDIE << " low fre lcm init error! ";
    }

    lcm_.subscribe("FMS_INFO",&ReceiveLowFreInfo::HandleHmiFmsInfo, this);
    lcm_.subscribe("EGO_PLANNINGMSG_FOR_PLATOON", &ReceiveLowFreInfo::HandlePlanningInfo, this);

    lcm_channel_.reset(new base::Channel(&loop_, lcm_.getFileno(),"ReceiveLowFreLcm"));
    lcm_channel_->setReadCallback(std::bind(&lcm::LCM::handle, &lcm_));
    lcm_channel_->enableReading();

    debug_plan_hz_ = ConfigData::GetInstance()->debug_plan_HZ_;
    debug_hmi_hz_ = ConfigData::GetInstance()->debug_hmi_HZ_;

    pre_info_ptr_ = std::make_shared<FMSPreFormationInfo>();
    apply_result_ptr_ = std::make_shared<FMSApplyResultInfo>();

    loop_.runEvery(1000, std::bind(&ReceiveLowFreInfo::ReceiveFmsPreInfo, this));
    loop_.runEvery(1000, std::bind(&ReceiveLowFreInfo::ReceiveFmsApplyBack, this));

    loop_.runEvery(100, std::bind(&LowFreDataContanier::DecreaseTtl, LowFreDataContanier::GetInstance()));
}

ReceiveLowFreInfo::~ReceiveLowFreInfo()
{
    LINFO << " Receive Low Frequency lcm info is end ";
}

void ReceiveLowFreInfo::Loop()
{
    LINFO << " Receive Low Frequency lcm info is start ";
    loop_.loop();
    LINFO << "Receive Low Frequency lcm info is end ";
}

void ReceiveLowFreInfo::HandleHmiFmsInfo(const lcm::ReceiveBuffer *rbuf,
                            const std::string &channel,
                            const HmiFmsInfo *msg)
{
    assert(channel == "FMS_INFO");
    static int hmi_count = 0;
    hmi_count++;
    if (hmi_count % debug_hmi_hz_ == 0)
    {
        printf("asdf reveive HMI info : %d\n", hmi_count);
    }
    if(fms_order_ != msg->fms_order)
    {
        std::cout << "asdf HMI FMS order changed : " << int(msg->fms_order) << std::endl;
    }
    if (safe_distance_ != msg->safe_distance)
        std::cout << "asdf safe distance changed : " << msg->safe_distance << std::endl;
    if (platoon_number_ != msg->platoon_number)
    {
        std::cout << "asdf platoon number changed : " << msg->platoon_number << std::endl;
        UDPDataContainer::GetInstance()->platoon_vehicles_data_.clearMap();
    }
    LowFreDataContanier::GetInstance()->hmi_fms_info_.setData(*msg);
    fms_order_ = msg->fms_order;
    safe_distance_ = msg->safe_distance;
    platoon_number_ = msg->platoon_number;
}
void print_drive_mode(const DriveMode& mode);

void ReceiveLowFreInfo::HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                                        const std::string &channel,
                                        const EgoPlanningMsg *msg) 
{
    assert(channel == "EGO_PLANNINGMSG_FOR_PLATOON");
    // static struct timeval tv0;
    // static struct timeval tv1;
    // gettimeofday (&tv1, NULL);
    // printf ("HandlePlanning ms %ld\n", ((tv1.tv_sec - tv0.tv_sec)*1000000 + (tv1.tv_usec - tv0.tv_usec)) / 1000);
    // tv0 = tv1;
    static int plan_count = 0;
    plan_count++;
    if (plan_count % debug_plan_hz_ == 0)
    {
        printf("asdf reveive plan info : %d\n\n", plan_count);
    }
    
    if (actual_drive_mode_ != msg->actual_drive_mode )
    {
        printf ("asdf actual_drive_mode changed: ");
        print_drive_mode(DriveMode(msg->actual_drive_mode));
    }
    LowFreDataContanier::GetInstance()->planning_data_.setData(*msg);
    actual_drive_mode_ = msg->actual_drive_mode;
}

bool ReceiveLowFreInfo::ReceiveFmsPreInfo()
 { 
    bool result  = RxMsgPtr<FMSPreFormationInfo>("FMS_PRE_INFO", pre_info_ptr_);
    if(result)
    {
        if (pre_serial_id_ == pre_info_ptr_->id())
        {
            result = false;
        }
        else
        {
            std::cout << "FMS Pre Info Changed! " << std::endl;
            LowFreDataContanier::GetInstance()->fms_pre_info_.setData(*pre_info_ptr_);
            /**
             * clear paltoon-vehicles-map, because a new platoon is be calcuted
            */
            UDPDataContainer::GetInstance()->platoon_vehicles_data_.clearMap();
            pre_serial_id_ = pre_info_ptr_->id();
        }
    }
    return result;
 }

 bool ReceiveLowFreInfo::ReceiveFmsApplyBack()
 {
    bool result = RxMsgPtr<FMSApplyResultInfo>("FMS_APPLY_RESULT_INFO", apply_result_ptr_);
    if (result)
    {
        if (back_serial_id_ == apply_result_ptr_->id())
        {
            result = false;
        }
        else
        {
            std::cout << "FMS Apply Back is changed ! " << std::endl;
            LowFreDataContanier::GetInstance()->fms_apply_result_.setData(*apply_result_ptr_);
            back_serial_id_ = apply_result_ptr_->id();
        }
    }
    return result;
 }
}//namespace communication
}//namespace platoon