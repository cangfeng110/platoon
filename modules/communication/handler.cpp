#include "modules/communication/handler.h"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>

#include "include/base/Timestamp.h"
#include "include/base/Logging.h"

#include "modules/common/functiontool.h"
#include "modules/communication/inbound_communication_header.h"
#include "modules/communication/outbound_communication_header.h"
#include "modules/communication/udp.h"
#include "modules/communication/UDPVehicle.hpp"

namespace platoon {

namespace communication {

#define ACCURACY_10 1E10
#define ACCURACY_7 1E7
#define INVALID_FLOAT 1E10;
const double PI = 3.1415926;

Handler::Handler() {
    if((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        LDIE << "v2x socket create failed.";
    }

    int port = ConfigData::GetInstance()->local_port_;
    local_sockaddr_.sin_family = AF_INET;
    local_sockaddr_.sin_port = htons(port);  
    local_sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    if(::bind(sockfd_, (struct sockaddr *) & local_sockaddr_,
              sizeof(local_sockaddr_)) < 0){
        LDIE << "local port bind error.";
    } else {
        LINFO << "local port bind success.";
    }
    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
}
//
//function: broast ego vehicle info to ibox
//
int Handler::BroastEgoVehicleInfo() {
    // read gps data
    const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
   
    //assign ego vehicle info
    UDPVehicle ego_vehicle_info;
     
    //  config info
    //ego_vehicle_info.header = ego_vehicle_gps_data.header;
    ego_vehicle_info.vehicle_id = ConfigData::GetInstance()->vehicle_id_;
    ego_vehicle_info.vehicle_length = ConfigData::GetInstance()->vehicle_length_;
    ego_vehicle_info.vehicle_width = ConfigData::GetInstance()->vehicle_width_;
    ego_vehicle_info.vehicle_height = ConfigData::GetInstance()->vehicle_height_;
    
    // gps info
    ego_vehicle_info.longitude = ego_vehicle_gps_data.longitude;
    ego_vehicle_info.latitude = ego_vehicle_gps_data.latitude;
    ego_vehicle_info.altitude = ego_vehicle_gps_data.height;
    ego_vehicle_info.heading = ego_vehicle_gps_data.heading;
    ego_vehicle_info.gps_time = ego_vehicle_gps_data.time;

    if (fabs(ego_vehicle_gps_data.weight - 0.701) < 0.00001)
         ego_vehicle_info.gps_status = 2;
    else if (fabs(ego_vehicle_gps_data.weight - 0.602) < 0.00001)
        ego_vehicle_info.gps_status = 1;
    else 
        ego_vehicle_info.gps_status = 0;
    
    ego_vehicle_info.relative_x = INVALID_FLOAT;
    ego_vehicle_info.relative_y = INVALID_FLOAT;
    ego_vehicle_info.relative_heading = INVALID_FLOAT;

    //vcu info
    if(DataContainer::GetInstance()->ego_vehicle_vcu_data_.isUpToDate()){
        const VehicleVcuData &ego_vehicle_vcu_data = DataContainer::GetInstance()->ego_vehicle_vcu_data_.getData();
        ego_vehicle_info.longtitude_acc = ego_vehicle_vcu_data.fLongituAccel;
        ego_vehicle_info.lateral_acc = ego_vehicle_vcu_data.fLateralAccel;
        ego_vehicle_info.speed = ego_vehicle_vcu_data.fSpeed;
        ego_vehicle_info.steering_wheel_angle = ego_vehicle_vcu_data.fSteeringAngle;
        ego_vehicle_info.yaw_rate = ego_vehicle_vcu_data.fYawRate;
    } else {
        ego_vehicle_info.longtitude_acc = INVALID_FLOAT;
        ego_vehicle_info.lateral_acc = INVALID_FLOAT;
        ego_vehicle_info.speed = INVALID_FLOAT;
        ego_vehicle_info.steering_wheel_angle = INVALID_FLOAT;
        ego_vehicle_info.yaw_rate = INVALID_FLOAT;
    }
    

    //  manager info
    if(DataContainer::GetInstance()->manager_data_.isUpToDate()){
        const PlatoonManagerInfo &manager_data = DataContainer::GetInstance()->manager_data_.getData();
        ego_vehicle_info.desire_drive_mode = manager_data.desire_drive_mode;
    } else {
        ego_vehicle_info.desire_drive_mode = 7;
    }
   
    // platoon-planning info
    if(DataContainer::GetInstance()->planning_data_.isUpToDate()){
        const EgoPlanningMsg &planning_data = DataContainer::GetInstance()->planning_data_.getData();
        ego_vehicle_info.actual_drive_mode = planning_data.actual_drive_mode;
        ego_vehicle_info.cut_in_flag = planning_data.cut_in;
        ego_vehicle_info.desire_long_acc = planning_data.expire_acc;
    } else {
        ego_vehicle_info.actual_drive_mode = 7;
        ego_vehicle_info.cut_in_flag = 2;
        ego_vehicle_info.desire_long_acc = INVALID_FLOAT;
    }
    
    // 
    int data_len = sizeof(ego_vehicle_info);

    // assign ip port
    std::string send_ip =  ConfigData::GetInstance()->remote_ip_;
    int send_port =  ConfigData::GetInstance()->remote_port_;

    if(m_debug_flags & DEBUG_BroadcastEgoVehicleInfo){
        using namespace std;
        cout << "++++Display ego vehicle info++++" << endl;
        cout << "ego vehicle longitude is : "<< ego_vehicle_info.longitude << endl;
        cout << "ego vehicle latitude is : " << ego_vehicle_info.latitude << endl;
        cout << "ego vehicle heading is : " << ego_vehicle_info.heading << endl;
        cout << "ego vehicle speed is(km/h) : " << ego_vehicle_info.speed * 3.6 << endl;
        cout << "udp send length is : " << data_len << endl << endl;
    }
    // udp send
    Udp sudp(send_ip,send_port);
    sudp.init();
    char* buffer = new char[data_len];
    memcpy(buffer, &ego_vehicle_info, data_len);
    sudp.send(buffer, data_len);
    delete []buffer;
    return 1;
}
//
//function: receive remote vehicle info from ibox
//
int Handler::DecodeV2xVechileInfo() {

    bzero(buffer_, MAX_RECV_LENGTH);

    int len = ::recvfrom(sockfd_, buffer_, MAX_RECV_LENGTH, 0, NULL, NULL);

    VehicleData v2x_other_vehicle_data;

    UDPVehicle udp_other_vehicle_data;

    int data_len = sizeof(udp_other_vehicle_data);

    memcpy(&udp_other_vehicle_data, buffer_, len);

    v2x_other_vehicle_data.vehicle_id = udp_other_vehicle_data.vehicle_id;
    v2x_other_vehicle_data.vehicle_length = udp_other_vehicle_data.vehicle_length;
    v2x_other_vehicle_data.vehicle_height = udp_other_vehicle_data.vehicle_height;
    v2x_other_vehicle_data.vehicle_width = udp_other_vehicle_data.vehicle_width;
    v2x_other_vehicle_data.desire_drive_mode = udp_other_vehicle_data.desire_drive_mode;
    v2x_other_vehicle_data.actual_drive_mode = udp_other_vehicle_data.actual_drive_mode;
    v2x_other_vehicle_data.cut_in_flag = udp_other_vehicle_data.cut_in_flag;
    v2x_other_vehicle_data.longitude = udp_other_vehicle_data.longitude;
    v2x_other_vehicle_data.latitude = udp_other_vehicle_data.latitude;
    v2x_other_vehicle_data.altitude = udp_other_vehicle_data.altitude;
    v2x_other_vehicle_data.heading = udp_other_vehicle_data.heading;
    v2x_other_vehicle_data.gps_status = udp_other_vehicle_data.gps_status;
    v2x_other_vehicle_data.gps_time = udp_other_vehicle_data.gps_time;
    v2x_other_vehicle_data.relative_x = udp_other_vehicle_data.relative_x;
    v2x_other_vehicle_data.relative_y = udp_other_vehicle_data.relative_y;
    v2x_other_vehicle_data.relative_heading = udp_other_vehicle_data.relative_heading;
    v2x_other_vehicle_data.longtitude_acc = udp_other_vehicle_data.longtitude_acc;
    v2x_other_vehicle_data.lateral_acc = udp_other_vehicle_data.lateral_acc;
    v2x_other_vehicle_data.speed = udp_other_vehicle_data.speed;
    v2x_other_vehicle_data.steering_wheel_angle = udp_other_vehicle_data.steering_wheel_angle;
    v2x_other_vehicle_data.yaw_rate = udp_other_vehicle_data.yaw_rate;
    v2x_other_vehicle_data.desire_long_acc = udp_other_vehicle_data.desire_long_acc;

    int key = v2x_other_vehicle_data.vehicle_id;

    if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
        const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                            ego_vehicle_gps_data.heading,
                                                            ego_vehicle_gps_data.longitude,ego_vehicle_gps_data.latitude,
                                                            ego_vehicle_gps_data.height,
                                                            v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                            v2x_other_vehicle_data.altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                ego_vehicle_gps_data.heading, v2x_other_vehicle_data.heading);
    } else {
        v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
        v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
    }
    if (m_debug_flags & DEBUG_V2xVehicleInfo) {
        using namespace std;
        cout << "-----------Display other vehicle info--------------" << endl;
        cout << "receive length is : " << len << endl;
        cout << "ego vehicle info length : " << data_len << endl;
        cout << "other vehicle id is : " << key << endl;
        cout << "other vehicle longitude is : " << v2x_other_vehicle_data.longitude << endl;
        cout << "other vehicle latitude is : " << v2x_other_vehicle_data.latitude << endl;
        cout << "other vehicle speed is(km/h): " << v2x_other_vehicle_data.speed * 3.6 << endl;
        cout << "other vehicle relative_x is : " << v2x_other_vehicle_data.relative_x << endl << endl;
    }
        DataContainer::GetInstance()->v2x_other_vehicle_data_.setData(key, v2x_other_vehicle_data);

        return 1;
    //}
}

} // namespace communication

} // namespace platoon
