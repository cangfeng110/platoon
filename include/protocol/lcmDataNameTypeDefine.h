#ifndef _LCM_DATA_NAME_TYPE_DEFINE_
#define _LCM_DATA_NAME_TYPE_DEFINE_

#include "include/protocol/VCU_VEHICLE_INFO.hpp"

#include "include/protocol/VehicleInfo.hpp"

#include "include/protocol/Localization_Out_2_Map.hpp"

#include "include/protocol/Location.hpp"

#include "include/protocol/WorldmodelVehicle.hpp"

#include "include/protocol/WorldmodelVehicles.hpp"

#include "include/protocol/FmsInfo.hpp"

#include "include/protocol/EgoPlanningMsg.hpp"

#include "include/protocol/PlatoonManagerInfo.hpp"


typedef platoon::protocol::VCU_VEHICLE_INFO             VehicleVcuData;         
typedef platoon::protocol::Localization_Out_2_Map       VehicleGpsData;
typedef platoon::protocol::VehicleInfo                  VehicleData;

typedef platoon::protocol::Location                     Location;
typedef platoon::protocol::WorldmodelVehicle            WorldModelObject;
typedef platoon::protocol::WorldmodelVehicles           WorldModelObjects;

typedef platoon::protocol::FmsInfo                      HmiFmsInfo;
typedef platoon::protocol::EgoPlanningMsg               EgoPlanningMsg;
typedef platoon::protocol::PlatoonManagerInfo           PlatoonManagerInfo;

#endif // _LCM_DATA_NAME_TYPE_DEFINE_
