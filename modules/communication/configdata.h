#ifndef _READ_CONFIG_H
#define _READ_CONFIG_H
#include <iostream>
#include <string>
namespace platoon {
namespace communication {

class ConfigData {
public:
//function
    static ConfigData *GetInstance();
//data
    int   vehicle_id_;
    float vehicle_length_;
    float vehicle_height_;
    float vehicle_width_;
    float desire_distance_;
//
    bool debug_;
    std::string log_path_;

private:
    static ConfigData* instance_;
    ConfigData();
    ConfigData(const ConfigData&) = delete;
    ConfigData& operator =(const ConfigData&) = delete;
};
}
}

#endif