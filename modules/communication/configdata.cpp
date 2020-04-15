#include "configdata.h"

#include <iostream>
#include <assert.h>

#include "include/libconfig/libconfig.h++"

#include "include/base/Logging.h"

namespace platoon {
namespace communication {

#define config_path "./platoon.conf"

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
    // read vehilce config
    try
    {
        const libconfig::Setting& vehicle_config = cfg.lookup("vehicle");
        /**
         *  if the data is not in the configuration file, no error is reported
         * */
        bool read_1 = vehicle_config.lookupValue("vehicle_license", vehicle_license_);
        bool read_2 = vehicle_config.lookupValue("vehicle_id", vehicle_id_);
        bool read_3 = vehicle_config.lookupValue("vehicle_height", vehicle_height_);
        bool read_4 = vehicle_config.lookupValue("vehicle_width", vehicle_width_);
        bool read_5 = vehicle_config.lookupValue("vehicle_length", vehicle_length_);

        bool read_vehicle_flag = read_1 && read_2 && read_3 && read_4 && read_5;
        assert(read_vehicle_flag);
    }
    catch(const libconfig::SettingNotFoundException& e)
    {
        DIE("No vehicle setting in config file");
    }
    // read control config
    try
    {
        const libconfig::Setting& control_config = cfg.lookup("control");
        bool read_1 = control_config.lookupValue("keep_mode_threshold", keep_mode_threshold_);
        bool read_2 = control_config.lookupValue("hmi_fms_valid", hmi_fms_valid_);
        bool read_3 = control_config.lookupValue("dequeue_threshold", enqueue_threshold_);
        bool read_4 = control_config.lookupValue("dequeue_threshold", dequeue_threshold_);
        bool read_5 = control_config.lookupValue("to_auto_threshold", to_auto_threshold_);
        bool read_6 = control_config.lookupValue("time_to_live", time_to_live_);
        bool read_7 = control_config.lookupValue("leave_threshold", leave_threshold_);
        bool read_8 = control_config.lookupValue("formation_threshold", formation_threshold_);
        bool read_control_flag = read_1 && read_2 && read_3 && read_4 && read_5 && read_6 && read_7 && read_8;
        assert(read_control_flag);
    }
    catch(const libconfig::SettingNotFoundException& e)
    {
       DIE("No control setting in config file");
    }
    //read communication config
     try
    {
        const libconfig::Setting& communication_config = cfg.lookup("communication");
        bool read_1 = communication_config.lookupValue("local_port", local_port_);
        bool read_2 = communication_config.lookupValue("remote_ip", remote_ip_);
        bool read_3 = communication_config.lookupValue("remote_port", remote_port_);
        bool read_4 = communication_config.lookupValue("broadcast_HZ", broadcast_HZ_);
        bool read_5 = communication_config.lookupValue("is_sil_test", is_sil_test_);
        bool read_6 = communication_config.lookupValue("is_protocol_2", is_protocol_2_);
        bool read_7 = communication_config.lookupValue("is_publish_v2x", is_publish_v2x_);
        bool read_8 = communication_config.lookupValue("discard_time", discard_time_);
        bool read_communication_flag = read_1 && read_2 && read_3 && read_4 && read_5 && read_6 && read_7 && read_8;
        assert(read_communication_flag);
    }
    catch(const libconfig::SettingNotFoundException& e)
    {
        DIE("No communication setting in config file");
    }

    //read debug config
    try
    {
        const libconfig::Setting& debug_config = cfg.lookup("debug");
        bool read_1 = debug_config.lookupValue("debug_IfAbnormal", debug_IfAbnormal_);
        bool read_2 = debug_config.lookupValue("debug_CalculateID", debug_CalculateID_);
        bool read_3 = debug_config.lookupValue("debug_V2xVehicleInfo", debug_V2xVehicleInfo_);
        bool read_4 = debug_config.lookupValue("debug_BroadcastEgoVehicleInfo", debug_BroadcastEgoVehicleInfo_);
        bool read_5 = debug_config.lookupValue("debug_ManagerInfo", debug_ManagerInfo_);
        bool read_6 = debug_config.lookupValue("debug_V2XCommunication", debug_V2XCommunication_);
        bool read_7 = debug_config.lookupValue("debug_StateFlow",debug_StateFlow_);
        bool read_8 = debug_config.lookupValue("debug_ToFmsInfo", debug_ToFmsInfo_);
        bool read_9 = debug_config.lookupValue("debug_FmsPreInfo", debug_FmsPreInfo_);
        bool read_10 = debug_config.lookupValue("debug_FmsBackInfo", debug_FmsBackInfo_);
        bool read_11 = debug_config.lookupValue("debug_thw_HZ", debug_thw_HZ_);
        bool read_12 = debug_config.lookupValue("debug_gps_HZ", debug_gps_HZ_);
        bool read_13 = debug_config.lookupValue("debug_vcu_HZ", debug_vcu_HZ_);
        bool read_14 = debug_config.lookupValue("debug_pmi_HZ", debug_pmi_HZ_);
        bool read_15 = debug_config.lookupValue("debug_plan_HZ", debug_plan_HZ_);
        bool read_16 = debug_config.lookupValue("debug_hmi_HZ", debug_hmi_HZ_);
        debug_flags_ = 0;
        if (debug_IfAbnormal_)
            debug_flags_ |= DEBUG_IfAbnormal;
        if (debug_CalculateID_)
            debug_flags_ |= DEBUG_CalculateID;
        if (debug_V2xVehicleInfo_)
            debug_flags_ |= DEBUG_V2xVehicleInfo;
        if (debug_BroadcastEgoVehicleInfo_)
            debug_flags_ |= DEBUG_BroadcastEgoVehicleInfo;
        if (debug_ManagerInfo_)
            debug_flags_ |= DEBUG_ManagerInfo;
        if (debug_V2XCommunication_)
            debug_flags_  |= DEBUG_V2XCommunication;
        bool read_flag_1 = read_1 && read_2 && read_3 && read_4 && read_5 && read_6 && read_7 && read_8;
        bool read_flag_2 = read_9 && read_10 && read_11 && read_12 && read_13 && read_14 && read_15 && read_16;
        bool read_debug_flag = read_flag_1 && read_flag_2;
        assert(read_debug_flag);
    }
    catch(const libconfig::SettingNotFoundException& e)
    {
        DIE("No debug setting in config file");
    }
    //read backtrace config
    try
    {
        const libconfig::Setting& bkt_config = cfg.lookup("backtrace");
        bool read_1 = bkt_config.lookupValue("test_with_log", test_with_log_);
        bool read_2 = bkt_config.lookupValue("print_log", print_log_);
        bool read_btk = read_1 && read_2;
        assert(read_btk);   
    }
    catch(const std::exception& e)
    {
        DIE("No backtrace setting in config file");
    }
    
    // try {
    //     const libconfig::Setting& vehicle_config = cfg.lookup("vehicle");

    //     /**
    //      *  if the data is not in the configuration file, no error is reported
    //      * */
    //     vehicle_config.lookupValue("vehicle_license", vehicle_license_);
    //     vehicle_config.lookupValue("vehicle_id", vehicle_id_);
    //     vehicle_config.lookupValue("vehicle_height", vehicle_height_);
    //     vehicle_config.lookupValue("vehicle_width", vehicle_width_);
    //     vehicle_config.lookupValue("vehicle_length", vehicle_length_);

    //     const libconfig::Setting& control_config = cfg.lookup("control");
    //     control_config.lookupValue("keep_mode_threshold", keep_mode_threshold_);
    //     control_config.lookupValue("hmi_fms_valid", hmi_fms_valid_);
    //     control_config.lookupValue("dequeue_threshold", enqueue_threshold_);
    //     control_config.lookupValue("dequeue_threshold", dequeue_threshold_);
    //     control_config.lookupValue("to_auto_threshold", to_auto_threshold_);
    //     control_config.lookupValue("time_to_live", time_to_live_);
    //     control_config.lookupValue("leave_threshold", leave_threshold_);
    //     control_config.lookupValue("formation_threshold", formation_threshold_);

    //     const libconfig::Setting& communication_config = cfg.lookup("communication");
    //     communication_config.lookupValue("local_port", local_port_);
    //     communication_config.lookupValue("remote_ip", remote_ip_);
    //     communication_config.lookupValue("remote_port", remote_port_);
    //     communication_config.lookupValue("broadcast_HZ", broadcast_HZ_);
    //     communication_config.lookupValue("is_sil_test", is_sil_test_);

    //     const libconfig::Setting& debug_config = cfg.lookup("debug");
    //     debug_config.lookupValue("debug_IfAbnormal", debug_IfAbnormal_);
    //     debug_config.lookupValue("debug_CalculateID", debug_CalculateID_);
    //     debug_config.lookupValue("debug_V2xVehicleInfo", debug_V2xVehicleInfo_);
    //     debug_config.lookupValue("debug_BroadcastEgoVehicleInfo", debug_BroadcastEgoVehicleInfo_);
    //     debug_config.lookupValue("debug_ManagetInfo", debug_ManagetInfo_);
    //     debug_config.lookupValue("debug_V2XCommunication", debug_V2XCommunication_);
    //     debug_config.lookupValue("debug_StateFlow",debug_StateFlow_);
    //     debug_config.lookupValue("debug_ToFmsInfo", debug_ToFmsInfo_);
    //     debug_config.lookupValue("debug_FmsPreInfo", debug_FmsPreInfo_);
    //     debug_config.lookupValue("debug_FmsBackInfo", debug_FmsBackInfo_);
    //     debug_flags_ = 0;
    //     if (debug_IfAbnormal_)
    //         debug_flags_ |= DEBUG_IfAbnormal;
    //     if (debug_CalculateID_)
    //         debug_flags_ |= DEBUG_CalculateID;
    //     if (debug_V2xVehicleInfo_)
    //         debug_flags_ |= DEBUG_V2xVehicleInfo;
    //     if (debug_BroadcastEgoVehicleInfo_)
    //         debug_flags_ |= DEBUG_BroadcastEgoVehicleInfo;
    //     if (debug_ManagetInfo_)
    //         debug_flags_ |= DEBUG_ManagerInfo;
    //     if (debug_V2XCommunication_)
    //         debug_flags_  |= DEBUG_V2XCommunication;

    //     debug_config.lookupValue("debug_thw_HZ", debug_thw_HZ_);
    //     debug_config.lookupValue("debug_gps_HZ", debug_gps_HZ_);
    //     debug_config.lookupValue("debug_vcu_HZ", debug_vcu_HZ_);
    //     debug_config.lookupValue("debug_pmi_HZ", debug_pmi_HZ_);
    //     debug_config.lookupValue("debug_plan_HZ", debug_plan_HZ_);
    //     debug_config.lookupValue("debug_hmi_HZ", debug_hmi_HZ_);
    //     std::cout << "read all config data" << std::endl;
    // } catch(const libconfig::SettingNotFoundException& e) {
    //     DIE("can not read config data (%s) form file  \n", e.what());
    // }

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


