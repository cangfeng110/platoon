#include "configdata.h"

#include <iostream>

#include "include/libconfig/libconfig.h++"

#include "include/base/Logging.h"

namespace platoon {
namespace communication {

#define config_path "./data_conf/data/platoon.conf"

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
        platoon_config.lookupValue("vehicle_length", vehicle_length_);
        platoon_config.lookupValue("desire_distance", desire_distance_);
        platoon_config.lookupValue("local_port", local_port_);
        platoon_config.lookupValue("remote_ip", remote_ip_);
        platoon_config.lookupValue("remote_port", remote_port_);
        platoon_config.lookupValue("keep_mode_threshold", keep_mode_threshold_);
        platoon_config.lookupValue("broadcast_HZ", broadcast_HZ_);

        const libconfig::Setting& platoon_debug = cfg.lookup("debug");
        platoon_debug.lookupValue("debug_TimeToFront", debug_TimeToFront_);
        platoon_debug.lookupValue("debug_CalculateID", debug_CalculateID_);
        platoon_debug.lookupValue("debug_V2xVehicleInfo", debug_V2xVehicleInfo_);
        platoon_debug.lookupValue("debug_BroadcastEgoVehicleInfo", debug_BroadcastEgoVehicleInfo_);
        platoon_debug.lookupValue("debug_RelativeX", debug_RelativeX_);
        debug_flags_ = 0;
        if (debug_TimeToFront_)
            debug_flags_ |= DEBUG_TimeToFront;
        if (debug_CalculateID_)
            debug_flags_ |= DEBUG_CalculateID;
        if (debug_V2xVehicleInfo_)
            debug_flags_ |= DEBUG_V2xVehicleInfo;
        if (debug_BroadcastEgoVehicleInfo_)
            debug_flags_ |= DEBUG_BroadcastEgoVehicleInfo;
        if (debug_RelativeX_)
            debug_flags_ |= DEBUG_RelativeX;

        platoon_debug.lookupValue("debug_thw_HZ", debug_thw_HZ_);
        platoon_debug.lookupValue("debug_gps_HZ", debug_gps_HZ_);
        platoon_debug.lookupValue("debug_vcu_HZ", debug_vcu_HZ_);
        platoon_debug.lookupValue("debug_pmi_HZ", debug_pmi_HZ_);

        std::cout << "read all config data" << std::endl;
    } catch(const libconfig::SettingNotFoundException& e) {
        DIE("can not read config data (%s) form file  \n", e.what());

    }

}

const int
ConfigData::GetDebugFlags ()
{
    return debug_flags_;
}

const int
ConfigData::GetDebugThwHZ ()
{
    return debug_thw_HZ_;
}

const int
ConfigData::GetDebugGpsHZ ()
{
    return debug_gps_HZ_;
}

const int
ConfigData::GetDebugVcuHZ ()
{
    return debug_vcu_HZ_;
}

const int
ConfigData::GetDebugPmiHZ ()
{
    return debug_pmi_HZ_;
}

} //namespace communication
} //namespace platoon


