#include "worldmodle.h"

#include <math.h>

#include "modules/communication/highfredatacontainer.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/udpdatacontainer.h"
#include "modules/communication/senddatacontanier.h"
#include "modules/customfunction/functiontool.h"

namespace platoon
{
namespace communication 
{

void WorldModle::GetWorldmodleVehiles() {
    if (UDPDataContainer::GetInstance()->platoon_vehicles_data_.isUpToDate()) {
        auto temp = UDPDataContainer::GetInstance()->platoon_vehicles_data_.getData();
        for(auto map_it : temp) 
        {
            const VehicleData v2x_other_vehicle_data = map_it.second.getData();
            const int key = v2x_other_vehicle_data.vehicle_id;
            WorldModelObject worldmodel_vehicle_data =
                SendDataContanier::GetInstance()->worldmodle_other_vehicle_data_.getData()[key].getData();
            worldmodel_vehicle_data.vehicle_id = v2x_other_vehicle_data.vehicle_id;
            worldmodel_vehicle_data.actual_drive_mode = v2x_other_vehicle_data.actual_drive_mode;
            worldmodel_vehicle_data.desire_drive_mode = v2x_other_vehicle_data.desire_drive_mode;
            TransV2xInfoToWorldmodelInfo(v2x_other_vehicle_data, worldmodel_vehicle_data);
            SendDataContanier::GetInstance()->worldmodle_other_vehicle_data_.getData()[key].setData(worldmodel_vehicle_data); 
        }
    }
    worldmodel_other_vehicles_data_.vehicle_num = 0;
    worldmodel_other_vehicles_data_.vehicles.clear();
    if (SendDataContanier::GetInstance()->worldmodle_other_vehicle_data_.isUpToDate()) {
        auto temp =  SendDataContanier::GetInstance()->worldmodle_other_vehicle_data_.getData();
        for (auto map_it : temp) 
        {    
            WorldModelObject worldmodel_vehicle_data = map_it.second.getData();
            worldmodel_other_vehicles_data_.vehicles.push_back(worldmodel_vehicle_data);
        }
    }  
    worldmodel_other_vehicles_data_.vehicle_num = worldmodel_other_vehicles_data_.vehicles.size();
}

//
// save othervehicle trajectoty info to worldmodel info
//
 void WorldModle::TransV2xInfoToWorldmodelInfo(const VehicleData &v2x_vehicle_data, 
                                        WorldModelObject &worldmodel_vehicle_data) {
    Location temp;
    temp.longitude = v2x_vehicle_data.longitude;
    temp.latitude = v2x_vehicle_data.latitude;
    temp.altitude = v2x_vehicle_data.altitude;
    temp.heading = v2x_vehicle_data.heading;
    temp.gps_status = v2x_vehicle_data.gps_status;
    temp.gps_time = v2x_vehicle_data.gps_time;
    temp.relative_x = v2x_vehicle_data.relative_x;
    temp.relative_y = v2x_vehicle_data.relative_y;
    temp.relative_heading = v2x_vehicle_data.relative_heading;
    temp.longtitude_acc = v2x_vehicle_data.longtitude_acc;
    temp.lateral_acc = v2x_vehicle_data.lateral_acc;
    temp.speed = v2x_vehicle_data.speed;
    temp.steering_wheel_angle = v2x_vehicle_data.steering_wheel_angle;
    temp.yaw_rate = v2x_vehicle_data.yaw_rate;
    temp.desire_long_acc = v2x_vehicle_data.desire_long_acc;

    // insert new location info if other vehicle speed is not zero 
    // and new location is far from the previous location
    if (worldmodel_vehicle_data.trajectory.size() == 0)
        worldmodel_vehicle_data.trajectory.push_back(temp);
    else {
        double long_error = fabs(worldmodel_vehicle_data.trajectory.back().longitude - temp.longitude);
        double lat_error = fabs(worldmodel_vehicle_data.trajectory.back().latitude - temp.latitude);
        if ((temp.speed > 0.001) && (long_error > 0.0000001 || lat_error > 0.0000001))
            worldmodel_vehicle_data.trajectory.push_back(temp);
        else 
            worldmodel_vehicle_data.trajectory.back() = temp;
    }
    ProcessTrajectory(worldmodel_vehicle_data.trajectory);
    worldmodel_vehicle_data.point_num = worldmodel_vehicle_data.trajectory.size();
     
    if (worldmodel_vehicle_data.trajectory.size() > 0) {
        Location nearest_point = worldmodel_vehicle_data.trajectory.front();
        worldmodel_vehicle_data.frenet_lat_distance = nearest_point.relative_y;

        double dis_temp = 0;
        for (int i = 0; i < worldmodel_vehicle_data.trajectory.size() - 1; i++) {
            double x = worldmodel_vehicle_data.trajectory[i + 1].relative_x - 
                        worldmodel_vehicle_data.trajectory[i].relative_x;
            double y = worldmodel_vehicle_data.trajectory[i + 1].relative_y -
                        worldmodel_vehicle_data.trajectory[i].relative_y;
            dis_temp += sqrt(x * x + y * y);
        }
        dis_temp += sqrt(nearest_point.relative_x * nearest_point.relative_x + 
                        nearest_point.relative_y * nearest_point.relative_y);
        dis_temp -= v2x_vehicle_data.vehicle_length; //subtract vehicle length
        worldmodel_vehicle_data.frenet_lon_distance = dis_temp;
    } else {
//        std::cout << worldmodel_vehicle_data.vehicle_id << " vehicle's history trajectory is all behind ego" << std::endl;
        worldmodel_vehicle_data.frenet_lat_distance = 1.0e10; //invalid
        worldmodel_vehicle_data.frenet_lon_distance = 1.0e10; //invalid
    }   
}

//
//function:erase other vehicle trajectory points behind ego vehicle
//

void WorldModle::ProcessTrajectory(std::vector<Location> &trajectory) {
    //Update relative info
    if(HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
        const  VehicleGpsData ego_vehicle_location = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
        for (int i = 0; i < trajectory.size(); i++) {
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(trajectory[i].relative_x, trajectory[i].relative_y,
                                                                ego_vehicle_location.heading,
                                                                ego_vehicle_location.longitude, ego_vehicle_location.latitude,
                                                                ego_vehicle_location.height,
                                                                trajectory[i].longitude, trajectory[i].latitude,
                                                                trajectory[i].altitude);
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(trajectory[i].relative_heading, 
                                                                ego_vehicle_location.heading, trajectory[i].heading);
        }
    }
    // find the first point infront of ego vehicle
    int index_near = -1;
    for (int i = 0; i < trajectory.size();i++) {
        if(trajectory[i].relative_x > 0) { 
            index_near = i;
            break;
        }        
    }
    if (index_near != -1) 
        trajectory.erase(trajectory.begin(), trajectory.begin() + index_near);
    else if (index_near == -1)
        trajectory.clear();
}

double WorldModle::GetFrenetDis(int vehicle_id)
{
    if (worldmodel_other_vehicles_data_.vehicle_num < 1)
        return 1.0e10;
    for (auto temp : worldmodel_other_vehicles_data_.vehicles){
        if(temp.vehicle_id == vehicle_id)
            return temp.frenet_lon_distance;
    }
    return 1.0e10;
}

}
}
