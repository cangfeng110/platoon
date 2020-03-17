#ifndef _WORLDMODLE_H_
#define _WORLDMODLE_H_

#include <iostream>

#include "include/base/NonCopyable.h"
#include "include/protocol/lcmDataNameTypeDefine.h"
#include "modules/communication/datacontainer.h"
#include "modules/communication/configdata.h"

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

        void TransV2xInfoToWorldmodelInfo(const VehicleData &v2x_vehicle_data, 
                                        WorldModelObject &worldmodel_vehicle_data);
        void ProcessTrajectory(std::vector<Location> &trajectory);   
};

}
}



#endif