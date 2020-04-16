#include "modules/communication/receivelowfreinfo.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/udpdatacontainer.h"
#include "modules/communication/configdata.h"

namespace platoon
{
namespace communication
{
ReceiveLowFreInfo::ReceiveLowFreInfo():lcm_("udpm://239.255.76.67:7667?ttl=1"), loop_("ReceiveLowFreInfo")
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
    if (LowFreDataContanier::GetInstance()->hmi_fms_info.isUpToDate())
    {
        if ( LowFreDataContanier::GetInstance()->hmi_fms_info.getData().fms_order != msg->fms_order)
            std::cout << "asdf HMI FMS order changed : " << int(msg->fms_order) << std::endl;
        if (LowFreDataContanier::GetInstance()->hmi_fms_info.getData().safe_distance != msg->safe_distance)
            std::cout << "asdf safe distance changed : " << msg->safe_distance << std::endl;
        if (LowFreDataContanier::GetInstance()->hmi_fms_info.getData().platoon_number != msg->platoon_number)
        {
            std::cout << "asdf platoon number changed : " << msg->platoon_number << std::endl;
            UDPDataContainer::GetInstance()->platoon_vehicles_data_.getData().clear();
        }
    }
    else
    {
        std::cout << "asdf HMI FMS order changed : " << int(msg->fms_order) << std::endl;
        std::cout << "asdf safe distance changed : " << msg->safe_distance << std::endl;
        std::cout << "asdf platoon number changed : " << msg->platoon_number << std::endl;
    }
    LowFreDataContanier::GetInstance()->hmi_fms_info.setData(*msg);
}

void ReceiveLowFreInfo::HandlePlanningInfo(const lcm::ReceiveBuffer *rbuf,
                                        const std::string &channel,
                                        const EgoPlanningMsg *msg) 
{
    assert(channel == "EGO_PLANNINGMSG_FOR_PLATOON");
    static int plan_count = 0;
    plan_count++;
    if (plan_count % debug_plan_hz_ == 0)
    {
        printf("asdf reveive plan info : %d\n\n", plan_count);
    }
    if (LowFreDataContanier::GetInstance ()->planning_data_.isUpToDate())
    {
        EgoPlanningMsg ego_planning_msg = LowFreDataContanier::GetInstance ()->planning_data_.getData ();
        if (msg->actual_drive_mode != ego_planning_msg.actual_drive_mode)
        {
            printf ("asdf actual_drive_mode changed: %d\n", msg->actual_drive_mode);
        }
    }
    else 
    {
        printf ("asdf actual_drive_mode changed: %d\n", msg->actual_drive_mode);
    }
    LowFreDataContanier::GetInstance()->planning_data_.setData(*msg);
}

bool ReceiveLowFreInfo::ReceiveFmsPreInfo()
 { 
    bool result  = RxMsgPtr<FMSPreFormationInfo>("FMS_PRE_INFO", pre_info_ptr_);
    if(result)
    {
        if (LowFreDataContanier::GetInstance()->fms_pre_info_.isUpToDate())
        {
            std::string old_id = LowFreDataContanier::GetInstance()->fms_pre_info_.getData().id();
            std::string new_id = pre_info_ptr_->id();
            if (new_id == old_id)
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
               UDPDataContainer::GetInstance()->platoon_vehicles_data_.getData().clear();
            }
        }
        else
        {
            std::cout << "FMS Pre Info Changed! " << std::endl;
            LowFreDataContanier::GetInstance()->fms_pre_info_.setData(*pre_info_ptr_);
        }
            
    }
    return result;
 }

 bool ReceiveLowFreInfo::ReceiveFmsApplyBack()
 {
    bool result = RxMsgPtr<FMSApplyResultInfo>("FMS_APPLY_RESULT_INFO", apply_result_ptr_);
    if (result)
    {
        if (LowFreDataContanier::GetInstance()->fms_apply_result_.isUpToDate())
        {
            std::string old_id = LowFreDataContanier::GetInstance()->fms_apply_result_.getData().id();
            std::string new_id = apply_result_ptr_->id();
            if (new_id == old_id)
            {
                result = false;
            }
            else
            {
                std::cout << "FMS Apply Back is changed ! " << std::endl;
                LowFreDataContanier::GetInstance()->fms_apply_result_.setData(*apply_result_ptr_);
            } 
        }
        else
        {
            std::cout << "FMS Apply Back is changed ! " << std::endl;
            LowFreDataContanier::GetInstance()->fms_apply_result_.setData(*apply_result_ptr_);
        }
        
    }
    return result;
 }
}//namespace communication
}//namespace platoon