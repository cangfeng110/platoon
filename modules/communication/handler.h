﻿#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/base/NonCopyable.h"
#include "protocol/lcmDataNameTypeDefine.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/remote_info.h"
#include "modules/communication/outbound_communication_header.h"



namespace platoon
{

namespace communication
{

#define MAX_RECV_LENGTH 1024

class Handler : public base::NonCopyable
{
public:
    Handler();

    int GetFileno()
    {
        return sockfd_;
    }

    // 
    int BroastEgoVehicleVcuInfo();

    int BroastEgoVehicleGpsInfo();

    // 
    int DecodeV2xVechileInfo();

    //
    WorldModelObjects &GetWorldmodleVehiles();

private:
    // socket
    int sockfd_;
    struct sockaddr_in remote_sockaddr_;
    struct sockaddr_in local_sockaddr_;
    
    char buffer_[MAX_RECV_LENGTH];
    // 
    WorldModelObjects worldmodel_other_vehicles_data_;

    void TransV2xInfoToWorldmodelInfo(const VehicleData &v2x_vehicle_data, 
                                        WorldModelObject &worldmodel_vehicle_data);
    void ProcessTrajectory(std::vector<Location> &trajectory);
};

} // namespace communication

} // namespace platoon

#endif // _HANDLER_H_
