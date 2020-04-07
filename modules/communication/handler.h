﻿#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/base/NonCopyable.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/configdata.h"



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

     // broast ego vehicle info
    int BroastEgoVehicleInfo();

    // receive v2x other info from ibox
    int DecodeV2xVechileInfo();
    
    // sil broast ego vehicle info
    int SilBroastEgoVehicleInfo();

    // sil receive v2x other info from ibox
    int SilDecodeV2xVechileInfo();

    // protocol2.0 broast ego vehicle info
    int BroastEgoVehicleInfo(int);
    int DecodeV2xVehicleInfo(int);

private:
    // socket
    int sockfd_;
    struct sockaddr_in local_sockaddr_;
    
    char buffer_[MAX_RECV_LENGTH];

    int m_debug_flags;

    static int send_number_;
};

} // namespace communication

} // namespace platoon

#endif // _HANDLER_H_
