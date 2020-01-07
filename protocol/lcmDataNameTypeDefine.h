#ifndef _LCM_DATA_NAME_TYPE_DEFINE_
#define _LCM_DATA_NAME_TYPE_DEFINE_

#include "protocol/VCU_VEHILCE_INFO.hpp"

#include "protocol/EgoVehicleInfo.hpp"

#include "protocol/INERTIAL_Debug.hpp"

#include "protocol/Location.hpp"

#include "protocol/WorldmodelVehicle.hpp"

#include "protocol/WorldmodelVehicles.hpp"




typedef platoon::protocol::VCU_VEHILCE_INFO             VehicleVcuData;         
typedef platoon::protocol::INERTIAL_Debug               VehicleGpsData;
typedef platoon::protocol::EgoVehicleInfo               VehicleData;

typedef platoon::protocol::Location                     Location;
typedef platoon::protocol::WorldmodelVehicle            WorldModelObject;
typedef platoon::protocol::WorldmodelVehicles           WorldModelObjects;

#endif // _LCM_DATA_NAME_TYPE_DEFINE_
