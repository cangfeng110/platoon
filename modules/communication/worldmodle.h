#ifndef _WORLDMODLE_H_
#define _WORLDMODLE_H_

#include <iostream>

#include "include/base/NonCopyable.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "modules/template/templateDataContainer.h"

namespace platoon
{
namespace communication 
{

class WorldModle : public base::NonCopyable
{
    public:
        WorldModle(){};

        void GetWorldmodleVehiles();

        double GetFrenetDis(int vehicle_id);

    private:
        WorldModelObjects worldmodel_other_vehicles_data_;

        VehicleGpsData ego_vehicle_gps_info_;
        bool ego_gps_isupdate_;

        std::map<int, templateDataContainer<VehicleData>> platoon_vehicles_info_;
        bool platoon_info_isupdate_;

        std::map<int, templateDataContainer<WorldModelObject> > worldmodle_other_vehicle_data_;
        bool worldmodle_isupdate_;

        void UpdateInfo();

        void TransV2xInfoToWorldmodelInfo(const VehicleData &v2x_vehicle_data, 
                                        WorldModelObject &worldmodel_vehicle_data);
        void ProcessTrajectory(std::vector<Location> &trajectory);   
};

}
}



#endif