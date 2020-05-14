#include "modules/communication/receiveudp.h"
#include "modules/customfunction/functiontool.h"
#include "modules/communication/datadefine.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/highfredatacontainer.h"
#include "modules/communication/udpdatacontainer.h"
#include "modules/communication/senddatacontanier.h"
#include "modules/communication/configdata.h"

#include "include/ibox/UDPVehicle.hpp"
#include "include/ibox/inbound_communication_header.h"
#include "include/ibox/outbound_communication_header.h"



namespace platoon
{
namespace communication
{
ReceiveUDP::ReceiveUDP() : lcm_("udpm://239.255.76.67:7667?ttl=1"), loop_("ReceiveUDP"), platoon_number_(0), isupdate_(false)
{
    if (!lcm_.good())
    {
        LDIE << " udp lcm init error! ";
    }
    /**
     * init socket 
    */
    if((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        LDIE << "v2x socket create failed.";
    }

    int port = ConfigData::GetInstance()->local_port_;
    v2x_sockaddr_.sin_family = AF_INET;
    v2x_sockaddr_.sin_port = htons(port);  
    v2x_sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if(::bind(sockfd_, (struct sockaddr *) & v2x_sockaddr_,
              sizeof(v2x_sockaddr_)) < 0)
    {
        LDIE << "local port bind error.";
    } 
    else 
    {
        LINFO << "local port bind success.";
    }
    // init v2x channel 
    v2x_channel_.reset(new base::Channel(&loop_, sockfd_, "V2X_UDP"));

    // chose version
    if (ConfigData::GetInstance()->test_with_log_)
    {
        lcm_.subscribe("V2X_OTHER_VEHICLE_INFO", &ReceiveUDP::HandleLogV2xInfo, this);
        lcm_channel_.reset(new base::Channel(&loop_, lcm_.getFileno(), "LogTest"));
        lcm_channel_->setReadCallback(std::bind(&lcm::LCM::handle, &lcm_));
        lcm_channel_->enableReading();
    }
    else if (ConfigData::GetInstance()->is_sil_test_)
    {
        v2x_channel_->setReadCallback(std::bind(&ReceiveUDP::SilDecodeV2xVechileInfo, this));
    }
    else if (ConfigData::GetInstance()->is_protocol_2_)
    {
        v2x_channel_->setReadCallback(std::bind(&ReceiveUDP::DecodeV2xVehicleInfo2, this));
    }
    else 
    {
        v2x_channel_->setReadCallback(std::bind(&ReceiveUDP::DecodeV2xVechileInfo3, this));
    }

    v2x_channel_->enableReading();

    // decrease ttl per 100ms
    loop_.runEvery(100, std::bind(&UDPDataContainer::DecreaseTtl, UDPDataContainer::GetInstance()));    

    debug_flags_ = ConfigData::GetInstance()->GetDebugFlags();
}

ReceiveUDP::~ReceiveUDP()
{
    LINFO << " Receive UDP info is end ";
}

void ReceiveUDP::Loop()
{
    LINFO << " Receive UDP info is start ";
    loop_.loop();
    LINFO << " Receive UDP info is end ";
}

/**
 * 3.0version
*/
int ReceiveUDP::DecodeV2xVechileInfo3() 
{
    bzero(v2x_buffer_, MAX_RECEIVE_LEN);

    int len = ::recvfrom(sockfd_, v2x_buffer_, MAX_RECEIVE_LEN, 0, NULL, NULL);

    if (debug_flags_ & DEBUG_V2xVehicleInfo)
        std::cout << "receive length is : " << len << std::endl;

    outbound_communication_header outbound_header;
    int header_len = sizeof(outbound_header);
    memcpy(&outbound_header, v2x_buffer_, header_len);

    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 6 
        || outbound_header.op_type != 2 || outbound_header.op_code != 98) 
    {
        if (debug_flags_ & DEBUG_V2xVehicleInfo)
            printf("header type error: %x\n\n", outbound_header.op_code);
        return -1;
    } 
    else 
    {
        char* buffer_temp;
        int verify_len = sizeof(Transparent_Transmission_Data_Header);
        buffer_temp = &v2x_buffer_[header_len + verify_len]; // skip outbound header, verify_len

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
        v2x_other_vehicle_data.safedistance = udp_other_vehicle_data.safe_distance;

        int key = v2x_other_vehicle_data.vehicle_id;

        ego_vehicle_gps_data_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(isupdate_);
    
        if (isupdate_) 
        {
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                                ego_vehicle_gps_data_.heading,
                                                                ego_vehicle_gps_data_.longitude,ego_vehicle_gps_data_.latitude,
                                                                ego_vehicle_gps_data_.height,
                                                                v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                v2x_other_vehicle_data.altitude);
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                    ego_vehicle_gps_data_.heading, v2x_other_vehicle_data.heading);
        } 
        else 
        {
            v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
            v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
        }
        UDPDataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

        std::string if_platoon = "No";
        
        /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
        // if (ConfigData::GetInstance()->hmi_fms_valid_)
        // {
        //     platoon_number_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(isupdate_).platoon_number;
        //     //printf("the ego vehicle platoon number is %d\n", ego_platoon_number);
        //     //printf("the other vehicle platoon number is %d\n", v2x_other_vehicle_data.platoon_number);
        //     if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        //     {
        //         if_platoon = "Yes_hmi";
        //         UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        //     }
        // }
        // else if (LowFreDataContanier::GetInstance()->fms_pre_info_.isUpToDate()) 
        // {
        //     platoon_number_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(isupdate_).platoonnumber();
        //     if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        //     {
        //         if_platoon = "Yes_FMS";
        //         UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        //     }
        // }
        platoon_number_ = SendDataContanier::GetInstance()->platoon_number_.getData(isupdate_);
        if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        {
            if_platoon = "Yes, FMS or Hmi";
            UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }

        lcm_.publish("V2X_OTHER_VEHICLE_INFO", &v2x_other_vehicle_data);

        if (debug_flags_ & DEBUG_V2xVehicleInfo) 
        {
            using namespace std;
            cout << "-----------Display other vehicle info--------------" << endl;
            cout << "other vehicle id is : " << key << endl;
            cout << "if a platoon vehicle : " << if_platoon << endl;
            printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
            printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
            printf("other vehicle gps time is : %f\n", v2x_other_vehicle_data.gps_time);
            printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
            printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
            printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
            printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
            printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
            printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
            printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
            printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
        }
        return key;
    }
}
/**
 * 2.0version
*/
int ReceiveUDP::DecodeV2xVehicleInfo2() {

    bzero(v2x_buffer_, MAX_RECEIVE_LEN);

    int len = ::recvfrom(sockfd_, v2x_buffer_, MAX_RECEIVE_LEN, 0, NULL, NULL);

    if (debug_flags_ & DEBUG_V2xVehicleInfo)
        std::cout << "receive length is : " << len << std::endl;

    outbound_communication_header outbound_header;
    int header_len = 36;
    memcpy(&outbound_header, v2x_buffer_, header_len);

    if (outbound_header.proto_id != 0xAFEE2468 || outbound_header.ver != 3 
        || outbound_header.op_type != 2 || outbound_header.op_code != 98) 
    {
        if (debug_flags_ & DEBUG_V2xVehicleInfo)
            printf("header type error: %x\n\n", outbound_header.op_code);
        return -1;
    } 
    else 
    {
        //std::cout << "header type right!" << std::endl;

        char * buffer_temp;
        buffer_temp = &v2x_buffer_[header_len]; // skip outbound header

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
        v2x_other_vehicle_data.safedistance = udp_other_vehicle_data.safe_distance;

        int key = v2x_other_vehicle_data.vehicle_id;
    
        ego_vehicle_gps_data_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(isupdate_);
    
        if (isupdate_) 
        {
            platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                                ego_vehicle_gps_data_.heading,
                                                                ego_vehicle_gps_data_.longitude,ego_vehicle_gps_data_.latitude,
                                                                ego_vehicle_gps_data_.height,
                                                                v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                                v2x_other_vehicle_data.altitude);
            platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                    ego_vehicle_gps_data_.heading, v2x_other_vehicle_data.heading);
        } 
        else 
        {
            v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
            v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
        }
        UDPDataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

        std::string if_platoon = "No";
        
        /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
        // if (ConfigData::GetInstance()->hmi_fms_valid_)
        // {
        //     platoon_number_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(isupdate_).platoon_number;
        //     //printf("the ego vehicle platoon number is %d\n", ego_platoon_number);
        //     //printf("the other vehicle platoon number is %d\n", v2x_other_vehicle_data.platoon_number);
        //     if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        //     {
        //         if_platoon = "Yes_hmi";
        //         UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        //     }
        // }
        // else if (LowFreDataContanier::GetInstance()->fms_pre_info_.isUpToDate()) 
        // {
        //     platoon_number_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(isupdate_).platoonnumber();
        //     if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        //     {
        //         if_platoon = "Yes_FMS";
        //         UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        //     }
        // }
        platoon_number_ = SendDataContanier::GetInstance()->platoon_number_.getData(isupdate_);
        if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        {
            if_platoon = "Yes, FMS or Hmi";
            UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }
        lcm_.publish("V2X_OTHER_VEHICLE_INFO", &v2x_other_vehicle_data);

        if (debug_flags_ & DEBUG_V2xVehicleInfo) 
        {
            using namespace std;
            cout << "-----------Display other vehicle info--------------" << endl;
            cout << "other vehicle id is : " << key << endl;
            cout << "if a platoon vehicle : " << if_platoon << endl;
            printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
            printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
            printf("other vehicle gps time is : %f\n", v2x_other_vehicle_data.gps_time);
            printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
            printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
            printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
            printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
            printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
            printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
            printf ("other vehicle desire acc is : %f\n", v2x_other_vehicle_data.desire_long_acc);
            printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
            printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
        }
        return key;
    }
}
/**
 * sil-version
 **/
int ReceiveUDP::SilDecodeV2xVechileInfo() 
{
    bzero(v2x_buffer_, MAX_RECEIVE_LEN);

    int len = ::recvfrom(sockfd_, v2x_buffer_, MAX_RECEIVE_LEN, 0, NULL, NULL);

    VehicleData v2x_other_vehicle_data;

    UDPVehicle udp_other_vehicle_data;

    int data_len = sizeof(udp_other_vehicle_data);
    memcpy(&udp_other_vehicle_data, v2x_buffer_, data_len);
    //memcpy(&udp_other_vehicle_data, buffer_temp, data_len);
    if (debug_flags_ & DEBUG_V2xVehicleInfo)
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
    v2x_other_vehicle_data.safedistance = udp_other_vehicle_data.safe_distance;

    int key = v2x_other_vehicle_data.vehicle_id;

    if (key < 1)
    {
        std::cout << "ERROR : other vehicle id is < 1"<< std::endl;
        return -1;
    }

    ego_vehicle_gps_data_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(isupdate_);
    
    if (isupdate_) 
    {
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                            ego_vehicle_gps_data_.heading,
                                                            ego_vehicle_gps_data_.longitude,ego_vehicle_gps_data_.latitude,
                                                            ego_vehicle_gps_data_.height,
                                                            v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                            v2x_other_vehicle_data.altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                ego_vehicle_gps_data_.heading, v2x_other_vehicle_data.heading);
    } 
    else 
    {
        v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
        v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
    }
    UDPDataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

    std::string if_platoon = "No";
    
    /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
    // if (ConfigData::GetInstance()->hmi_fms_valid_)
    // {
    //     platoon_number_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(isupdate_).platoon_number;
    //     //printf("the ego vehicle platoon number is %d\n", ego_platoon_number);
    //     //printf("the other vehicle platoon number is %d\n", v2x_other_vehicle_data.platoon_number);
    //     if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
    //     {
    //         if_platoon = "Yes_hmi";
    //         UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
    //     }
    // }
    // else if (LowFreDataContanier::GetInstance()->fms_pre_info_.isUpToDate()) 
    // {
    //     platoon_number_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(isupdate_).platoonnumber();
    //     if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
    //     {
    //         if_platoon = "Yes_FMS";
    //         UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
    //     }
    // }
    platoon_number_ = SendDataContanier::GetInstance()->platoon_number_.getData(isupdate_);
    if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
    {
        if_platoon = "Yes, FMS or Hmi";
        UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
    }
    lcm_.publish("V2X_OTHER_VEHICLE_INFO", &v2x_other_vehicle_data);

    if (debug_flags_ & DEBUG_V2xVehicleInfo) 
    {
        using namespace std;
        cout << "-----------Display other vehicle info--------------" << endl;
        cout << "other vehicle id is : " << key << endl;
        cout << "if a platoon vehicle : " << if_platoon << endl;
        printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
        printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
        printf("other vehicle gps time is : %f\n", v2x_other_vehicle_data.gps_time);
        printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
        printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
        printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
        printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
        printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
        printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
        printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
        printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
    }
    return key;
}
void ReceiveUDP::HandleLogV2xInfo(const lcm::ReceiveBuffer* rbuf, 
                          const std::string& channel,
                          const VehicleData* msg)
{
    assert(channel == "V2X_OTHER_VEHICLE_INFO");

    VehicleData v2x_other_vehicle_data = *msg; 
    int key = v2x_other_vehicle_data.vehicle_id;

    ego_vehicle_gps_data_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(isupdate_);
    if (isupdate_) 
    {
        platoon::common::TransfromGpsAbsoluteToEgoRelaCoord(v2x_other_vehicle_data.relative_x, v2x_other_vehicle_data.relative_y,
                                                            ego_vehicle_gps_data_.heading,
                                                            ego_vehicle_gps_data_.longitude,ego_vehicle_gps_data_.latitude,
                                                            ego_vehicle_gps_data_.height,
                                                            v2x_other_vehicle_data.longitude, v2x_other_vehicle_data.latitude,
                                                            v2x_other_vehicle_data.altitude);
        platoon::common::TransfromGpsAbsoluteToEgoRelaAzimuth(v2x_other_vehicle_data.relative_heading,
                                                                ego_vehicle_gps_data_.heading, v2x_other_vehicle_data.heading);
    } 
    else 
    {
        v2x_other_vehicle_data.relative_x = INVALID_FLOAT;
        v2x_other_vehicle_data.relative_y = INVALID_FLOAT;
    }
    UDPDataContainer::GetInstance()->v2x_other_vehicles_data_.setData(key, v2x_other_vehicle_data);

    std::string if_platoon = "No";
    
    /* storage the platoon number is equal vehicle to platoon_vehicles_dara_*/
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        platoon_number_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(isupdate_).platoon_number;
        //printf("the ego vehicle platoon number is %d\n", ego_platoon_number);
        //printf("the other vehicle platoon number is %d\n", v2x_other_vehicle_data.platoon_number);
        if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        {
            if_platoon = "Yes_hmi";
            UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }
    }
    else if (LowFreDataContanier::GetInstance()->fms_pre_info_.isUpToDate()) 
    {
        platoon_number_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(isupdate_).platoonnumber();
        if (isupdate_ && platoon_number_ > 0 && platoon_number_ == v2x_other_vehicle_data.platoon_number) 
        {
            if_platoon = "Yes_FMS";
            UDPDataContainer::GetInstance()->platoon_vehicles_data_.setData(key, v2x_other_vehicle_data);
        }
    }

    if (debug_flags_ & DEBUG_V2xVehicleInfo) 
    {
        using namespace std;
        cout << "-----------Display other vehicle info--------------" << endl;
        cout << "other vehicle id is : " << key << endl;
        cout << "if a platoon vehicle : " << if_platoon << endl;
        printf("other vehicle platoon number is : %d\n", v2x_other_vehicle_data.platoon_number);
        printf("other vehicle sequence is : %d\n", v2x_other_vehicle_data.vehicle_sequence);
        printf("other vehicle gps time is : %f\n", v2x_other_vehicle_data.gps_time);
        printf ("other vehicle longitude is : %f\n", v2x_other_vehicle_data.longitude);
        printf ("other vehicle latitude is  : %f\n", v2x_other_vehicle_data.latitude);
        printf ("other vehicle altitude is : %f\n", v2x_other_vehicle_data.altitude);
        printf ("other vehicle heading is(rad) : %f\n", v2x_other_vehicle_data.heading);
        printf ("other vehicle speed is(km/h) : %f\n", v2x_other_vehicle_data.speed * 3.6);
        printf ("other vehicel acc is : %f\n", v2x_other_vehicle_data.longtitude_acc);
        printf ("other vehicle relative_x is: %f\n", v2x_other_vehicle_data.relative_x);
        printf ("other vehicle relative_y is: %f\n\n", v2x_other_vehicle_data.relative_y);
    }
}

}
}//namespace platoon