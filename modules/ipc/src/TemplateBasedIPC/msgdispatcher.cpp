#include "modules/ipc/include/TemplateBasedIPC/msgdispatcher.h"


namespace  atd
{
namespace ipc
{

MsgDispatcherBase::MsgDispatcherBase(std::string channelname):
    m_pLCM(new lcm::LCM("udpm://239.255.76.67:7667?ttl=3")),
    mychannelname(channelname),
    m_bInitialized(false)
{

}

}
}
