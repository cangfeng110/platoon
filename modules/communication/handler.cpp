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

    // /remote_sockaddr_.sin_family = AF_INET;
    // remote_sockaddr_.sin_port = htons(15803);  //15803 ibox  10001 v2x
    // remote_sockaddr_.sin_addr.s_addr = inet_addr("10.42.0.30");  //change 50->30

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
//function: receive remote vehicle info from ibox
//
//
int Handler::DecodeV2xVechileInfo() {

    bzero(buffer_, MAX_RECV_LENGTH);

    int len = ::recvfrom(sockfd_, buffer_, MAX_RECV_LENGTH, 0, NULL, NULL);

    if(len <= 75) {
        LDEBUG << "v2x receive error.";
        return -1;
    }

    LINFO << "receive length is : " << len;

    outbound_communication_header outbound_header;
    int header_len = 36;
    memcpy(&outbound_header, buffer_, header_len);
    // recoder opcode
    // std::ofstream opcode_out;
    // opcode_out.open("./opcode_id_out.txt");
    // opcode_out << outbound_header.op_code << std::endl;
    // opcode_out.close();

    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 3 
        || outbound_header.op_type != 2 || outbound_header.op_code != 1) {
        LDEBUG << "header type error!";
        printf("%x\n\n",outbound_header.op_code);
        std::cout << outbound_header.op_code << std::endl;
        return -1;
    } else {
        LINFO << "header type right!";

        char * buffer_temp;
        buffer_temp = &buffer_[header_len]; // skip outbound header

        uint8_t other_vehicle_num = (*reinterpret_cast<uint8_t *>(buffer_temp + 40));
        printf("remote num=%d\n", other_vehicle_num);

        remote_info other_vehicle_data; //

    if (len < 36 + 44 + other_vehicle_num * 72) {
        LDEBUG << "packet length error!";
        return -1;
    } else {
        LINFO << "packet length right";

        buffer_temp += 44;

        for(int i=0; i < other_vehicle_num; i++) { 
            memcpy(&other_vehicle_data, buffer_temp, 72);
            
            VehicleData v2x_other_vehicle_data;

            //assign data
            v2x_other_vehicle_data.header.nTimeStamp = 1000 * other_vehicle_data.tv_millisec 
                                                        + 1000000 * other_vehicle_data.tv_sec;
            v2x_other_vehicle_data.iVehicleID = other_vehicle_data.remote_id;
            v2x_other_vehicle_data.dLatitude = (double)other_vehicle_data.lat / ACCURACY_7;
            v2x_other_vehicle_data.dLongitude = (double)other_vehicle_data.lon / ACCURACY_7;
            v2x_other_vehicle_data.fHeading = (float)other_vehicle_data.heading * 2 * PI /3600;
            v2x_other_vehicle_data.fSpeed = (float)other_vehicle_data.speed / 1000;  
            v2x_other_vehicle_data.fYawRate = (float)other_vehicle_data.yaw_rate / (100 * 360) * 2 * PI;
            v2x_other_vehicle_data.iShiftPosition = (int8_t)other_vehicle_data.transmisn_state;
            v2x_other_vehicle_data.fBrakePedalAngle = 0;
            v2x_other_vehicle_data.fSteeringAngle = (float)other_vehicle_data.SteeringWheel * 1.5;  
            v2x_other_vehicle_data.fLateralAcc = (float)other_vehicle_data.acc_y / 100.0;
            v2x_other_vehicle_data.fLongituAcc = (float)other_vehicle_data.acc_x /  100.0;
            //v2x_other_vehicle_data.fVehicleLength = (float)other_vehicle_data.length/ 100.0;
            v2x_other_vehicle_data.fVehicleLength = (float)other_vehicle_data.distance / 10.0;
            v2x_other_vehicle_data.fVehicleWidth = (float)other_vehicle_data.width / 100.0;
            // std::cout << "+++++++++++display raw data++++++++++++" << std::endl;
            // std::cout <<"distance" << other_vehicle_data.distance << std::endl;
            // std::cout << "iVehicleID       : " << v2x_other_vehicle_data.iVehicleID    << std::endl
            //               << "fVehicleLength   : " << v2x_other_vehicle_data.fVehicleLength << std::endl
            //               << "fVehicleWidth    : " << v2x_other_vehicle_data.fVehicleWidth << std::endl
            //               << "iDriveModeStatus : " << (int)v2x_other_vehicle_data.iDriveModeStatus << std::endl
            //               << "fBrakePedalAngle : " << v2x_other_vehicle_data.fBrakePedalAngle    << std::endl
            //               << "dLongitude       : " << v2x_other_vehicle_data.dLongitude          << std::endl
            //               << "dLatitude        : " << v2x_other_vehicle_data.dLatitude           << std::endl
            //               << "fLongituAcc      : " << v2x_other_vehicle_data.fLongituAcc         << std::endl
            //               << "fLateralAcc      : " << v2x_other_vehicle_data.fLateralAcc         << std::endl
            //               << "fHeading         : " << v2x_other_vehicle_data.fHeading            << std::endl
            //               << "fSpeed           : " << v2x_other_vehicle_data.fSpeed              << std::endl
            //               << "fSteeringAngle   : " << v2x_other_vehicle_data.fSteeringAngle      << std::endl
            //               << "fYawRate         : " << v2x_other_vehicle_data.fYawRate            << std::endl
            //               << "iGpsState        : " << (int)v2x_other_vehicle_data.iGpsState      << std::endl
            //               << "iGpsTime         : " << v2x_other_vehicle_data.iGpsTime            << std::endl
            //               << "iShiftPosition   : " << (int)v2x_other_vehicle_data.iShiftPosition << std::endl;

            // tramsfrom GPS coordination to ego vehicle coordination
            // if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
            //     const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
            //     platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.dRelativeX, v2x_other_vehicle_data.dRelativeY,
            //                                                         ego_vehicle_gps_data.fHeading,
            //                                                         ego_vehicle_gps_data.fLongitude,ego_vehicle_gps_data.fLatitude,
            //                                                         ego_vehicle_gps_data.fAltitude,
            //                                                         v2x_other_vehicle_data.dLongitude, v2x_other_vehicle_data.dLatitude,
            //                                                         v2x_other_vehicle_data.fAltitude);
            //     platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.dRelativeHeading, 
            //                                                             ego_vehicle_gps_data.fHeading, v2x_other_vehicle_data.fHeading);
            //     std::cout<<"ego vehicle heading:" << ego_vehicle_gps_data.fHeading << std::endl;
            //     std::cout<<"v2x other vehicle heading: "<< v2x_other_vehicle_data.fHeading << std::endl;
            // } else {
            //     v2x_other_vehicle_data.dRelativeX = 0.0;
            //     v2x_other_vehicle_data.dRelativeY = 0.0;
            // }
            DataContainer::GetInstance()->v2x_other_vehicle_data_.setData(v2x_other_vehicle_data.iVehicleID, v2x_other_vehicle_data);
            buffer_temp += 72;   //locate next vehicle info
        }
        return 1;
        }
    }
}
//
//function: send ego vehicle gps info to ibox
//


int Handler::BroastEgoVehicleGpsInfo() {
    // read gps data
    const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();

    //init header and data
    int header_len = 24;
    inbound_communication_header send_header;
    int data_len = 80;
    inbound_gnss_data send_data;
    std::string send_ip = "192.168.1.18";
    int send_port = 20001;

    //send_header
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 3;
    send_header.op_type = 2;
    send_header.op_code = 104;
    send_header.op_sn = 0;
    send_header.msg_len = data_len;

    //send_data
    send_data.lat = (int32_t)(ego_vehicle_gps_data.fLatitude * ACCURACY_7);
    send_data.lon = (int32_t)(ego_vehicle_gps_data.fLongitude * ACCURACY_7);
    send_data.heading = (uint16_t)(ego_vehicle_gps_data.fHeading * 180 * 10 / PI);
    send_data.alt = (int32_t)(ego_vehicle_gps_data.fAltitude * 100);

    //timestamp
    send_data.timestamp_seconds = (uint32_t)(ego_vehicle_gps_data.header.nTimeStamp / 1000000);
    send_data.timestamp_miliseconds = (uint32_t)(ego_vehicle_gps_data.header.nTimeStamp / 1000 - send_data.timestamp_seconds * 1000);
   
   //display
    std::cout << "ego vehicle longitude is : " << ego_vehicle_gps_data.fLongitude << std::endl;
    std::cout << "ego_vehicle latitude is : " << ego_vehicle_gps_data.fLatitude << std::endl;
    std::cout << "ego vehilce altitude is :" << ego_vehicle_gps_data.fAltitude << std::endl;
    std::cout << "ego vehicle heading is : " << ego_vehicle_gps_data.fHeading << std::endl;
    
    // udp send
    Udp sudp(send_ip,send_port);
    sudp.init();
    char * buffer = new char[header_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &send_data, data_len);
    sudp.send(buffer, header_len + data_len);
    delete []buffer;
    LINFO << "broast ego vehicle gps info over";
}
//
//function: send ego vehicle vcu info to ibox
//

int Handler::BroastEgoVehicleVcuInfo() {
    // receive vcu data
    const VehicleVcuData &ego_vehicle_vcu_data = DataContainer::GetInstance()->ego_vehicle_vcu_data_.getData();

    //assign send header/send data
    int header_len = 24;
    inbound_communication_header send_header;
    int data_len = 64;
    inbound_can_data send_data;

    // assign udp ip / port
    std::string send_ip = "192.168.1.18";
    int send_port = 20001;

    //assign send_header 
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 3;
    send_header.op_type = 2;
    send_header.op_code = 200; // 
    send_header.op_sn = 0;
    send_header.msg_len = data_len; //
    

    //assignment
    send_data.speed = (uint16_t)(ego_vehicle_vcu_data.fSpeed * 1000);
    send_data.steering_wheel_angle = (int16_t)(ego_vehicle_vcu_data.fSteeringAngle * 3600 / (2 * PI));
    //send_data.gyro_z = (int32_t)(ego_vehicle_vcu_data.fYawRate * 1000);
    send_data.longitudinal_acceleration = (int32_t)(ego_vehicle_vcu_data.fLongituAccel * 1000);
    //default
    send_data.rear_wheel_tick = 65535;
    send_data.transmisn = 7;
    send_data.light = 65535;
    send_data.ESP_status = 0;
    send_data.TCS_status = 0;
    send_data.ABS_status = 0;
    send_data.LDW_status = 0;
    send_data.brake_pedal = 0;
    send_data.pedal_status = 101;
    send_data.lose_contorl = 0;
    send_data.tire_pressure = 0;
    //display
    std::cout << "ego vehicle speed is : " << ego_vehicle_vcu_data.fSpeed << std::endl;
    std::cout << "ego_vehicle steering wheel angle is : " << ego_vehicle_vcu_data.fSteeringAngle * 360 / (2 * PI) << std::endl;
    std::cout << "ego vehicle longtitude acc is : " << ego_vehicle_vcu_data.fLongituAccel << std::endl;
    //std::cout << "ego vehilce yaw rate is :" << ego_vehicle_vcu_data.fYawRate << std::endl;

    // udp send
    Udp sudp(send_ip,send_port);
    sudp.init();
    char * buffer = new char[header_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &send_data, data_len);
    sudp.send(buffer, header_len + data_len);
    delete []buffer;
    LINFO << "broast ego vehicle vuc info over";
}
//
//function: get other vehicles info in ego vehicle coordination 
//include frenet diatance
//
WorldModelObjects & Handler::GetWorldmodleVehiles() {
    if (DataContainer::GetInstance()->v2x_other_vehicle_data_.isUpToDate()) {
        for(auto map_it : DataContainer::GetInstance()->v2x_other_vehicle_data_.getData()) {
            const VehicleData v2x_other_vehicle_data = map_it.second.getData();
            const int key = v2x_other_vehicle_data.iVehicleID;
            WorldModelObject worldmodel_vehicle_data =
                DataContainer::GetInstance()->worldmodle_other_vehicle_data_.getData()[key].getData();
            TransV2xInfoToWorldmodelInfo(v2x_other_vehicle_data, worldmodel_vehicle_data);
            DataContainer::GetInstance()->worldmodle_other_vehicle_data_.getData()[key].setData(worldmodel_vehicle_data); 
        }
    }
    worldmodel_other_vehicles_data_.nVehicleNum = 0;
    worldmodel_other_vehicles_data_.vehicles.clear();
    for (auto map_it : DataContainer::GetInstance()->worldmodle_other_vehicle_data_.getData()) {
        WorldModelObject worldmodel_vehicle_data = map_it.second.getData();
        worldmodel_other_vehicles_data_.vehicles.push_back(worldmodel_vehicle_data);
    }
    worldmodel_other_vehicles_data_.nVehicleNum = worldmodel_other_vehicles_data_.vehicles.size();
    return worldmodel_other_vehicles_data_;
}

//
// save othervehicle trajectoty info to worldmodel info
//
 void Handler::TransV2xInfoToWorldmodelInfo(const VehicleData &v2x_vehicle_data, 
                                        WorldModelObject &worldmodel_vehicle_data) {
    Location temp;
    temp.timestamp = v2x_vehicle_data.header.nTimeStamp;
    temp.speed = v2x_vehicle_data.fSpeed;
    temp.wheelAngle = v2x_vehicle_data.fSteeringAngle;
    temp.Longitude = v2x_vehicle_data.dLongitude;
    temp.Latitude = v2x_vehicle_data.dLatitude;
    temp.Altitude = v2x_vehicle_data.fAltitude;
    temp.acceleration = v2x_vehicle_data.fLongituAcc;
    temp.heading = v2x_vehicle_data.fHeading;
    temp.relative_heading = v2x_vehicle_data.dRelativeHeading;
    temp.relative_x = v2x_vehicle_data.dRelativeX;
    temp.relative_y = v2x_vehicle_data.dRelativeY;

    int long_error = fabs(worldmodel_vehicle_data.hisTrajectory.back().Longitude - temp.Longitude);
    int lat_error = fabs(worldmodel_vehicle_data.hisTrajectory.back().Latitude - temp.Latitude);
    
    // insert new location info if other vehicle speed is not zero 
    // and new location is far from the previous location
    if(worldmodel_vehicle_data.hisTrajectory.size() == 0)
        worldmodel_vehicle_data.hisTrajectory.push_back(temp);
    else if ((temp.speed > 0.001) && (long_error > 0.0000001 || lat_error > 0.0000001))
        worldmodel_vehicle_data.hisTrajectory.push_back(temp);
    else 
        worldmodel_vehicle_data.hisTrajectory.back() = temp;

    ProcessTrajectory(worldmodel_vehicle_data.hisTrajectory);
    worldmodel_vehicle_data.pointNum = worldmodel_vehicle_data.hisTrajectory.size();

    Location nearest_point = worldmodel_vehicle_data.hisTrajectory.front();
    worldmodel_vehicle_data.frenet_lat_distance = nearest_point.relative_y;

    double dis_temp = 0;
    for (int i = 0; i < worldmodel_vehicle_data.hisTrajectory.size() - 1; i++) {
        double x = worldmodel_vehicle_data.hisTrajectory[i].relative_x - 
                    worldmodel_vehicle_data.hisTrajectory[i + 1].relative_x;
        double y = worldmodel_vehicle_data.hisTrajectory[i].relative_y -
                    worldmodel_vehicle_data.hisTrajectory[i + 1].relative_y;
        dis_temp += sqrt(x * x + y * y);
    }
    dis_temp += sqrt(nearest_point.relative_x * nearest_point.relative_x + 
                     nearest_point.relative_y * nearest_point.relative_y);
    dis_temp -= 17; //subtract vehicle length
    worldmodel_vehicle_data.frenet_lon_distance = dis_temp;
}

//
//function:erase other vehicle trajectory points behind ego vehicle
//

void Handler::ProcessTrajectory(std::vector<Location> &trajectory) {
    const  VehicleGpsData ego_vehicle_location = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
    //double min_dis = 1000;
    int index_near = -1;
    for (int i = 0; i < trajectory.size(); i++) {
        //Update relative info
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(trajectory[i].relative_x, trajectory[i].relative_y,
                                                            ego_vehicle_location.fHeading,
                                                            ego_vehicle_location.fLongitude, ego_vehicle_location.fLatitude,
                                                            ego_vehicle_location.fAltitude,
                                                            trajectory[i].Longitude, trajectory[i].Latitude,
                                                            trajectory[i].Altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(trajectory[i].relative_heading, 
                                                            ego_vehicle_location.fHeading, trajectory[i].heading);
    }
    // find the first point infront of ego vehicle
    for (int i = 0; i < trajectory.size();i++) {
        if(trajectory[i].relative_x > 0) {
            index_near = i;
            break;
        }        
    }
    if (index_near != -1) 
        trajectory.erase(trajectory.begin(), trajectory.begin() + index_near);
}

} // namespace communication

} // namespace platoon
