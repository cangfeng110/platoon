#include "configdata.h"

#include <iostream>

#include "include/libconfig/libconfig.h++"

#include "include/base/Logging.h"

namespace platoon {
namespace communication {

#define config_path "./config/platoon.conf"

ConfigData *ConfigData::instance_ = new ConfigData();
ConfigData *ConfigData::GetInstance() {
    return instance_;
}

ConfigData::ConfigData() {
    libconfig::Config cfg;
    try {
        cfg.readFile(config_path);
    } catch (const libconfig::FileIOException& e) {
        DIE("can not open file %s \n", config_path);
    } catch (const libconfig::ParseException& e) {
        DIE("can not parse file %s : %d %s \n",e.getFile(), e.getLine(), e.getError());
    }

    try {
        const libconfig::Setting& platoon_config = cfg.lookup("platoon");
        //because GCC 4.8 auto trans string to cosnt string
        const char* debug = "debug"; 
        const char* log_path = "log_path";
        const char* vehicle_id = "vehicle_id";
        const char* vehicle_height = "vehicle_height";
        const char* vehicle_width = "vehicle_width";
        const char* desire_distance = "desire_distance";
        const char* local_port = "local_port";
        const char* remote_ip = "remote_ip";
        const char* remote_port = "remote_port";
        platoon_config.lookupValue(debug, debug_);
        platoon_config.lookupValue(log_path,log_path_);
        platoon_config.lookupValue(vehicle_id, vehicle_id_);
        platoon_config.lookupValue(vehicle_height, vehicle_height_);
        platoon_config.lookupValue(vehicle_width, vehicle_width_);
        platoon_config.lookupValue(desire_distance, desire_distance_);
        platoon_config.lookupValue(local_port, local_port_);
        platoon_config.lookupValue(remote_ip, remote_ip_);
        platoon_config.lookupValue(remote_port, remote_port_);

        std::cout << "read all config data" << std::endl;
    } catch(const libconfig::SettingNotFoundException& e) {
        DIE("can not read config data (%s) form file  \n", e.what());

    }

}

} //namespace communication
} //namespace platoon


