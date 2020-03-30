#include "modules/communication/handler.h"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>

#include "modules/communication/fmsdata.h"
#include "modules/communication/udp.h"
#include "modules/communication/datadefine.h"
#include "modules/customfunction/functiontool.h"

#include "include/base/Timestamp.h"
#include "include/base/Logging.h"
#include "include/ibox/inbound_communication_header.h"
#include "include/ibox/outbound_communication_header.h"
#include "include/ibox/UDPVehicle.hpp"
#include "include/proto/ProtoClassNameyTypeDefine.h"

namespace platoon 
{
namespace communication 
{

int Handler::send_number_ = 0;

Handler::Handler() 
{
    if((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        LDIE << "v2x socket create failed.";
    }

    int port = ConfigData::GetInstance()->local_port_;
    local_sockaddr_.sin_family = AF_INET;
    local_sockaddr_.sin_port = htons(port);  
    local_sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if(::bind(sockfd_, (struct sockaddr *) & local_sockaddr_,
              sizeof(local_sockaddr_)) < 0)
    {
        LDIE << "local port bind error.";
    } 
    else 
    {
        LINFO << "local port bind success.";
        //LINFO << "length" << sizeof(UDPVehicle);
    }
    m_debug_flags = ConfigData::GetInstance ()->GetDebugFlags ();
}
/**
 * 
 * 
*/
int Handler::BroastEgoVehicleInfo() 
{
    // Record number of send
    send_number_++;

    //assign ego vehicle info
    UDPVehicle ego_vehicle_info;
     
    //  config info
    ego_vehicle_info.vehicle_id = ConfigData::GetInstance()->vehicle_id_;
    ego_vehicle_info.vehicle_length = ConfigData::GetInstance()->vehicle_length_;
    ego_vehicle_info.vehicle_width = ConfigData::GetInstance()->vehicle_width_;
    ego_vehicle_info.vehicle_height = ConfigData::GetInstance()->vehicle_height_;
    
    // gps info
    const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
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
    if(DataContainer::GetInstance()->ego_vehicle_vcu_data_.isUpToDate())
    {
        const VehicleVcuData &ego_vehicle_vcu_data = DataContainer::GetInstance()->ego_vehicle_vcu_data_.getData();
        ego_vehicle_info.longtitude_acc = ego_vehicle_vcu_data.fLongituAccel;
        ego_vehicle_info.lateral_acc = ego_vehicle_vcu_data.fLateralAccel;
        ego_vehicle_info.speed = ego_vehicle_vcu_data.fSpeed;
        ego_vehicle_info.steering_wheel_angle = ego_vehicle_vcu_data.fSteeringAngle;
        ego_vehicle_info.yaw_rate = ego_vehicle_vcu_data.fYawRate;
    } 
    else 
    {
        ego_vehicle_info.longtitude_acc = INVALID_FLOAT;
        ego_vehicle_info.lateral_acc = INVALID_FLOAT;
        ego_vehicle_info.speed = INVALID_FLOAT;
        ego_vehicle_info.steering_wheel_angle = INVALID_FLOAT;
        ego_vehicle_info.yaw_rate = INVALID_FLOAT;
    }
    

    //  manager info
    if(DataContainer::GetInstance()->manager_data_.isUpToDate())
    {
        const PlatoonManagerInfo &manager_data = DataContainer::GetInstance()->manager_data_.getData();
        ego_vehicle_info.desire_drive_mode = manager_data.desire_drive_mode;
        ego_vehicle_info.vehicle_sequence = manager_data.vehicle_sequence;
    } 
    else 
    {
        ego_vehicle_info.desire_drive_mode = 7;
        ego_vehicle_info.vehicle_sequence = -1;
    }
   
    // platoon-planning info
    if(DataContainer::GetInstance()->planning_data_.isUpToDate())
    {
        const EgoPlanningMsg &planning_data = DataContainer::GetInstance()->planning_data_.getData();
        ego_vehicle_info.actual_drive_mode = planning_data.actual_drive_mode;
        ego_vehicle_info.cut_in_flag = planning_data.cut_in;
        ego_vehicle_info.desire_long_acc = planning_data.expire_acc;
    } 
    else 
    {
        ego_vehicle_info.actual_drive_mode = 7;
        ego_vehicle_info.cut_in_flag = 2;
        ego_vehicle_info.desire_long_acc = INVALID_FLOAT;
    } 

    //fms info 
    if (FmsData::GetInstance()->fms_pre_info_.isUpToDate()) 
    {
        const FMSPreFormationInfo& fms_pre_info = FmsData::GetInstance()->fms_pre_info_.getData();
        ego_vehicle_info.platoon_number = fms_pre_info.platoonnumber();
    } 
    else 
    {
        ego_vehicle_info.platoon_number = 0; //don't need to be a platoon
    }
        

    int data_len = sizeof(ego_vehicle_info);

    //assign transparrent transmission data header
    Transparent_Transmission_Data_Header verify_header;
    verify_header.ttdID = 0;
    verify_header.dataSN = send_number_;
    verify_header.informationage = 0;
    verify_header.secmark = 0;
    verify_header.discardage = abs(ConfigData::GetInstance()->discard_time_);
    verify_header.maxIOsize = 0;
    int verify_len = sizeof(verify_header);

     //assign inbound header
    inbound_communication_header send_header;
    int header_len = sizeof(send_header);
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 6;
    send_header.op_type = 2;
    send_header.op_code = 98;
    send_header.op_sn = 0;
    send_header.msg_len = data_len + verify_len;

    // udp send
    // assign ip port
    std::string send_ip =  ConfigData::GetInstance()->remote_ip_;
    int send_port =  ConfigData::GetInstance()->remote_port_;
    Udp sudp(send_ip,send_port);
    sudp.init();
    char* buffer = new char[header_len + verify_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &verify_header, verify_len);
    memcpy(buffer + header_len + verify_len, &ego_vehicle_info, data_len);
    sudp.send(buffer, header_len + verify_len + data_len);
    delete []buffer;

    if(m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
        {
            using namespace std;
            cout << "++++Display ego vehicle info++++" << endl;
            printf ("send data length is : %d\n", data_len);
            printf ("ego vehicle platoon number is : %d\n", ego_vehicle_info.platoon_number);
            printf ("ego vehicle sequence is : %d\n", ego_vehicle_info.vehicle_sequence);
            printf ("ego vehicle actual drive mode is : %d\n", ego_vehicle_info.actual_drive_mode);
            printf ("ego vehicle desire drive mode is : %d\n", ego_vehicle_info.desire_drive_mode);
            printf ("ego vehicle cut_in flag is : %d\n", ego_vehicle_info.cut_in_flag);
            printf ("ego vehicle gps_time is : %f\n", ego_vehicle_info.gps_time);
            printf ("ego vehicle longitude is : %f\n", ego_vehicle_info.longitude);
            printf ("ego vehicle latitude is  : %f\n", ego_vehicle_info.latitude);
            printf ("ego vehicle altitude is : %f\n", ego_vehicle_info.altitude);
            printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
            printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
            printf ("ego vehicle speed is(km/h) : %f\n", ego_vehicle_info.speed * 3.6);
            printf ("ego vehicle acc is : %f\n\n", ego_vehicle_info.longtitude_acc);
        }

    return 1;
}
/**
 * 
 * 
*/
int Handler::DecodeV2xVechileInfo() 
{
    bzero(buffer_, MAX_RECV_LENGTH);

    int len = ::recvfrom(sockfd_, buffer_, MAX_RECV_LENGTH, 0, NULL, NULL);

    if (m_debug_flags & DEBUG_V2xVehicleInfo)
        std::cout << "receive length is : " << len << std::endl;

    outbound_communication_header outbound_header;
    int header_len = sizeof(outbound_header);
    memcpy(&outbound_header, buffer_, header_len);

    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 6 
        || outbound_header.op_type != 2 || outbound_header.op_code != 98) 
    {
        if (m_debug_flags & DEBUG_V2xVehicleInfo)
            printf("header type error: %x\n\n", outbound_header.op_code);
        return -1;
    } 
    else 
    {
        char* buffer_temp;
        int verify_len = sizeof(Transparent_Transmission_Data_Header);
        buffer_temp = &buffer_[header_len + verify_len]; // skip outbound header, verify_len

        VehicleData v2x_other_vehicle_data;

        UDPVehicle udp_other_vehicle_data;

        int data_len = sizeof(udp_other_vehicle_data);

        memcpy(&udp_other_vehicle_data, buffer_temp, data_len);

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
        // v3.0 new data
        v2x_other_vehicle_data.platoon_number = udp_other_vehicle_data.platoon_number;
        v2x_other_vehicle_data.vehicle_sequence = udp_other_vehicle_data.vehicle_sequence;

        int key = v2x_other_vehicle_data.vehicle_id;
    
        if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) 
        {
            const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                                ego_vehicle_gps_data.heading,
                                                                ego_vehicle_gps_data.longitude,ego_vehicle_gps_data.latitude,
                                                                ego_vehicle_gps_data.height,
                                                                v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                v2x_other_vehicle_data.altitude);
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                    ego_vehicle_gps_data.heading, v2x_other_vehicle_data.heading);
        } 
        else 
        {
            v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
            v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
        }
        DataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

        std::string if_platoon = "No";
        
        /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
        if (ConfigData::GetInstance()->hmi_fms_valid_)
        {
            if_platoon = "Yes";
            DataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }
        else if (FmsData::GetInstance()->fms_pre_info_.isUpToDate()) 
        {
            int ego_platoon_number = FmsData::GetInstance()->fms_pre_info_.getData().platoonnumber();
            if (ego_platoon_number == v2x_other_vehicle_data.platoon_number) 
            {
                if_platoon = "Yes";
                DataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
            }
        }

        if (m_debug_flags & DEBUG_V2xVehicleInfo) 
        {
            using namespace std;
            cout << "-----------Display other vehicle info--------------" << endl;
            cout << "other vehicle id is : " << key << endl;
            cout << "if a platoon vehicle : " << if_platoon << endl;
            printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
            printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
            printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
            printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
            printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
            printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
            printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
            printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
            printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
            printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
        }
        return 1;
    }
}


//
//function: broast ego vehicle info to ibox
//
int Handler::SilBroastEgoVehicleInfo() 
{
    // Record number of send
    send_number_++;

    //assign ego vehicle info
    UDPVehicle ego_vehicle_info;
     
    //  config info
    ego_vehicle_info.vehicle_id = ConfigData::GetInstance()->vehicle_id_;
    ego_vehicle_info.vehicle_length = ConfigData::GetInstance()->vehicle_length_;
    ego_vehicle_info.vehicle_width = ConfigData::GetInstance()->vehicle_width_;
    ego_vehicle_info.vehicle_height = ConfigData::GetInstance()->vehicle_height_;
    
    // gps info
    const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
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
    if(DataContainer::GetInstance()->ego_vehicle_vcu_data_.isUpToDate())
    {
        const VehicleVcuData &ego_vehicle_vcu_data = DataContainer::GetInstance()->ego_vehicle_vcu_data_.getData();
        ego_vehicle_info.longtitude_acc = ego_vehicle_vcu_data.fLongituAccel;
        ego_vehicle_info.lateral_acc = ego_vehicle_vcu_data.fLateralAccel;
        ego_vehicle_info.speed = ego_vehicle_vcu_data.fSpeed;
        ego_vehicle_info.steering_wheel_angle = ego_vehicle_vcu_data.fSteeringAngle;
        ego_vehicle_info.yaw_rate = ego_vehicle_vcu_data.fYawRate;
    } 
    else 
    {
        ego_vehicle_info.longtitude_acc = INVALID_FLOAT;
        ego_vehicle_info.lateral_acc = INVALID_FLOAT;
        ego_vehicle_info.speed = INVALID_FLOAT;
        ego_vehicle_info.steering_wheel_angle = INVALID_FLOAT;
        ego_vehicle_info.yaw_rate = INVALID_FLOAT;
    }
    

    //  manager info
    if(DataContainer::GetInstance()->manager_data_.isUpToDate())
    {
        const PlatoonManagerInfo &manager_data = DataContainer::GetInstance()->manager_data_.getData();
        ego_vehicle_info.desire_drive_mode = manager_data.desire_drive_mode;
        ego_vehicle_info.vehicle_sequence = manager_data.vehicle_sequence;
    } 
    else 
    {
        ego_vehicle_info.desire_drive_mode = 7;
        ego_vehicle_info.vehicle_sequence = -1;
    }
   
    // platoon-planning info
    if(DataContainer::GetInstance()->planning_data_.isUpToDate())
    {
        const EgoPlanningMsg &planning_data = DataContainer::GetInstance()->planning_data_.getData();
        ego_vehicle_info.actual_drive_mode = planning_data.actual_drive_mode;
        ego_vehicle_info.cut_in_flag = planning_data.cut_in;
        ego_vehicle_info.desire_long_acc = planning_data.expire_acc;
    } 
    else 
    {
        ego_vehicle_info.actual_drive_mode = 7;
        ego_vehicle_info.cut_in_flag = 2;
        ego_vehicle_info.desire_long_acc = INVALID_FLOAT;
    } 

    //fms info 
    if (FmsData::GetInstance()->fms_pre_info_.isUpToDate()) 
    {
        const FMSPreFormationInfo& fms_pre_info = FmsData::GetInstance()->fms_pre_info_.getData();
        ego_vehicle_info.platoon_number = fms_pre_info.platoonnumber();
    } 
    else 
    {
        ego_vehicle_info.platoon_number = 0; //don't need to be a platoon
    }
        

    int data_len = sizeof(ego_vehicle_info);

    /* //assign transparrent transmission data header
    Transparent_Transmission_Data_Header verify_header;
    verify_header.ttdID = 0;
    verify_header.dataSN = send_number_;
    verify_header.informationage = 0;
    verify_header.secmark = 0;
    verify_header.discardage = abs(ConfigData::GetInstance()->discard_time_);
    verify_header.maxIOsize = 0;
    int verify_len = sizeof(verify_header);

     //assign inbound header
    inbound_communication_header send_header;
    int header_len = sizeof(send_header);
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 6;
    send_header.op_type = 2;
    send_header.op_code = 98;
    send_header.op_sn = 0;
    send_header.msg_len = data_len + verify_len;
 */
    // udp send
    // assign ip port
    std::string send_ip =  ConfigData::GetInstance()->remote_ip_;
    int send_port =  ConfigData::GetInstance()->remote_port_;
    Udp sudp(send_ip,send_port);
    sudp.init();
    char* buffer = new char[data_len];
    memcpy(buffer, &ego_vehicle_info, data_len);
    sudp.send(buffer, data_len);
   /*  char* buffer = new char[header_len + verify_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &verify_header, verify_len);
    memcpy(buffer + header_len + verify_len, &ego_vehicle_info, data_len);
    sudp.send(buffer, header_len + verify_len + data_len);
    delete []buffer; */

    if(m_debug_flags & DEBUG_BroadcastEgoVehicleInfo)
    {
        using namespace std;
        cout << "++++Display ego vehicle info++++" << endl;
        printf ("send data length is : %d\n", data_len);
        printf ("ego vehicle platoon number is : %d\n", ego_vehicle_info.platoon_number);
        printf ("ego vehicle sequence is : %d\n", ego_vehicle_info.vehicle_sequence);
        printf ("ego vehicle actual drive mode is : %d\n", ego_vehicle_info.actual_drive_mode);
        printf ("ego vehicle desire drive mode is : %d\n", ego_vehicle_info.desire_drive_mode);
        printf ("ego vehicle cut_in flag is : %d\n", ego_vehicle_info.cut_in_flag);
        printf ("ego vehicle gps_time is : %f\n", ego_vehicle_info.gps_time);
        printf ("ego vehicle longitude is : %f\n", ego_vehicle_info.longitude);
        printf ("ego vehicle latitude is  : %f\n", ego_vehicle_info.latitude);
        printf ("ego vehicle altitude is : %f\n", ego_vehicle_info.altitude);
        printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
        printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
        printf ("ego vehicle speed is(km/h) : %f\n", ego_vehicle_info.speed * 3.6);
        printf ("ego vehicle acc is : %f\n\n", ego_vehicle_info.longtitude_acc);
    }

    return 1;
}
//
//function: receive remote vehicle info from ibox
//
int Handler::SilDecodeV2xVechileInfo() 
{
    bzero(buffer_, MAX_RECV_LENGTH);

    int len = ::recvfrom(sockfd_, buffer_, MAX_RECV_LENGTH, 0, NULL, NULL);

    

    /* outbound_communication_header outbound_header;
    int header_len = sizeof(outbound_header);
    memcpy(&outbound_header, buffer_, header_len);

    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 6 
        || outbound_header.op_type != 2 || outbound_header.op_code != 98) 
    {
        if (m_debug_flags & DEBUG_V2xVehicleInfo)
            printf("header type error: %x\n\n", outbound_header.op_code);
        return -1;
    } 
    else 
    { */
       /*  char* buffer_temp;
        int verify_len = sizeof(Transparent_Transmission_Data_Header);
        buffer_temp = &buffer_[header_len + verify_len]; // skip outbound header, verify_len */

        VehicleData v2x_other_vehicle_data;

        UDPVehicle udp_other_vehicle_data;

        int data_len = sizeof(udp_other_vehicle_data);
        memcpy(&udp_other_vehicle_data, buffer_, data_len);
        //memcpy(&udp_other_vehicle_data, buffer_temp, data_len);
        if (m_debug_flags & DEBUG_V2xVehicleInfo)
        {
            std::cout << "receive length is : " << len << std::endl;
            std::cout << "need length is : " << data_len << std::endl;
        }
            
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
        // v3.0 new data
        v2x_other_vehicle_data.platoon_number = udp_other_vehicle_data.platoon_number;
        v2x_other_vehicle_data.vehicle_sequence = udp_other_vehicle_data.vehicle_sequence;

        int key = v2x_other_vehicle_data.vehicle_id;

        if (key < 1)
        {
            std::cout << "ERROR : other vehicle id is < 1"<< std::endl;
            return -1;
        }

        if (DataContainer::GetInstance()->ego_vehicle_gps_data_.isUpToDate()) 
        {
            const VehicleGpsData &ego_vehicle_gps_data = DataContainer::GetInstance()->ego_vehicle_gps_data_.getData();
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                                ego_vehicle_gps_data.heading,
                                                                ego_vehicle_gps_data.longitude,ego_vehicle_gps_data.latitude,
                                                                ego_vehicle_gps_data.height,
                                                                v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                v2x_other_vehicle_data.altitude);
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                    ego_vehicle_gps_data.heading, v2x_other_vehicle_data.heading);
        } 
        else 
        {
            v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
            v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
        }
        DataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

        std::string if_platoon = "No";
        /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
        if (ConfigData::GetInstance()->hmi_fms_valid_)
        {
            if_platoon = "Yes";
            DataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }
        else if (FmsData::GetInstance()->fms_pre_info_.isUpToDate()) 
        {
            int ego_platoon_number = FmsData::GetInstance()->fms_pre_info_.getData().platoonnumber();
            if (ego_platoon_number > 0 && ego_platoon_number == v2x_other_vehicle_data.platoon_number) 
            {
                if_platoon = "Yes";
                DataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
            }
        }

        if (m_debug_flags & DEBUG_V2xVehicleInfo) 
        {
            using namespace std;
            cout << "-----------Display other vehicle info--------------" << endl;
            cout << "other vehicle id is : " << key << endl;
            cout << "if a platoon vehicle : " << if_platoon << endl;
            printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
            printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
            printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
            printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
            printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
            printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
            printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
            printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
            printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
            printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
        }
        return 1;
    //}
}

} // namespace communication

} // namespace platoon
