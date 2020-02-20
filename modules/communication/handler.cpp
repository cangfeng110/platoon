#include "modules/communication/handler.h"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <fstream>

#include "include/base/Timestamp.h"
#include "include/base/Logging.h"

#include "modules/common/functiontool.h"
#include "modules/communication/inbound_gnss_data.h"
#include "modules/communication/inbound_communication_header.h"
#include "modules/communication/inbound_can_data.h"
#include "modules/communication/udp.h"

namespace platoon {

namespace communication {

#define ACCURACY_10 1E10
#define ACCURACY_7 1E7
const double PI = 3.1415926;

Handler::Handler() {
    if((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        LDIE << "v2x socket create failed.";
    }

    local_sockaddr_.sin_family = AF_INET;
    local_sockaddr_.sin_port = htons(16820);  
    local_sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if(::bind(sockfd_, (struct sockaddr *) & local_sockaddr_,
              sizeof(local_sockaddr_)) < 0){
        LDIE << "local port bind error.";
    } else {
        LINFO << "local port bind success.";
    }
}
//
//function: broast ego vehicle info to ibox
//
int Handler::BroastEgoVehicleInfo() {
    // read gps data
    const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
    // read vcu data
    const VehicleVcuData &ego_vehicle_vcu_data = DataContainer::GetInstance()->ego_vehicle_vcu_data_.getData();

    //assign ego vehicle info
    VehicleData ego_vehicle_info;

    ego_vehicle_info.header = ego_vehicle_gps_data.header;
    ego_vehicle_info.vehicle_id = 0;
    ego_vehicle_info.vehicle_length = 0;
    ego_vehicle_info.vehicle_width = 0;
    ego_vehicle_info.vehicle_height = 0;
    // fms info
    ego_vehicle_info.actual_drive_mode = 0;
    ego_vehicle_info.desire_drive_mode = 0;
    //gps info
    ego_vehicle_info.longitude = ego_vehicle_gps_data.longitude;
    ego_vehicle_info.latitude = ego_vehicle_gps_data.latitude;
    ego_vehicle_info.altitude = ego_vehicle_gps_data.height;
    ego_vehicle_info.heading = ego_vehicle_gps_data.heading;
    ego_vehicle_info.gps_time = ego_vehicle_gps_data.header.nTimeStamp;
//    ego_vehicle_info.gps_status = ego_vehicle_gps_data.weight == 0.701 ? 0.602;//XXX TODO
    
    ego_vehicle_info.relative_x = 0;
    ego_vehicle_info.relative_y = 0;
    ego_vehicle_info.relative_heading = 0;

    //vcu info
    ego_vehicle_info.longtitude_acc = ego_vehicle_vcu_data.fLongituAccel;
    ego_vehicle_info.lateral_acc = ego_vehicle_vcu_data.fLateralAccel;
    ego_vehicle_info.speed = ego_vehicle_vcu_data.fSpeed;
    ego_vehicle_info.steering_wheel_angle = ego_vehicle_vcu_data.fSteeringAngle;
    ego_vehicle_info.yaw_rate = ego_vehicle_vcu_data.fYawRate;

    //control info
    ego_vehicle_info.desire_long_acc = 0;

    // 
    int data_len = sizeof(ego_vehicle_info);

    // assign ip port
    std::string send_ip = "192.168.1.18";
    int send_port = 20001;

    //assign header
    int header_len = 24;
    inbound_communication_header send_header;
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 3;
    send_header.op_type = 2;
    send_header.op_code = 98;
    send_header.op_sn = 0;
    send_header.msg_len = data_len;

    //display
    // std::cout << "ego vehicle longitude is : " << ego_vehicle_gps_data.longitude << std::endl;
    // std::cout << "ego_vehicle latitude is : " << ego_vehicle_gps_data.latitude << std::endl;
    // std::cout << "ego vehilce altitude is :" << ego_vehicle_gps_data.height << std::endl;
    // std::cout << "ego vehicle heading is : " << ego_vehicle_gps_data.heading << std::endl;
    
    // udp send
    Udp sudp(send_ip,send_port);
    sudp.init();
    char * buffer = new char[header_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &ego_vehicle_info, data_len);
    sudp.send(buffer, header_len + data_len);
    delete []buffer;
    std::cout << "broast ego vehicle gps info over" << std::endl;
}
//
//function: receive remote vehicle info from ibox
//
int Handler::DecodeV2xVechileInfo() {

    bzero(buffer_, MAX_RECV_LENGTH);

    int len = ::recvfrom(sockfd_, buffer_, MAX_RECV_LENGTH, 0, NULL, NULL);

    std::cout << "receive length is : " << len << std::endl;

    outbound_communication_header outbound_header;
    int header_len = 36;
    memcpy(&outbound_header, buffer_, header_len);

    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 3 
        || outbound_header.op_type != 2 || outbound_header.op_code != 98) {
        LDEBUG << "header type error!";
        printf("%x\n\n",outbound_header.op_code);
        std::cout << outbound_header.op_code << std::endl;
        return -1;
    } else {
        std::cout << "header type right!" << std::endl;

        char * buffer_temp;
        buffer_temp = &buffer_[header_len]; // skip outbound header

        VehicleData v2x_other_vehicle_info;

        int data_len = sizeof(v2x_other_vehicle_info);

        memcpy(&v2x_other_vehicle_info, buffer_temp, data_len);

        int key = v2x_other_vehicle_info.vehicle_id;

        DataContainer::GetInstance()->v2x_other_vehicle_data_.setData(key, v2x_other_vehicle_info);

        return 1;
    }
}

//
//function: send ego vehicle gps info to ibox
//

WorldModelObjects & Handler::GetWorldmodleVehiles() {
    if (DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
        for(auto map_it : DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()) {
            const VehicleData v2x_other_vehicle_data = map_it.second.getData();
            const int key = v2x_other_vehicle_data.vehicle_id;
            WorldModelObject worldmodel_vehicle_data =
                DataContainer::GetInstance()->worldmodle_other_vehicle_data_.getData()[key].getData();
            worldmodel_vehicle_data.vehicle_id = v2x_other_vehicle_data.vehicle_id;
            worldmodel_vehicle_data.actual_drive_mode = v2x_other_vehicle_data.actual_drive_mode;
            worldmodel_vehicle_data.desire_drive_mode = v2x_other_vehicle_data.desire_drive_mode;
            TransV2xInfoToWorldmodelInfo(v2x_other_vehicle_data, worldmodel_vehicle_data);
            DataContainer::GetInstance()->worldmodle_other_vehicle_data_.getData()[key].setData(worldmodel_vehicle_data); 
        }
    }
    worldmodel_other_vehicles_data_.vehicle_num = 0;
    worldmodel_other_vehicles_data_.vehicles.clear();
    if (DataContainer::GetInstance()->worldmodle_other_vehicle_data_.isUpToDate()) {
        for (auto map_it : DataContainer::GetInstance()->worldmodle_other_vehicle_data_.getData()) {    
            WorldModelObject worldmodel_vehicle_data = map_it.second.getData();
            worldmodel_other_vehicles_data_.vehicles.push_back(worldmodel_vehicle_data);
        }
    }  
    worldmodel_other_vehicles_data_.vehicle_num = worldmodel_other_vehicles_data_.vehicles.size();
    return worldmodel_other_vehicles_data_;
}

//
// save othervehicle trajectoty info to worldmodel info
//
 void Handler::TransV2xInfoToWorldmodelInfo(const VehicleData &v2x_vehicle_data, 
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
        std::cout << worldmodel_vehicle_data.vehicle_id << " vehicle's history trajectory is all behind ego" << std::endl;
        worldmodel_vehicle_data.frenet_lat_distance = 1.0e10; //invalid
        worldmodel_vehicle_data.frenet_lon_distance = 1.0e10; //invalid
    }   
}

//
//function:erase other vehicle trajectory points behind ego vehicle
//

void Handler::ProcessTrajectory(std::vector<Location> &trajectory) {
    //Update relative info
    if(DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
        const  VehicleGpsData ego_vehicle_location = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
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

} // namespace communication

} // namespace platoon
