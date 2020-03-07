#ifndef _READ_CONFIG_H
#define _READ_CONFIG_H
#include <iostream>
#include <string>
namespace platoon {
namespace communication {

typedef enum
{
    DEBUG_TimeToFront             = 1 << (0),
    DEBUG_CalculateID             = 1 << (1),
    DEBUG_V2xVehicleInfo          = 1 << (2),
    DEBUG_BroadcastEgoVehicleInfo = 1 << (3),
}
DebugFlags;

class ConfigData {
public:
//function
    static ConfigData *GetInstance();
    const int GetDebugFlags();
    const int GetDebugThwHZ();
    const int GetDebugGpsHZ();
    const int GetDebugVcuHZ();
    const int GetDebugPmiHZ();
// vehicle paraments config
    int   vehicle_id_;
    float vehicle_length_;
    float vehicle_height_;
    float vehicle_width_;
    float desire_distance_;
    float keep_mode_threshold_;
//udp config
    int local_port_;
    std::string remote_ip_;
    int remote_port_;
//
    bool debug_;
    std::string log_path_;

private:
    static ConfigData* instance_;
    ConfigData();
    ConfigData(const ConfigData&) = delete;
    ConfigData& operator =(const ConfigData&) = delete;
    int debug_flags_;

    bool debug_TimeToFront_;
    bool debug_CalculateID_;
    bool debug_V2xVehicleInfo_;
    bool debug_BroadcastEgoVehicleInfo_;

    int debug_thw_HZ_;
    int debug_gps_HZ_;
    int debug_vcu_HZ_;
    int debug_pmi_HZ_;
};
}
}

#endif
