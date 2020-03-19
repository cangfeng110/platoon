#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"

namespace atd {
namespace ipc {

std::shared_ptr<SupperChannelManager> SupperChannelManager::instance(new SupperChannelManager());

SupperChannelManager::SupperChannelManager():SupperChannelManagerBase()
{

}

SupperChannelManager::~SupperChannelManager()
{

}

std::shared_ptr<SupperChannelManager> SupperChannelManager::getInstance()
{
    return instance;
}

bool SupperChannelManager::disableThread(const std::string ChannelName)
{
    {
        auto guard= m_RWLockthreadPool.read_guard();
        if(threadPool.find(ChannelName)!=threadPool.end())
        {
            return true;
        }
    }
    return false;
}

SupperChannelManagerBase::~SupperChannelManagerBase()
{

}

}
}


