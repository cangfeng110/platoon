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
        platoon_config.lookupValue("debug", debug_);
        platoon_config.lookupValue("log_path",log_path_);
        platoon_config.lookupValue("vehicle_id", vehicle_id_);
        platoon_config.lookupValue("vehicle_height", vehicle_height_);
        platoon_config.lookupValue("vehicle_width", vehicle_width_);
        platoon_config.lookupValue("desire_distance", desire_distance_);
        std::cout << "read all config data" << std::endl;
    } catch(const libconfig::SettingNotFoundException& e) {
        DIE("can not read config data (%s) form file  \n", e.what());

    }

}

} //namespace communication
} //namespace platoon


