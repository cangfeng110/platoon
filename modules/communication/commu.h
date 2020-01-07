#ifndef _COMMU_H_
#define _COMMU_H_

#include <lcm/lcm-cpp.hpp>

#include <tjP/base/CountDownLatch.h>
#include <tjP/base/EventLoop.h>
#include <tjP/base/Logging.h>
#include <tjP/communication/commuConfig.h>

#include <tjP/protocol/lcmDataNameTypeDefine.h>
#include <tjP/communication/dataContainer.h>
#include <tjP/communication/handler.h>

#include <memory>

namespace tjP
{

namespace communication
{

class commu : public base::NonCopyable
{
public:
    commu();
    ~commu();

    void loop();

private:
    void handleEgoVehicleMessage(const lcm::ReceiveBuffer *rbuf,
                                 const std::string &string,
                                 const egoVehicleData *msg);
    void handleV2xSelfTeamMessage(const lcm::ReceiveBuffer *rbuf,
                                  const std::string &string,
                                  const v2xVehicleTeamInfo *msg);

    void handleV2xCommuMessage();
    void broadCastV2XMessage();

    std::unique_ptr<base::FileLogger> _logger;
    base::EventLoop _loop;

    // lcm channel
    lcm::LCM _lcm;
    std::unique_ptr<base::Channel> _lcmChannel;
    // v2x channel
    std::unique_ptr<base::Channel> _v2xChannel;

    // handler
    handler _handler;
};

} // namesapce communication

} // namespace tjP

#endif // _COMMU_H_
