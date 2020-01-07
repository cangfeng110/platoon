#include <tjP/protocol/ProtocolChannel.h>

#include <tjP/base/EventLoop.h>
#include <tjP/base/Channel.h>

#include <tjP/communication/commu.h>

#include <string>
#include <iostream>

namespace tjP
{

namespace communication
{

commu::commu()
    : _lcm("udpm://239.255.76.67:7667?ttl=1"),
      _loop("commu")
{
    // Logger
    if(CommuConfig::getInstance()->debug)
    {
        base::Logging::setLogLevel(base::Logging::Debug);
    }
    else
    {
        // no file log
    }

    //Lcm
    if(!_lcm.good()){
        LDIE << "lcm init error";
    }

    //
    _lcm.subscribe(TJP_CHANNEL_NAME_EGO_VEHICLE_INFO, &commu::handleEgoVehicleMessage, this);
    _lcm.subscribe(TJP_CHANNEL_V2X_SELF_TEAM_STATE_INFO, &commu::handleV2xSelfTeamMessage, this);

    // lcm channel
    _lcmChannel.reset(new tjP::base::Channel(&_loop, _lcm.getFileno(), "lcm"));
    _lcmChannel->setReadCallback(std::bind(&lcm::LCM::handle, &_lcm));
    _lcmChannel->enableReading();
    // v2x channel
    _v2xChannel.reset(new tjP::base::Channel(&_loop, _handler.getFileno(), "v2xComm"));
    _v2xChannel->setReadCallback(std::bind(&commu::handleV2xCommuMessage, this));
    _v2xChannel->enableReading();

    // decrease ttl
    _loop.runEvery(100, std::bind(&dataContainer::decreaseTtl, dataContainer::getInstance()));
    // broadcast v2x message
    _loop.runEvery(20, std::bind(&commu::broadCastV2XMessage, this));
}

//###################
//      Decode
//###################
void commu::handleV2xCommuMessage()
{
    if(_handler.bsmDecodeAndAssignValue() > 0)
    {
        if(dataContainer::getInstance()->_v2xOtherVehicleData.isUpToDate())
        {
            auto it = dataContainer::getInstance()->_v2xOtherVehicleData.getData().begin();
            for(;it!=dataContainer::getInstance()->_v2xOtherVehicleData.getData().end();it++)
            {
                // display
                std::cout << "========= display ==========" << std::endl;
                const egoVehicleData &data = it->second.getData();
                std::cout << "iVehicleID       : " << data.iVehicleID    << std::endl
                          << "fVehicleLength   : " << data.fVehicleLength << std::endl
                          << "fVehicleWidth    : " << data.fVehicleWidth << std::endl
                          << "iDriveModeStatus : " << (int)data.iDriveModeStatus << std::endl
                          << "fBrakePedalAngle : " << data.fBrakePedalAngle    << std::endl
                          << "dLongitude       : " << data.dLongitude          << std::endl
                          << "dLatitude        : " << data.dLatitude           << std::endl
                          << "fLongituAcc      : " << data.fLongituAcc         << std::endl
                          << "fLateralAcc      : " << data.fLateralAcc         << std::endl
                          << "fHeading         : " << data.fHeading            << std::endl
                          << "fSpeed           : " << data.fSpeed              << std::endl
                          << "fSteeringAngle   : " << data.fSteeringAngle      << std::endl
                          << "fYawRate         : " << data.fYawRate            << std::endl
                          << "iGpsState        : " << (int)data.iGpsState      << std::endl
                          << "iGpsTime         : " << data.iGpsTime            << std::endl
                          << "iShiftPosition   : " << (int)data.iShiftPosition << std::endl;
                // display
                _lcm.publish(TJP_CHANNEL_NAME_V2X_OTHER_VEHICLE_INFO,
                             &(it->second.getData()));
            }
        }
        if(!dataContainer::getInstance()->_v2xOtherTeamData.isLocked())
        {
            _lcm.publish(TJP_CHANNEL_V2X_OTHER_TEAM_STATE_INFO,
                         &(dataContainer::getInstance()->_v2xOtherTeamData.getDataAndLock()));
        }
    }
}

//###################
//      Encode
//###################
void commu::broadCastV2XMessage()
{
    if(!dataContainer::getInstance()->_egoVehicleData.isUpToDate()){
        std::cout<<"ego vehicle out date"<<std::endl;
    }
    if(dataContainer::getInstance()->_egoVehicleData.isUpToDate())
    {
        _handler.bsmEncodeAndBroadCast();
    }
}

void commu::handleEgoVehicleMessage(const lcm::ReceiveBuffer *rbuf,
                                    const std::string &channel,
                                    const egoVehicleData *msg)
{
    assert(channel == TJP_CHANNEL_NAME_EGO_VEHICLE_INFO);
//    LDEBUG << "Receive from ego vehicle info.";
    dataContainer::getInstance()->_egoVehicleData.setData(*msg);
}

//###################
//      Encode
//###################
void commu::handleV2xSelfTeamMessage(const lcm::ReceiveBuffer *rbuf,
                                     const std::string &channel,
                                     const v2xVehicleTeamInfo *msg)
{
    assert(channel == TJP_CHANNEL_V2X_SELF_TEAM_STATE_INFO);
//    LDEBUG << "Receive from self team info.";
    dataContainer::getInstance()->_v2xEgoTeamData.setDataAndUnlock(*msg);

    // send team message at once
    if(dataContainer::getInstance()->_egoVehicleData.isUpToDate())
    {
        _handler.bsmEncodeAndBroadCast();
    }
}

commu::~commu()
{
    LINFO << "loop end";
}

void commu::loop()
{
    LINFO << "loop start";
    _loop.loop();
    LINFO << "loop end";
}

} // namesapce communication

} // namespace tjP
