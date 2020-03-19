#include "modules/ipc/include/TemplateBasedIPC/datastreamcontainer.h"

namespace atd
{
namespace ipc
{

void BaseData::setTimeStamp(const common::time::Timestamp &rVal)
{
    timestamp = std::move(rVal);
}



void BaseData::setTimeStamp()
{
    timestamp = common::time::Clock::Now();
}


bool BaseData::isFresher(const BaseData &rVal)
{
    bool moreFresherData = true;
    auto timeGap = std::chrono::duration_cast<std::chrono::microseconds>(timestamp - rVal.timestamp);
    if(common::time::ToMicroSecond(timeGap) > 0)
    {
        moreFresherData = false;
    }
    return moreFresherData;
}

Timestamp BaseData::getTimeStamp()
{
    return timestamp;
}
}
}
