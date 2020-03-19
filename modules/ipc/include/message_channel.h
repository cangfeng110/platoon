#ifndef MODULES_IPC_PROTOBUF_MESSAGE_CHANNEL_H_
#define MODULES_IPC_PROTOBUF_MESSAGE_CHANNEL_H_

#include "modules/ipc/message/protobuf/lcm_example.pb.h"

//monitor
#include "modules/ipc/message/protobuf/monitor/monitor_log/monitor_log.pb.h"
#include "modules/ipc/message/protobuf/monitor/monitor_conf.pb.h"
#include "modules/ipc/message/protobuf/monitor/online_report.pb.h"
#include "modules/ipc/message/protobuf/monitor/system_status.pb.h"

//driver
#include "modules/ipc/message/protobuf/drivers/ins.pb.h"
#include "modules/ipc/message/protobuf/drivers/uwb.pb.h"
#include "modules/ipc/message/protobuf/drivers/vcu.pb.h"
#include "modules/ipc/message/protobuf/drivers/ultrasonic.pb.h"
#include "modules/ipc/message/protobuf/drivers/mobileye.pb.h"
#include "modules/ipc/message/protobuf/drivers/momentaloc.pb.h"

#include "modules/ipc/message/protobuf/dr/dr.pb.h"
#include "modules/ipc/message/protobuf/localization/localization.pb.h"
#include "modules/ipc/message/protobuf/perception/gridlist.pb.h"

#include "modules/ipc/message/protobuf/common/geometry.pb.h"
#include "modules/ipc/message/protobuf/common/pnc_point.pb.h"

//map
#include "modules/ipc/message/protobuf/hdmap/enginelocmap.pb.h"
#include "modules/ipc/message/protobuf/hdmap/enginetargetlanes.pb.h"
#include "modules/ipc/message/protobuf/hdmap/engineremaindistance.pb.h"
#include "modules/ipc/message/protobuf/hdmap/enginesurmap.pb.h"
#include "modules/ipc/message/protobuf/hdmap/enginedestination.pb.h"

#include "modules/ipc/message/protobuf/hdmap/gridlog.pb.h"
//EyeQ4
#include "modules/ipc/message/protobuf/MeProcess/mobileyeq4.pb.h"

//lcm
#include "modules/ipc/message/lcm/VCU_VEHILCE_INFO_After20170612.hpp"
#include "modules/ipc/message/lcm/ULTRASONIC_DISTS.hpp"
//#include "modules/ipc/message/lcm/MELaneLines.hpp"
#include "modules/ipc/message/lcm/MOBILEYE_LANES.hpp"
#include "modules/ipc/message/lcm/MOBILEYE_FREESPACE.hpp"
#include "modules/ipc/message/lcm/MOBILEYE_OBJECTS.hpp"
#include "modules/ipc/message/lcm/USDistance.hpp"
#include "modules/ipc/message/lcm/LOGITECH_IMAGE.hpp"

#include "modules/ipc/message/lcm/RAW_INS.hpp"
#include "modules/ipc/message/lcm/INS300C.hpp"

#include "modules/ipc/message/lcm/UWBData.hpp"
#include "modules/ipc/message/lcm/VIEWLOC.hpp"
#include "modules/ipc/message/lcm/OFILM_LOCATION.hpp"
#include "modules/ipc/message/lcm/DRIVER_MAP_TSR_DISTANCE.hpp"
#include "modules/ipc/message/lcm/DRIVER_MAP_ONE_PARKING_SITE.hpp"
#include "modules/ipc/message/lcm/PERCEPTION_LINES.hpp"
#include "modules/ipc/message/lcm/ADM_RPPOINT_LIST.hpp"
#include "modules/ipc/message/lcm/PARKINGLOT.hpp"
//map_app
#include "modules/ipc/message/lcm/APP_PROXY_REQUEST.hpp"
#include "modules/ipc/message/lcm/DRIVER_MAP_PARKING_SITE_POI_LIST.hpp"
#include "modules/ipc/message/lcm/EXECUTE_BEHAVIOR_PLAN.hpp"

#include "modules/ipc/message/lcm/OWNSINS.hpp"
#include "modules/ipc/message/lcm/DISPLAY_CoupleOut.hpp"
#include "modules/ipc/message/lcm/EKF_OUT.hpp"
#include "modules/ipc/message/lcm/MWR_RAW_OBJECTS.hpp"
#include "modules/ipc/message/lcm/MWR_RAW_OBJECTS_BOSCH.hpp"
#include "modules/ipc/message/lcm/MOBILEYE_TFLS.hpp"
#include "modules/ipc/message/lcm/PERCEPTION_TSR.hpp"

#include "modules/ipc/message/lcm/PERCEPTION_OBSTACLES.hpp"
#include "modules/ipc/message/lcm/TRACKED_OBJECTS.hpp"

#include "modules/ipc/message/protobuf/MwrProcess/DensoMrr.pb.h"


namespace atd
{
namespace ipc
{

//protobuf
using FixMsgProto = atd::common::FixMsg;
using DelimitedMsgProto = atd::common::DelimitedMsg;
using DrListProto = atd::dr::DrList;
using DrInsListProto = atd::dr::DrList;
using PfsProto = atd::localization::Particles_Collection;
using PfsAfterMatchProto = atd::localization::Particles_Collection;

using LocalizationProto=atd::localization::LocalizationEstimate;
using LocalizationAbsProto=atd::localization::LocalizationAbsEstimate;
using LocalizationMapMatchProto=atd::localization::LocalizationMapMatchEstimate;

using GridListProto = atd::perception::GridList;
using GridListMatchDisplayProto = atd::perception::MatchDisplay;

using DriverInsProto = atd::drivers::Ins;
using DriverUwbProto = atd::drivers::Uwb;
using DriverVcuProto = atd::drivers::Vcu;
using DrivermomentalocProto = atd::drivers::Momentaloc;
using DriverUltrasonicProto = atd::drivers::Perception_ULTRASONIC;
using DriverMobileyeLaneProto = atd::drivers::MOBI_LANES;


//map
using EngineLocMapProto = atd::map::EngineLocMap;
using EngineTargetArrowsProto = atd::map::EngineTargetLanes;
using EngineTargetLanesProto = atd::map::EngineTargetLanes;
using EngineRemainDistanceProto = atd::map::EngineRemainDistance;
using EngineSurMapProto = atd::map::EngineSurMap;

using EngineLocMapBufferProto = atd::map::EngineLocMap;
using EngineTargetArrowsBufferProto = atd::map::EngineTargetLanes;
using EngineTargetLanesBufferProto = atd::map::EngineTargetLanes;
using EngineRemainDistanceBufferProto = atd::map::EngineRemainDistance;
using EngineSurMapBufferProto = atd::map::EngineSurMap;
using EngineDestinationProto = atd::map::EngineDestination;

//gridlog
using GridLogProto=atd::map::GridLogInfo;

using ConstructionAreaProto = atd::meprocess::Dt_Record_ConstructionArea;
using Lane_SupportProto = atd::meprocess::Dt_Record_Lane_Support;
using LS_ADVProto = atd::meprocess::Dt_Record_LS_ADV;
using LS_Road_EdgeProto = atd::meprocess::Dt_Record_LS_Road_Edge;
using MeFreeSpaceProto = atd::meprocess::Dt_Record_MeFreeSpace;
using MeObjectsProto = atd::meprocess::Dt_Record_MeObjects;
using SematicLineProto = atd::meprocess::Dt_Record_SematicLine;
using SematicMarkProto = atd::meprocess::Dt_Record_SematicMark;
using SL_DescripProto = atd::meprocess::Dt_Record_SL_Descrip;
using TFLProto = atd::meprocess::Dt_Record_TFL;
using TSRProto = atd::meprocess::Dt_Record_TSR;
using MeHLBProto = atd::meprocess::Dt_Record_HLB;
using MeLDWProto = atd::meprocess::Dt_Record_LDW;
using MeApplicationProto = atd::meprocess::Dt_Record_App;
using MeCommonProto = atd::meprocess::Dt_Record_Common;
using FCFVDProto = atd::meprocess::Dt_Record_FCF;
using FCF_VRUProto = atd::meprocess::Dt_Record_FCF_VRU;
using FailSafeProto = atd::meprocess::Dt_Record_FailSafe;

//protobuf_chassis
//using ChassisAdapter = Adapter<::atd::canbus::Chassis>;
//using ChassisDetailAdapter = Adapter<::atd::canbus::ChassisDetail>;
//using ControlCommandAdapter = Adapter<control::ControlCommand>;

//monitor
using MonitorProto = atd::ipc::monitor::MonitorMessage;
using SystemStatusProto = atd::monitor::SystemStatus;
//using StaticInfoAdapter = Adapter<atd::data::StaticInfo>;


//protobuf_drivers
//using MobileyeAdapter = Adapter<drivers::Mobileye>;
//using DelphiESRAdapter = Adapter<drivers::DelphiESR>;
//using ContiRadarAdapter = Adapter<drivers::ContiRadar>;
//using RawImuAdapter = Adapter<atd::drivers::gnss::Imu>;
//using InsStatAdapter = Adapter<drivers::gnss::InsStat>;

//raw_data
//using CompressedImageAdapter = Adapter<sensor_msgs::CompressedImage>;
//using PointCloudAdapter = Adapter<::sensor_msgs::PointCloud2>;
//using ImageShortAdapter = Adapter<::sensor_msgs::Image>;
//using ImageLongAdapter = Adapter<::sensor_msgs::Image>;


//key module
//using GpsAdapter = Adapter<atd::dr::Gps>;
//using ImuAdapter = Adapter<dr::Imu>;
//using LocalizationAdapter = Adapter<atd::dr::LocalizationEstimate>;
//using PredictionAdapter = Adapter<prediction::PredictionObstacles>;
//using PadAdapter = Adapter<control::PadMessage>;
//using PerceptionObstaclesAdapter = Adapter<perception::PerceptionObstacles>;
//using PlanningAdapter = Adapter<planning::ADCTrajectory>;
//using DriveEventAdapter = Adapter<DriveEvent>;
//using TrafficLightDetectionAdapter = Adapter<perception::TrafficLightDetection>;
//using RoutingRequestAdapter = Adapter<routing::RoutingRequest>;
//using RoutingResponseAdapter = Adapter<routing::RoutingResponse>;


//lcm
using HeaderLcm = HEADER;
using VcuLcm = VCU_VEHILCE_INFO;
using MELaneLinesLcm = MOBILEYE_LANES;
using USDistsLcm = ULTRASONIC_DISTS; //preprocess
using USDistanceLcm = USDistance;//raw data
using LogitechCameraLcm = LOGITECH_IMAGE;
using RawInsLcm = RAW_INS;
using RawIns300CLcm = INS300C;
using OWNSINSLcm = OWNSINS;
using OFILMlocLcm = OFILM_LOCATION;
using MomentaLocLcm = VIEWLOC;
using UwbLcm = UWBData;
using MapRDisTsrLcm = DRIVER_MAP_TSR_DISTANCE;
using MapRDisOneParkLcm = DRIVER_MAP_ONE_PARKING_SITE;
using MapTargetLanesLcm = PERCEPTION_LINES;
using FusionPerceptionLaneLcm = PERCEPTION_LINES;
using NaviRouteLcm = ADM_RPPOINT_LIST;
using ProxyRequestLcm = APP_PROXY_REQUEST;
using CoupleOutLcm = DISPLAY_CoupleOut;
using AdpStatusLcm = EXECUTE_BEHAVIOR_PLAN;
using EKFOutLcm = EKF_OUT;
using OFILMParkingSearchLcm=PARKINGLOT;

using MEFreeSpaceLcm = MOBILEYE_FREESPACE;
using MEObjectsLcm = MOBILEYE_OBJECTS;

using MWRRawObjectsFSDALcm = MWR_RAW_OBJECTS;
using MWRRawObjectsRDALcm = MWR_RAW_OBJECTS;

using MWRRawObjectsSV51FrontLcm = MWR_RAW_OBJECTS;
using MWRRawObjectsSV51FrontLeftLcm = MWR_RAW_OBJECTS;
using MWRRawObjectsSV51FrontRightLcm = MWR_RAW_OBJECTS;
using MWRRawObjectsSV51RearLeftLcm = MWR_RAW_OBJECTS;
using MWRRawObjectsSV51RearRightLcm = MWR_RAW_OBJECTS;

using MWRBoschObjectsLcm = MWR_RAW_OBJECTS_BOSCH;
using PerceptionTSRLcm = PERCEPTION_TSR;


using PerceptionObstaclesLcm= PERCEPTION_OBSTACLES;
using TrackedObjectsLcm=TRACKED_OBJECTS;

}  // namespace ipc
}  // namespace atd

#endif  // MODULES_IPC_PROTOBUF_MESSAGE_CHANNEL_H_
