#include "modules/communication/handler.h"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <fstream>

#include "include/base/Timestamp.h"
#include "include/base/Logging.h"

#include "modules/common/functiontool.h"
#include "modules/communication/inbound_communication_header.h"
#include "modules/communication/outbound_communication_header.h"
#include "modules/communication/udp.h"

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
   
    //assign ego vehicle info
    VehicleData ego_vehicle_info;
     
    //  config info
    ego_vehicle_info.header = ego_vehicle_gps_data.header;
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

    if (ego_vehicle_gps_data.weight - 0.701 < 0.00001)
         ego_vehicle_info.gps_status = 2;
    else if (ego_vehicle_gps_data.weight - 0.602 < 0.00001)
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
    return 1;
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

} // namespace communication

} // namespace platoon
