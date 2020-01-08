#include "modules/communication/handler.h"

#include <assert.h>
#include <iostream>
#include <string.h>

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

    remote_sockaddr_.sin_family = AF_INET;
    remote_sockaddr_.sin_port = htons(15803);  //15803 ibox  10001 v2x
    remote_sockaddr_.sin_addr.s_addr = inet_addr("10.42.0.30");  //change 50->30

    local_sockaddr_.sin_family = AF_INET;
    local_sockaddr_.sin_port = htons(15803);  //15803 ibox  10001 v2x
    local_sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);


    if(::bind(sockfd_, (struct sockaddr *) & local_sockaddr_,
              sizeof(local_sockaddr_)) < 0){
        LDIE << "local port bind error.";
    } else {
        LINFO << "local port bind success.";
    }
}

int Handler::DecodeV2xVechileInfo() {

    bzero(buffer_, MAX_RECV_LENGTH);

    int len = ::recvfrom(sockfd_, buffer_, MAX_RECV_LENGTH, 0, NULL, NULL);

    if(len <= 75) {
        LDEBUG << "v2x receive error.";
        return -1;
    }

    LDEBUG << "Receive, len : " << len;

    outbound_communication_header outbound_header;
   
    memcpy(&outbound_header, buffer_, 36);
   
    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 3 
        || outbound_header.op_type != 2 || outbound_header.op_code != 1) {
        LDEBUG << "packet type error!";
        return -1;
    } else {
        LDEBUG << "packet type right!";

        char * buffer_temp;
        buffer_temp = &buffer_[36]; // skip outbound header

        uint8_t other_vehicle_num = (*reinterpret_cast<uint8_t *>(buffer_temp + 40));
        printf("remote num=%d\n", other_vehicle_num);

        remote_info other_vehicle_data; //

    if (len < 36 + 44 + other_vehicle_num * 72) {
        LDEBUG << "packet length error!";
        return -1;
    } else {
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
            v2x_other_vehicle_data.fVehicleLength = (float)other_vehicle_data.length/ 100.0;
            v2x_other_vehicle_data.fVehicleWidth = (float)other_vehicle_data.width / 100.0;

            // tramsfrom GPS coordination to ego vehicle coordination
            if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
                const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
                platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.dRelativeX, v2x_other_vehicle_data.dRelativeY,
                                                                    ego_vehicle_gps_data.fHeading,
                                                                    ego_vehicle_gps_data.fLongitude,ego_vehicle_gps_data.fLatitude,
                                                                    ego_vehicle_gps_data.fAltitude,
                                                                    v2x_other_vehicle_data.dLongitude, v2x_other_vehicle_data.dLatitude,
                                                                    v2x_other_vehicle_data.fAltitude);
                platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.dRelativeHeading, 
                                                                        ego_vehicle_gps_data.fHeading, v2x_other_vehicle_data.fHeading);
                std::cout<<"ego vehicle heading:" <<ego_vehicle_gps_data.fHeading<<std::endl;
                std::cout<<"v2x other vehicle heading: "<<v2x_other_vehicle_data.fHeading<<std::endl;
            } else {
                v2x_other_vehicle_data.dRelativeX = 0.0;
                v2x_other_vehicle_data.dRelativeY = 0.0;
            }
            DataContainer::GetInstance()->v2x_other_vehicle_data_.setData(v2x_other_vehicle_data.iVehicleID, v2x_other_vehicle_data);
            buffer_temp += 72;   //locate next vehicle info
        }
        return 1;
        }
    }
}

int Handler::BroastEgoVehicleGpsInfo() {
    // read gps data
    const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();

    //init header and data
    int header_len = 36;
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
    printf("%I64u\n%ld\n%ld\n",ego_vehicle_gps_data.header.nTimeStamp, send_data.timestamp_seconds, send_data.timestamp_miliseconds);
    printf("ego vehicle heading:%f\n",ego_vehicle_gps_data.fAltitude);

    // udp send
    Udp sudp(send_ip,send_port);
    sudp.init();
    char * buffer = new char[header_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &send_data, data_len);
    sudp.send(buffer, header_len + data_len);
    printf("send ego vehicle gps information to ibox!\n");
    printf("******************************send ego vehicle gps information over!******************************\n\n\n\n\n");
    delete []buffer;
}

int Handler::BroastEgoVehicleVcuInfo() {
    // receive vcu data
    const VehicleVcuData &ego_vehicle_vcu_data = DataContainer::GetInstance()->ego_vehicle_vcu_data_.getData();

    //assign send header/send data
    int header_len = 36;
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
    send_data.steering_wheel_angle = (int16_t)(ego_vehicle_vcu_data.fSteeringAngle * 10);
    send_data.gyro_z = (int32_t)(ego_vehicle_vcu_data.fYawRate * 1000);
    send_data.acc_x = (int32_t)(ego_vehicle_vcu_data.fLongituAccel * 1000);


    //display
    std::cout << "ego vehicle speed is : " << ego_vehicle_vcu_data.fSpeed << std::endl;
    std::cout << "ego_vehicle steering wheel angle is : " << ego_vehicle_vcu_data.fSteeringAngle << std::endl;
    std::cout << "ego vehicle longtitude acc is : " << ego_vehicle_vcu_data.fLongituAccel << std::endl;
    std::cout << "ego vehilce yaw rate is :" << ego_vehicle_vcu_data.fYawRate << std::endl;

    // udp send
    Udp sudp(send_ip,send_port);
    sudp.init();
    char * buffer = new char[header_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &send_data, data_len);
    sudp.send(buffer, header_len + data_len);
    printf("send ego vehicle vcu information to ibox!\n");
    printf("******************************send ego vehicle vcu information over!******************************\n\n\n\n\n");
    delete []buffer;
}

} // namespace communication

} // namespace platoon
