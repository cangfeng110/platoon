#ifndef _FMS_H
#define _FMS_H
#include <vector>

#include "modules/communication/datadefine.h"

#include "include/proto/ProtoClassNameyTypeDefine.h"
namespace platoon
{
namespace communication
{
class FMS
{
private:
    std::vector<double> m_enqueue_point_;
    std::vector<double> m_dequeue_point_;
    FmsOrder m_fms_order_;
    bool m_hmi_fms_valid_;

    ToFMSInfo m_to_fms_info_;

private:
    void UpdatePoint();
    ApplyResult CalApplyResult();
    void CalApplyOrder();
    bool CalIfDisBand();
    void ResetApplyResult();
    void CalFmsOrder();

    //print function
    void PrintApplyOrder(const ApplyOrder& order);
    void PrintApplyBack(const ApplyResult& result);
    void PrintToFmsInfo();
    void PrintFmsPreInfo();
    void PrintFmsBackInfo();

public:
    FMS();
    const ToFMSInfo& GetToFmsInfo() const;
    void UpdateToFmsInfo();
    void UpdateFmsOrder();
};

}//communication
}//namespace platoon




#endif