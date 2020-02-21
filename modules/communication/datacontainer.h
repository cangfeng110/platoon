#ifndef _DATA_CONTAINER_H_
#define _DATA_CONTAINER_H_

#include <memory>
#include <map>
#include <assert.h>

// #include "modules/communication/communication_config.h"
#include "protocol/lcmDataNameTypeDefine.h"
#include "modules/template/templateDataContainer.h"

namespace platoon {

namespace communication {

class DataContainer {
public:
    // ego vehicle vcu/gps information from pegasus
    templateDataContainer<VehicleVcuData> ego_vehicle_vcu_data_;
    templateDataContainer<VehicleGpsData> ego_vehicle_gps_data_;

    // v2x other vehicle information from ibox
    templateDataContainer_Map<VehicleData> v2x_other_vehicle_data_;

    // other vehicle information in ego vehicle coordination system
    templateDataContainer_Map<WorldModelObject> worldmodle_other_vehicle_data_;

    // manager data
    templateDataContainer<PlatoonManagerInfo> manager_data_;

    // planning data
    templateDataContainer<EgoPlanningMsg> planning_data_;

    static DataContainer *GetInstance();
    void DecreaseTtl();

private:
    static DataContainer *instance_;

    DataContainer(){ }
    DataContainer(const DataContainer&) = delete;
    DataContainer& operator=(const DataContainer&) = delete;
};

} // namespace communication

} // namespace platoon

#endif // _DATA_CONTAINER_H_
