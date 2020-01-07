#include "modules/communication/handler.h"

#include <assert.h>
#include <iostream>
#include <string.h>

#include "include/base/Timestamp.h"
#include "include/base/Logging.h"

#include "modules/common/functiontool.h"
#include "modules/communication/inbound_gnss_data.h"
#include "modules/communication/inbound_communication_header.h"
#include "modules/communication/udp.h"

namespace platoon {

namespace communication {

#define ACCURACY_SMALL 1E10
#define ACCURACY_LARGE 1E7

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
            v2x_other_vehicle_data.header.nTimeStamp = 1000*other_vehicle_data.tv_millisec 
                                                        + 1000000*other_vehicle_data.tv_sec;
            v2x_other_vehicle_data.iVehicleID = other_vehicle_data.remote_id;
            v2x_other_vehicle_data.dLatitude = (double)other_vehicle_data.lat / ACCURACY_LARGE;
            v2x_other_vehicle_data.dLongitude = (double)other_vehicle_data.lon / ACCURACY_LARGE;
            v2x_other_vehicle_data.fHeading = (float)other_vehicle_data.heading * 6.2831852 /3600;
            v2x_other_vehicle_data.fSpeed = (float)other_vehicle_data.speed / 1000;  //current unit:1m/s
            v2x_other_vehicle_data.fYawRate = (float)other_vehicle_data.yaw_rate ;
            v2x_other_vehicle_data.iShiftPosition = (int8_t)other_vehicle_data.transmisn_state;
            v2x_other_vehicle_data.fBrakePedalAngle = 0;
            v2x_other_vehicle_data.fSteeringAngle = (float)other_vehicle_data.SteeringWheel;  //???
            v2x_other_vehicle_data.fLateralAcc = (float)other_vehicle_data.acc_y / 100.0;
            v2x_other_vehicle_data.fLongituAcc = (float)other_vehicle_data.acc_x /  100.0;
            v2x_other_vehicle_data.fVehicleLength = (float)other_vehicle_data.length/ 100.0;
            v2x_other_vehicle_data.fVehicleWidth = (float)other_vehicle_data.width / 100.0;
            if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) {
                const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
                platoon::common::
               
                std::cout<<"egoHeading:"<<egoVehData.fHeading<<std::endl;
                std::cout<<"v2xHeading:"<<v2x_other_vehicle_data.fHeading<<std::endl;
            }
            else
            {
                v2x_other_vehicle_data.dRelativeX = 0.0;
                v2x_other_vehicle_data.dRelativeY = 0.0;
            }
            dataContainer::getInstance()->_v2xOtherVehicleData.setData(v2x_other_vehicle_data.iVehicleID,v2x_other_vehicle_data);
            buffer_temp += 72;   //locate next vehicle info
        }
        return 1;
        }
    }
}

int handler::bsmEncodeAndBroadCast() {
//TEST============================================================================================
    const egoVehicleData &vehicleData = dataContainer::getInstance()->_egoVehicleData.getData();
    inbound_communication_header send_header;
    inbound_gnss_data send_data;
    std::string send_ip = "192.168.1.18";
    int send_port = 20001;
    //send_header
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 3;
    send_header.op_type = 2;
    send_header.op_code = 104;
    send_header.op_sn = 0;
    send_header.msg_len = 80;
    //send_data
    send_data.lat = (int32_t)(vehicleData.dLatitude * 10000000);
    send_data.lon = (int32_t)(vehicleData.dLongitude * 10000000);
    send_data.heading = (uint16_t)(vehicleData.fHeading * 180 * 10 / 6.2831852);
    send_data.alt = (int32_t)(vehicleData.fAltitude);
    //timestamp
    send_data.timestamp_seconds = (uint32_t)(vehicleData.header.nTimeStamp / 1000000);
    send_data.timestamp_miliseconds = (uint32_t)(vehicleData.header.nTimeStamp / 1000 - send_data.timestamp_seconds * 1000);
    printf("%I64u\n%ld\n%ld\n",vehicleData.header.nTimeStamp,send_data.timestamp_seconds,send_data.timestamp_miliseconds);
    printf("egoHeading:%f\n",vehicleData.fAltitude);
    Udp sudp(send_ip,send_port);
    sudp.init();
    char * buffer = new char[104];
    memcpy(buffer,&send_header,24);
    memcpy(buffer + 24,&send_data,80);
    sudp.send(buffer,104);
    printf("send Gnss information to hmi!\n");
    printf("******************************send Gnss Info over!******************************\n\n\n\n\n");
    delete []buffer;

}

} // namespace communication

} // namespace platoon
