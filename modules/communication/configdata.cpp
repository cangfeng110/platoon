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
        const libconfig::Setting& vehicle_config = cfg.lookup("vehicle");

        /**
         *  if the data is not in the configuration file, no error is reported
         * */
        vehicle_config.lookupValue("vehicle_license", vehicle_license_);
        vehicle_config.lookupValue("vehicle_id", vehicle_id_);
        vehicle_config.lookupValue("vehicle_height", vehicle_height_);
        vehicle_config.lookupValue("vehicle_width", vehicle_width_);
        vehicle_config.lookupValue("vehicle_length", vehicle_length_);

        const libconfig::Setting& control_config = cfg.lookup("control");
        control_config.lookupValue("keep_mode_threshold", keep_mode_threshold_);
        control_config.lookupValue("hmi_fms_valid", hmi_fms_valid_);
        control_config.lookupValue("dequeue_threshold", enqueue_threshold_);
        control_config.lookupValue("dequeue_threshold", dequeue_threshold_);
        control_config.lookupValue("to_auto_threshold", to_auto_threshold_);

        const libconfig::Setting& communication_config = cfg.lookup("communication");
        communication_config.lookupValue("local_port", local_port_);
        communication_config.lookupValue("remote_ip", remote_ip_);
        communication_config.lookupValue("remote_port", remote_port_);
        communication_config.lookupValue("broadcast_HZ", broadcast_HZ_);
        communication_config.lookupValue("is_sil_test", is_sil_test_);

        const libconfig::Setting& debug_config = cfg.lookup("debug");
        debug_config.lookupValue("debug_IfAbnormal", debug_IfAbnormal_);
        debug_config.lookupValue("debug_CalculateID", debug_CalculateID_);
        debug_config.lookupValue("debug_V2xVehicleInfo", debug_V2xVehicleInfo_);
        debug_config.lookupValue("debug_BroadcastEgoVehicleInfo", debug_BroadcastEgoVehicleInfo_);
        debug_config.lookupValue("debug_ManagetInfo", debug_ManagetInfo_);
        debug_config.lookupValue("debug_V2XCommunication", debug_V2XCommunication_);
        debug_config.lookupValue("debug_StateFlow",debug_StateFlow_);
        debug_config.lookupValue("debug_ToFmsInfo", debug_ToFmsInfo_);
        debug_config.lookupValue("debug_FmsPreInfo", debug_FmsPreInfo_);
        debug_config.lookupValue("debug_FmsBackInfo", debug_FmsBackInfo_);
        debug_flags_ = 0;
        if (debug_IfAbnormal_)
            debug_flags_ |= DEBUG_IfAbnormal;
        if (debug_CalculateID_)
            debug_flags_ |= DEBUG_CalculateID;
        if (debug_V2xVehicleInfo_)
            debug_flags_ |= DEBUG_V2xVehicleInfo;
        if (debug_BroadcastEgoVehicleInfo_)
            debug_flags_ |= DEBUG_BroadcastEgoVehicleInfo;
        if (debug_ManagetInfo_)
            debug_flags_ |= DEBUG_ManagerInfo;
        if (debug_V2XCommunication_)
            debug_flags_  |= DEBUG_V2XCommunication;

        debug_config.lookupValue("debug_thw_HZ", debug_thw_HZ_);
        debug_config.lookupValue("debug_gps_HZ", debug_gps_HZ_);
        debug_config.lookupValue("debug_vcu_HZ", debug_vcu_HZ_);
        debug_config.lookupValue("debug_pmi_HZ", debug_pmi_HZ_);
        debug_config.lookupValue("debug_plan_HZ", debug_plan_HZ_);
        debug_config.lookupValue("debug_hmi_HZ", debug_hmi_HZ_);
        std::cout << "read all config data" << std::endl;
    } catch(const libconfig::SettingNotFoundException& e) {
        DIE("can not read config data (%s) form file  \n", e.what());
    }

}

int ConfigData::GetDebugFlags () const
{
    return debug_flags_;
}

 int ConfigData::GetDebugThwHZ () const
{
    return debug_thw_HZ_;
}

int ConfigData::GetDebugGpsHZ () const
{
    return debug_gps_HZ_;
}

int ConfigData::GetDebugVcuHZ () const
{
    return debug_vcu_HZ_;
}
 
int ConfigData::GetDebugPmiHZ () const
{
    return debug_pmi_HZ_;
}

} //namespace communication
} //namespace platoon


