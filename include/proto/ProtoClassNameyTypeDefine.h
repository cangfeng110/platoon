#ifndef _PROTO_CLASS_NAME_TYPE_DEFINE_
#define _PROTO_CLASS_NAME_TYPE_DEFINE_

#include "include/proto/DataMessageProto.pb.h"

namespace platoon
{
namespace communication
{
    typedef VehicleFleetOperation                                   FMSPreFormationInfo;
    typedef Fleet_node                                              RefPoint;
    typedef VehicleFleetResult                                      FMSApplyResultInfo;
    typedef VehicleFleetInfo                                        ToFMSInfo;
    typedef VehicleFleetInfo::VehicleFleetMode                      FmsDriveMode;
    typedef FleetApplyType                                          FmsApplyOrder;
}//namespace communication
}//namespace platoon

#endif