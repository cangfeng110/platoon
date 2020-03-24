#ifndef _READ_CONFIG_H
#define _READ_CONFIG_H
#include <iostream>
#include <string>
namespace platoon {
namespace communication {

typedef enum
{
    DEBUG_IfAbnormal              = 1 << (0),
    DEBUG_CalculateID             = 1 << (1),
    DEBUG_V2xVehicleInfo          = 1 << (2),
    DEBUG_BroadcastEgoVehicleInfo = 1 << (3),
    DEBUG_ManagerInfo             = 1 << (4),
    DEBUG_V2XCommunication        = 1 << (5),
}
DebugFlags;

class ConfigData {
public:
//function
    static ConfigData *GetInstance();
   
    int GetDebugFlags() const;
 
    int GetDebugThwHZ() const;
  
    int GetDebugGpsHZ() const;
  
    int GetDebugVcuHZ() const;
  
    int GetDebugPmiHZ() const;

// vehicle paraments config
    std::string vehicle_license_;
    int   vehicle_id_;
    float vehicle_length_;
    float vehicle_height_;
    float vehicle_width_;

// control config
    float keep_mode_threshold_;
    bool hmi_fms_valid_;
    int enqueue_threshold_;
    int dequeue_threshold_;
    
//communication config
    int local_port_;
    std::string remote_ip_;
    int remote_port_;
    int discard_time_;
    int broadcast_HZ_;

private:
    static ConfigData* instance_;
    ConfigData();
    ConfigData(const ConfigData&) = delete;
    ConfigData& operator =(const ConfigData&) = delete;

    int debug_flags_;

    bool debug_IfAbnormal_;
    bool debug_CalculateID_;
    bool debug_V2xVehicleInfo_;
    bool debug_BroadcastEgoVehicleInfo_;
    bool debug_ManagetInfo_;
    bool debug_V2XCommunication_;

    int debug_thw_HZ_;
    int debug_gps_HZ_;
    int debug_vcu_HZ_;
    int debug_pmi_HZ_;
};
}
}

#endif
