#include "modules/communication/sendpdm.h"

#include "modules/communication/configdata.h"
#include "modules/communication/highfredatacontainer.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/senddatacontanier.h"
#include "include/ibox/UDPVehicle.hpp"
#include "include/ibox/inbound_communication_header.h"



namespace platoon
{
namespace communication
{

int SendPDM::send_number_ = 0;

SendPDM::SendPDM() : platoon_number_(0)
{
    m_debug_flags_ = ConfigData::GetInstance()->GetDebugFlags();
    send_ip_ = ConfigData::GetInstance()->remote_ip_;
    send_port_ = ConfigData::GetInstance()->remote_port_;
    // sudp_ = Udp(send_ip_, send_port_);
    // sudp_.init();
}

void SendPDM::UpdateInfo()
{
    ego_gps_info_ = HighFreDataContainer::GetInstance()->ego_vehicle_gps_data_.getData(gps_isupdate_);
    ego_vcu_info_ = HighFreDataContainer::GetInstance()->ego_vehicle_vcu_data_.getData(vcu_isupdate_);
    planning_info_ = LowFreDataContanier::GetInstance()->planning_data_.getData(plan_isupdate_);
    manager_info_ = SendDataContanier::GetInstance()->manager_data_.getData(manager_isupdate_);
    bool isupdate;
    if (ConfigData::GetInstance()->hmi_fms_valid_)
    {
        platoon_number_ = LowFreDataContanier::GetInstance()->hmi_fms_info_.getData(isupdate).platoon_number;
        if (!isupdate)
            platoon_number_ = 0;
    }
    else
    {
        platoon_number_ = LowFreDataContanier::GetInstance()->fms_pre_info_.getData(isupdate).platoonnumber();
        if (!isupdate)
            platoon_number_ = 0;
    }
}
/**
 * new protocol
*/
int SendPDM::BroastEgoVehicleInfo3() 
{
    UpdateInfo();
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
    if (gps_isupdate_)
    {
        ego_vehicle_info.longitude = ego_gps_info_.longitude;
        ego_vehicle_info.latitude = ego_gps_info_.latitude;
        ego_vehicle_info.altitude = ego_gps_info_.height;
        ego_vehicle_info.heading = ego_gps_info_.heading;
        ego_vehicle_info.gps_time = ego_gps_info_.time;
        // read long acc from fusion info
        ego_vehicle_info.longtitude_acc = ego_gps_info_.acceleration;

        ego_vehicle_info.gps_status = ego_gps_info_.position_valid_flag_for_motorcade;
        
        ego_vehicle_info.relative_x = INVALID_FLOAT;
        ego_vehicle_info.relative_y = INVALID_FLOAT;
        ego_vehicle_info.relative_heading = INVALID_FLOAT;
    }
    else
    {
        return -1;
    }
    
    //vcu info
    if(vcu_isupdate_)
    {
        ego_vehicle_info.lateral_acc = ego_vcu_info_.fLateralAccel;
        ego_vehicle_info.speed = ego_vcu_info_.fSpeed;
        ego_vehicle_info.steering_wheel_angle = ego_vcu_info_.fSteeringAngle;
        ego_vehicle_info.yaw_rate = ego_vcu_info_.fYawRate;
    } 
    else 
    {
        ego_vehicle_info.lateral_acc = INVALID_FLOAT;
        ego_vehicle_info.speed = INVALID_FLOAT;
        ego_vehicle_info.steering_wheel_angle = INVALID_FLOAT;
        ego_vehicle_info.yaw_rate = INVALID_FLOAT;
    }
    
    //  manager info
    if(manager_isupdate_)
    {
        ego_vehicle_info.desire_drive_mode = manager_info_.desire_drive_mode;
        ego_vehicle_info.vehicle_sequence = manager_info_.vehicle_sequence;
    } 
    else 
    {
        ego_vehicle_info.desire_drive_mode = 7;
        ego_vehicle_info.vehicle_sequence = -1;
    }
   
    // platoon-planning info
    if(plan_isupdate_)
    {
        ego_vehicle_info.actual_drive_mode = planning_info_.actual_drive_mode;
        ego_vehicle_info.cut_in_flag = planning_info_.cut_in;
        ego_vehicle_info.desire_long_acc = planning_info_.expire_acc;
    } 
    else 
    {
        ego_vehicle_info.actual_drive_mode = 7;
        ego_vehicle_info.cut_in_flag = 0;
        ego_vehicle_info.desire_long_acc = INVALID_FLOAT;
    } 

    ego_vehicle_info.platoon_number = int8_t(platoon_number_);
   
        
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
    
    char* buffer = new char[header_len + verify_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &verify_header, verify_len);
    memcpy(buffer + header_len + verify_len, &ego_vehicle_info, data_len);
    
    Udp sudp(send_ip_,send_port_);
    sudp.init();
    sudp.send(buffer, header_len + verify_len + data_len);
    delete []buffer;

    if(m_debug_flags_ & DEBUG_BroadcastEgoVehicleInfo)
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
        printf ("ego vehicle gps_status is : %d\n", ego_vehicle_info.gps_status);
        printf ("ego vehicle longitude is : %f\n", ego_vehicle_info.longitude);
        printf ("ego vehicle latitude is  : %f\n", ego_vehicle_info.latitude);
        printf ("ego vehicle altitude is : %f\n", ego_vehicle_info.altitude);
        printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
        printf ("ego vehicle speed is(km/h) : %f\n", ego_vehicle_info.speed * 3.6);
        printf ("ego vehicle acc is : %f\n\n", ego_vehicle_info.longtitude_acc);
    }

    return 1;
}
//
//function: broast ego vehicle info to ibox
//
int SendPDM::SilBroastEgoVehicleInfo() 
{
    UpdateInfo();

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
    if (gps_isupdate_)
    {
        ego_vehicle_info.longitude = ego_gps_info_.longitude;
        ego_vehicle_info.latitude = ego_gps_info_.latitude;
        ego_vehicle_info.altitude = ego_gps_info_.height;
        ego_vehicle_info.heading = ego_gps_info_.heading;
        ego_vehicle_info.gps_time = ego_gps_info_.time;
        // read long acc from fusion info
        ego_vehicle_info.longtitude_acc = ego_gps_info_.acceleration;
        
        ego_vehicle_info.gps_status = ego_gps_info_.position_valid_flag_for_motorcade;

        ego_vehicle_info.relative_x = INVALID_FLOAT;
        ego_vehicle_info.relative_y = INVALID_FLOAT;
        ego_vehicle_info.relative_heading = INVALID_FLOAT;
    }
    else
    {
        return -1;
    }
    //vcu info
    if(vcu_isupdate_)
    {
        ego_vehicle_info.lateral_acc = ego_vcu_info_.fLateralAccel;
        ego_vehicle_info.speed = ego_vcu_info_.fSpeed;
        ego_vehicle_info.steering_wheel_angle = ego_vcu_info_.fSteeringAngle;
        ego_vehicle_info.yaw_rate = ego_vcu_info_.fYawRate;
    } 
    else 
    {
        //ego_vehicle_info.longtitude_acc = INVALID_FLOAT;
        ego_vehicle_info.lateral_acc = INVALID_FLOAT;
        ego_vehicle_info.speed = INVALID_FLOAT;
        ego_vehicle_info.steering_wheel_angle = INVALID_FLOAT;
        ego_vehicle_info.yaw_rate = INVALID_FLOAT;
    }
    //  manager info
    if(manager_isupdate_)
    {
        ego_vehicle_info.desire_drive_mode = manager_info_.desire_drive_mode;
        ego_vehicle_info.vehicle_sequence = manager_info_.vehicle_sequence;
    } 
    else 
    {
        ego_vehicle_info.desire_drive_mode = 7;
        ego_vehicle_info.vehicle_sequence = -1;
    }
   
    // platoon-planning info
    if(plan_isupdate_)
    {
        ego_vehicle_info.actual_drive_mode = planning_info_.actual_drive_mode;
        ego_vehicle_info.cut_in_flag = planning_info_.cut_in;
        ego_vehicle_info.desire_long_acc = planning_info_.expire_acc;
    } 
    else 
    {
        ego_vehicle_info.actual_drive_mode = 7;
        ego_vehicle_info.cut_in_flag = 2;
        ego_vehicle_info.desire_long_acc = INVALID_FLOAT;
    } 

    //fms info 
    ego_vehicle_info.platoon_number = int8_t(platoon_number_);
        
    int data_len = sizeof(ego_vehicle_info);

    char* buffer = new char[data_len];
    memcpy(buffer, &ego_vehicle_info, data_len);
    Udp sudp(send_ip_,send_port_);
    sudp.init();
    sudp.send(buffer, data_len);
   

    if(m_debug_flags_ & DEBUG_BroadcastEgoVehicleInfo)
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
        printf ("ego vehicle gps_status is : %d\n", ego_vehicle_info.gps_status);
        printf ("ego vehicle longitude is : %f\n", ego_vehicle_info.longitude);
        printf ("ego vehicle latitude is  : %f\n", ego_vehicle_info.latitude);
        printf ("ego vehicle altitude is : %f\n", ego_vehicle_info.altitude);
        printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
        printf ("ego vehicle speed is(km/h) : %f\n", ego_vehicle_info.speed * 3.6);
        printf ("ego vehicle acc is : %f\n\n", ego_vehicle_info.longtitude_acc);
    }

    return 1;
}

/**
 * 2.0version
*/
int SendPDM::BroastEgoVehicleInfo2() 
{

    UpdateInfo();

    //assign ego vehicle info

    UDPVehicle ego_vehicle_info;
     
     //  config info
    ego_vehicle_info.vehicle_id = ConfigData::GetInstance()->vehicle_id_;
    ego_vehicle_info.vehicle_length = ConfigData::GetInstance()->vehicle_length_;
    ego_vehicle_info.vehicle_width = ConfigData::GetInstance()->vehicle_width_;
    ego_vehicle_info.vehicle_height = ConfigData::GetInstance()->vehicle_height_;
    
    // gps info
    if (gps_isupdate_)
    {
        ego_vehicle_info.longitude = ego_gps_info_.longitude;
        ego_vehicle_info.latitude = ego_gps_info_.latitude;
        ego_vehicle_info.altitude = ego_gps_info_.height;
        ego_vehicle_info.heading = ego_gps_info_.heading;
        ego_vehicle_info.gps_time = ego_gps_info_.time;
        // read long acc from fusion info
        ego_vehicle_info.longtitude_acc = ego_gps_info_.acceleration;

        ego_vehicle_info.gps_status = ego_gps_info_.position_valid_flag_for_motorcade;
        
        ego_vehicle_info.relative_x = INVALID_FLOAT;
        ego_vehicle_info.relative_y = INVALID_FLOAT;
        ego_vehicle_info.relative_heading = INVALID_FLOAT;
    }
    else
    {
        return -1;
    }
    
    //vcu info
    if(vcu_isupdate_)
    {
        ego_vehicle_info.lateral_acc = ego_vcu_info_.fLateralAccel;
        ego_vehicle_info.speed = ego_vcu_info_.fSpeed;
        ego_vehicle_info.steering_wheel_angle = ego_vcu_info_.fSteeringAngle;
        ego_vehicle_info.yaw_rate = ego_vcu_info_.fYawRate;
    } 
    else 
    {
        //ego_vehicle_info.longtitude_acc = INVALID_FLOAT;
        ego_vehicle_info.lateral_acc = INVALID_FLOAT;
        ego_vehicle_info.speed = INVALID_FLOAT;
        ego_vehicle_info.steering_wheel_angle = INVALID_FLOAT;
        ego_vehicle_info.yaw_rate = INVALID_FLOAT;
    }
    
    //  manager info
    if(manager_isupdate_)
    {
        ego_vehicle_info.desire_drive_mode = manager_info_.desire_drive_mode;
        ego_vehicle_info.vehicle_sequence = manager_info_.vehicle_sequence;
    } 
    else 
    {
        ego_vehicle_info.desire_drive_mode = 7;
        ego_vehicle_info.vehicle_sequence = -1;
    }
   
    // platoon-planning info
    if(plan_isupdate_)
    {
        ego_vehicle_info.actual_drive_mode = planning_info_.actual_drive_mode;
        ego_vehicle_info.cut_in_flag = planning_info_.cut_in;
        ego_vehicle_info.desire_long_acc = planning_info_.expire_acc;
    } 
    else 
    {
        ego_vehicle_info.actual_drive_mode = 7;
        ego_vehicle_info.cut_in_flag = 2;
        ego_vehicle_info.desire_long_acc = INVALID_FLOAT;
    } 

    //fms info 
   
    ego_vehicle_info.platoon_number = int8_t(platoon_number_);
    
    // 
    int data_len = sizeof(ego_vehicle_info);

    // assign ip port
    std::string send_ip =  ConfigData::GetInstance()->remote_ip_;
    int send_port =  ConfigData::GetInstance()->remote_port_;

    //assign header
    int header_len = 24;
    inbound_communication_header send_header;
    send_header.proto_id = 0xADFF246C;
    send_header.ver = 3;
    send_header.op_type = 2;
    send_header.op_code = 98;
    send_header.op_sn = 0;
    send_header.msg_len = data_len;

    // udp send
    char * buffer = new char[header_len + data_len];
    memcpy(buffer, &send_header, header_len);
    memcpy(buffer + header_len, &ego_vehicle_info, data_len);

    Udp sudp(send_ip_,send_port_);
    sudp.init();
    sudp.send(buffer, header_len + data_len);
    delete []buffer;

    if(m_debug_flags_ & DEBUG_BroadcastEgoVehicleInfo)
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
        printf ("ego vehicle gps_status is : %d\n", ego_vehicle_info.gps_status);
        printf ("ego vehicle longitude is : %f\n", ego_vehicle_info.longitude);
        printf ("ego vehicle latitude is  : %f\n", ego_vehicle_info.latitude);
        printf ("ego vehicle altitude is : %f\n", ego_vehicle_info.altitude);
        printf ("ego vehicle heading is : %f\n", ego_vehicle_info.heading);
        printf ("ego vehicle speed is(km/h) : %f\n", ego_vehicle_info.speed * 3.6);
        printf ("ego vehicle desire acc is : %f\n", ego_vehicle_info.desire_long_acc);
        printf ("ego vehicle acc is : %f\n\n", ego_vehicle_info.longtitude_acc);
    }

    return 1;
}

}//namespace communication
}//namespace platoon