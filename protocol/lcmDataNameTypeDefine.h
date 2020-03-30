#ifndef _LCM_DATA_NAME_TYPE_DEFINE_
#define _LCM_DATA_NAME_TYPE_DEFINE_

#include "protocol/VCU_VEHICLE_INFO.hpp"

#include "protocol/VehicleInfo.hpp"

#include "protocol/Localization_Out_2_Map.hpp"

#include "protocol/Location.hpp"

#include "protocol/WorldmodelVehicle.hpp"

#include "protocol/WorldmodelVehicles.hpp"

#include "protocol/FmsInfo.hpp"

#include "protocol/EgoPlanningMsg.hpp"

#include "protocol/PlatoonManagerInfo.hpp"
#include "protocol/PlatoonDisplay.hpp"


typedef platoon::protocol::VCU_VEHICLE_INFO             VehicleVcuData;         
typedef platoon::protocol::Localization_Out_2_Map       VehicleGpsData;
typedef platoon::protocol::VehicleInfo                  VehicleData;

typedef platoon::protocol::Location                     Location;
typedef platoon::protocol::WorldmodelVehicle            WorldModelObject;
typedef platoon::protocol::WorldmodelVehicles           WorldModelObjects;

typedef platoon::protocol::FmsInfo                      FmsInfo;
typedef platoon::protocol::EgoPlanningMsg               EgoPlanningMsg;
typedef platoon::protocol::PlatoonManagerInfo           PlatoonManagerInfo;
typedef platoon::protocol::PlatoonDisplay               PlatoonDisplay;

#endif // _LCM_DATA_NAME_TYPE_DEFINE_
