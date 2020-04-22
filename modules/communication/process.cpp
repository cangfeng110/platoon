#include "modules/communication/process.h"
#include "modules/communication/senddatacontanier.h"
#include "modules/communication/lowfredatacontainer.h"

namespace platoon
{
namespace communication
{
Process::Process() : lcm_("udpm://239.255.76.67:7667?ttl=1"),loop_("MainProcess")
{
    if (!lcm_.good())
    {
        LDIE << " main process lcm init error! ";
    }
    debug_pmi_HZ_ = ConfigData::GetInstance()->GetDebugPmiHZ();

    if (ConfigData::GetInstance()->is_sil_test_)
    {
        loop_.runEvery(1000 / ConfigData::GetInstance()->broadcast_HZ_, std::bind(&SendPDM::SilBroastEgoVehicleInfo, send_pdm_));
    }
    else if (ConfigData::GetInstance()->is_protocol_2_)
    {
        loop_.runEvery(1000 / ConfigData::GetInstance()->broadcast_HZ_, std::bind(&SendPDM::BroastEgoVehicleInfo2, send_pdm_));
    }
    else
    {
        loop_.runEvery(1000 / ConfigData::GetInstance()->broadcast_HZ_, std::bind(&SendPDM::BroastEgoVehicleInfo3, send_pdm_));
    }

    loop_.runEvery(1000 / ConfigData::GetInstance()->broadcast_HZ_, std::bind(&Process::PublishManagerInfo, this));

    loop_.runEvery(50, std::bind(&Process::PublishToFusionInfo, this));

    loop_.runEvery(1000, std::bind(&FMS::UpdateFmsOrder, fms_));

    loop_.runEvery(1000, std::bind(&FMS::UpdateToFmsInfo, fms_));

    loop_.runEvery(1000, std::bind(&Process::PublishToFmsInfo, this));
}

Process::~Process()
{
     LINFO << " Main Process is end ";
}

void Process::Loop()
{
     LINFO << " Main Process is start ";
     loop_.loop();
     LINFO << " Main Process is end ";
}

void Process::PublishManagerInfo()
{
    static int pmi_count = 0;
    manager_.UpdatePlatoonManagerInfo ();
    bool isupdate;
    const PlatoonManagerInfo& data = SendDataContanier::GetInstance ()->manager_data_.getData(isupdate);
    if (isupdate)
    {
        lcm_.publish ("PLATOON_MANAGER_INFO", &data);
        pmi_count++;
        if (pmi_count % debug_pmi_HZ_ == 0)
        {
            printf ("asdf PLATOON_MANAGER_INFO published %d\n\n", pmi_count);
        }
    }
}

void Process::PublishToFmsInfo()
{
    static int tf_count = 0;
    if (LowFreDataContanier::GetInstance()->fms_pre_info_.isUpToDate())
    {
        ToFMSInfo temp = fms_.GetToFmsInfo();
        sendMessageViaLcm<ToFMSInfo>("PLATOON_APPLY_INFO", temp);
        ++tf_count;
        if (tf_count % debug_pmi_HZ_ == 0)
        {
            printf ("asdf PLATOON_APPLY_INFO published %d\n\n", tf_count);
        }
    }
}

void Process::PublishToFusionInfo()
{
    static int tf_count = 0;
    bool isupdate;
    const ToFusionData& temp = SendDataContanier::GetInstance()->to_fusion_data_.getData(isupdate);
    if (isupdate)
    {
        lcm_.publish ("TO_FUSION_INFO", &temp);
        ++tf_count;
        if (tf_count % debug_pmi_HZ_ == 0)
        {
            printf ("asdf TO_FUSION_INFO published %d\n\n", tf_count);
        }
    }
}

}//namespace communication
}//namespace platoon