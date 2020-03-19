/*
This file is generated by script using shell
The shell commands written in proto_script/proto.sh
All the message structs are defined in files put in protobuf/crude
By compiling .proto files, proto.pb.h can be obtained and used in C++ 
Message_Channel.h include all the proto.pb.h

author: Huang Jianyu  ID:G6873
Date 2018-11-18
*/

//#ifndef MESSAGE_CHANNEL_H
//#define MESSAGE_CHANNEL_H
#pragma once
//#include "ipc_template/message/protobuf/include/canbus_conf.pb.h"
//#include "ipc_template/message/protobuf/include/channel_config.pb.h"
//#include "ipc_template/message/protobuf/include/chassis_detail.pb.h"
//#include "ipc_template/message/protobuf/include/chassis.pb.h"
//#include "ipc_template/message/protobuf/include/delphi_esr.pb.h"
//#include "ipc_template/message/protobuf/include/drive_event.pb.h"
//#include "ipc_template/message/protobuf/include/drive_state.pb.h"
//#include "ipc_template/message/protobuf/include/dr.pb.h"
//#include "ipc_template/message/protobuf/include/enginedestination.pb.h"
//#include "ipc_template/message/protobuf/include/enginelocmap.pb.h"
//#include "ipc_template/message/protobuf/include/engineremaindistance.pb.h"
//#include "ipc_template/message/protobuf/include/enginesurmap.pb.h"
//#include "ipc_template/message/protobuf/include/enginetargetlanes.pb.h"
//#include "ipc_template/message/protobuf/include/error_code.pb.h"
//#include "ipc_template/message/protobuf/include/geometry.pb.h"
//#include "ipc_template/message/protobuf/include/gridlist.pb.h"
//#include "ipc_template/message/protobuf/include/header.pb.h"
//#include "ipc_template/message/protobuf/include/ins.pb.h"
//#include "ipc_template/message/protobuf/include/lcm_example.pb.h"
//#include "ipc_template/message/protobuf/include/localization.pb.h"
//#include "ipc_template/message/protobuf/include/measure.pb.h"
//#include "ipc_template/message/protobuf/include/mobileye.pb.h"
//#include "ipc_template/message/protobuf/include/mobileyeq4.pb.h"
//#include "ipc_template/message/protobuf/include/monitor_conf.pb.h"
//#include "ipc_template/message/protobuf/include/monitor_log.pb.h"
//#include "ipc_template/message/protobuf/include/online_report.pb.h"
//#include "ipc_template/message/protobuf/include/perception_obstacle.pb.h"
//#include "ipc_template/message/protobuf/include/pf.pb.h"
//#include "ipc_template/message/protobuf/include/pnc_point.pb.h"
//#include "ipc_template/message/protobuf/include/pose.pb.h"
//#include "ipc_template/message/protobuf/include/reserved.pb.h"
//#include "ipc_template/message/protobuf/include/static_info.pb.h"
//#include "ipc_template/message/protobuf/include/system_status.pb.h"
//#include "ipc_template/message/protobuf/include/TEST.pb.h"
//#include "ipc_template/message/protobuf/include/ultrasonic.pb.h"
//#include "ipc_template/message/protobuf/include/uwb.pb.h"
//#include "ipc_template/message/protobuf/include/vcu.pb.h"
//#include "ipc_template/message/protobuf/include/vehicle_config.pb.h"
//#include "ipc_template/message/protobuf/include/vehicle_parameter.pb.h"
//#include "ipc_template/message/protobuf/include/vehicle_signal.pb.h"
//#include "ipc_template/message/protobuf/include/vehicle_state.pb.h"

#include "modules/ipc/include/message_channel.h"

const std::string CHANNEL_NAME_FixMsg = "FixMsg";
const std::string CHANNEL_NAME_DelimitedMsg = "DelimitedMsg";
const std::string CHANNEL_NAME_Monitor = "Monitor";
const std::string CHANNEL_NAME_PROTO_DrList = "PROTO_DrList";
const std::string CHANNEL_NAME_PROTO_DrInsList = "PROTO_DrInsList";
const std::string CHANNEL_NAME_PROTO_Pfs = "PROTO_Pfs";
const std::string CHANNEL_NAME_PROTO_PfsAfterMatch = "PROTO_PfsAfterMatch";
const std::string CHANNEL_NAME_PROTO_GridList = "PROTO_GridList";
const std::string CHANNEL_NAME_PROTO_GridListMatchDisplay = "PROTO_GridListMatchDisplay";
const std::string CHANNEL_NAME_PROTO_Localization = "PROTO_Localization";
const std::string CHANNEL_NAME_PROTO_LocalizationAbs = "PROTO_LocalizationAbs";
const std::string CHANNEL_NAME_PROTO_DriverIns = "PROTO_DriverIns";
const std::string CHANNEL_NAME_PROTO_DriverUwb = "PROTO_DriverUwb";
const std::string CHANNEL_NAME_PROTO_DriverVcu = "PROTO_DriverVcu";
const std::string CHANNEL_NAME_PROTO_DriverUltrasonic = "PROTO_DriverUltrasonic";
const std::string CHANNEL_NAME_PROTO_DriverMobileyeLane = "PROTO_DriverMobileyeLane";
const std::string CHANNEL_NAME_PROTO_LocalizationMapMatch = "PROTO_LocalizationMapMatch";
const std::string CHANNEL_NAME_PROTO_EngineLocMap = "PROTO_EngineLocMap";
const std::string CHANNEL_NAME_PROTO_EngineTargetLanes = "PROTO_EngineTargetLanes";
const std::string CHANNEL_NAME_PROTO_EngineTargetArrows = "PROTO_EngineTargetArrows";
const std::string CHANNEL_NAME_PROTO_EngineRemainDistance = "PROTO_EngineRemainDistance";
const std::string CHANNEL_NAME_PROTO_EngineSurMap = "PROTO_EngineSurMap";
const std::string CHANNEL_NAME_PROTO_EngineLocMapBuffer = "PROTO_EngineLocMapBuffer";
const std::string CHANNEL_NAME_PROTO_EngineTargetLanesBuffer = "PROTO_EngineTargetLanesBuffer";
const std::string CHANNEL_NAME_PROTO_EngineTargetArrowsBuffer = "PROTO_EngineTargetArrowsBuffer";
const std::string CHANNEL_NAME_PROTO_EngineRemainDistanceBuffer = "PROTO_EngineRemainDistanceBuffer";
const std::string CHANNEL_NAME_PROTO_EngineSurMapBuffer = "PROTO_EngineSurMapBuffer";
const std::string CHANNEL_NAME_PROTO_EngineDestination = "PROTO_EngineDestination";
const std::string CHANNEL_NAME_PROTO_MeFreeSpace = "PROTO_MeFreeSpace";
const std::string CHANNEL_NAME_PROTO_ConstructionArea = "PROTO_ConstructionArea";
const std::string CHANNEL_NAME_PROTO_MeHLB = "PROTO_MeHLB";
const std::string CHANNEL_NAME_PROTO_Lane_Support = "PROTO_Lane_Support";
const std::string CHANNEL_NAME_PROTO_Lane_Support_Debug = "PROTO_Lane_Support_Debug";
const std::string CHANNEL_NAME_PROTO_MeLDW = "PROTO_MeLDW";
const std::string CHANNEL_NAME_PROTO_LS_ADV = "PROTO_LS_ADV";
const std::string CHANNEL_NAME_PROTO_MeLsRoadEdge = "PROTO_MeLsRoadEdge";
const std::string CHANNEL_NAME_PROTO_SematicLine = "PROTO_SematicLine";
const std::string CHANNEL_NAME_PROTO_SematicMark = "PROTO_SematicMark";
const std::string CHANNEL_NAME_PROTO_SL_Descrip = "PROTO_SL_Descrip";
const std::string CHANNEL_NAME_PROTO_MeObjects = "PROTO_MeObjects";
const std::string CHANNEL_NAME_PROTO_TFL = "PROTO_TFL";
const std::string CHANNEL_NAME_PROTO_TSR = "PROTO_TSR";
const std::string CHANNEL_NAME_PROTO_MeApplication = "PROTO_MeApplication";
const std::string CHANNEL_NAME_PROTO_MeCommon = "PROTO_MeCommon";
const std::string CHANNEL_NAME_PROTO_FCFVD = "PROTO_FCFVD";
const std::string CHANNEL_NAME_PROTO_FCF_VRU = "PROTO_FCF_VRU";
const std::string CHANNEL_NAME_PROTO_FailSafe = "PROTO_FailSafe";
const std::string CHANNEL_NAME_TEST = "TEST";
//const std::string CHANNEL_NAME_PROTO_DriverIns
