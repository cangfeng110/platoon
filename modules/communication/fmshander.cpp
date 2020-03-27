#include "fmshandler.h"

namespace platoon
{
namespace communication
{

FmsHandler::FmsHandler()
{
    pre_info_ptr_ = std::make_shared<FMSPreFormationInfo>();   
    apply_result_ptr_ = std::make_shared<FMSApplyResultInfo>();
}

 bool FmsHandler::ReceiveFmsPreInfo()
 { 
    bool result  = RxMsgPtr<FMSPreFormationInfo>("FMS_PRE_INFO", pre_info_ptr_);
    if(result)
    {
        if (FmsData::GetInstance()->fms_pre_info_.isUpToDate())
        {
            std::string old_id = FmsData::GetInstance()->fms_pre_info_.getData().id();
            std::string new_id = pre_info_ptr_->id();
            if (new_id == old_id)
            {
                result = false;
            }
            else
            {
                std::cout << "FMS Pre Info Changed! " << std::endl;
                FmsData::GetInstance()->fms_pre_info_.setData(*pre_info_ptr_);
            }
        }
        else
        {
            std::cout << "FMS Pre Info Changed! " << std::endl;
            FmsData::GetInstance()->fms_pre_info_.setData(*pre_info_ptr_);
        }
            
    }
    return result;
 }

 bool FmsHandler::ReceiveFmsApplyBack()
 {
    bool result = RxMsgPtr<FMSApplyResultInfo>("FMS_APPLY_RESULT_INFO", apply_result_ptr_);
    if (result)
    {
        if (FmsData::GetInstance()->fms_apply_result_.isUpToDate())
        {
            std::string old_id = FmsData::GetInstance()->fms_apply_result_.getData().id();
            std::string new_id = apply_result_ptr_->id();
            if (new_id == old_id)
            {
                result = false;
            }
            else
            {
                std::cout << "FMS Apply Back is changed ! " << std::endl;
                FmsData::GetInstance()->fms_apply_result_.setData(*apply_result_ptr_);
            } 
        }
        else
        {
            std::cout << "FMS Apply Back is changed ! " << std::endl;
            FmsData::GetInstance()->fms_apply_result_.setData(*apply_result_ptr_);
        }
        
    }
    return result;
 }

}
}