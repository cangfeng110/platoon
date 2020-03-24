#ifndef LCM_CHANNEL_H_
#define LCM_CHANNEL_H_

const std::string CHANNEL_NAME_BREAKTHROUGH_MSG="BREAKTHROUGH";//for test pathplan
const std::string CHANNEL_NAME_CAN_MSG="CAN";
const std::string CHANNEL_NAME_CONTROL_CMD="CONTROL_CMD";
const std::string CHANNEL_NAME_PIKE_IMAGE="PIKE_IMAGE";
const std::string CHANNEL_NAME_VCU_550="VCU_550";

const std::string CHANNEL_NAME_EXECUTE_TRAJECTORY="EXECUTE_TRAJECTORY";
const std::string CHANNEL_NAME_FITTED_TRAJECTORY="FITTED_TRAJECTORY";
const std::string CHANNEL_NAME_VCU_VEHICLE_INFO="VCU_VEHICLE_INFO";
const std::string CHANNEL_NAME_EXECUTE_BEHAVIOR_PLAN="EXECUTE_BEHAVIOR_PLAN";
//ins related channels;
//=======================================================================================
const std::string CHANNEL_NAME_INERTIAL="INERTIAL";
const std::string CHANNEL_NAME_SPATIAL_FOG="SPATIAL_FOG";
const std::string CHANNEL_NAME_UBLOX="UBLOX";
const std::string CHANNEL_NAME_GPS982="GPS982";
const std::string CHANNEL_NAME_RAW_INS_ORG="RAW_INS_ORG";
const std::string CHANNEL_NAME_RAW_INS="RAW_INS";
const std::string CHANNEL_NAME_OFFSET_INS="OFFSET_INS";
const std::string CHANNEL_NAME_TRANSFORM="TRANSFORM";
const std::string CHANNEL_NAME_INS300C58="INS300C58";
const std::string CHANNEL_NAME_INS300C="INS300C";
//=======================================================================================


//mwr related channels;
//=======================================================================================
const std::string CHANNEL_NAME_MWR_RAW_FRONT_OBJECTS="MWR_RAW_FRONT_OBJECTS";
const std::string CHANNEL_NAME_MWR_RAW_REAR_ADAS_MSG="MWR_RAW_REAR_ADAS_MSG";
const std::string CHANNEL_NAME_MWR_RAW_REAR_OBJECTS_FROM_PCAN_IN_SENSOR_COORDINATE="MWR_RAW_REAR_OBJECTS_FROM_PCAN_IN_SENSOR_COORDINATE";
const std::string CHANNEL_NAME_MWR_RAW_LEFT_OBJECTS_FROM_TCPIP_IN_RECTANGULAR_COORDINATE="MWR_RAW_LEFT_OBJECTS_FROM_TCPIP_IN_RECTANGULAR_COORDINATE";
const std::string CHANNEL_NAME_MWR_RAW_RIGHT_OBJECTS_FROM_TCPIP_IN_RECTANGULAR_COORDINATE="MWR_RAW_RIGHT_OBJECTS_FROM_TCPIP_IN_RECTANGULAR_COORDINATE";
const std::string CHANNEL_NAME_MWR_FILTERED_FRONT_OBJECTS="MWR_FILTERED_FRONT_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_LEFT_OBJECTS="MWR_FILTERED_LEFT_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_RIGHT_OBJECTS="MWR_FILTERED_RIGHT_OBJECTS";

const std::string CHANNEL_NAME_MWR_SELECTED_FRONT_OBJECTS="MWR_SELECTED_FRONT_OBJECTS";
const std::string CHANNEL_NAME_MWR_SELECTED_LEFT_OBJECTS="MWR_SELECTED_LEFT_OBJECTS";
const std::string CHANNEL_NAME_MWR_SELECTED_RIGHT_OBJECTS="MWR_SELECTED_RIGHT_OBJECTS";

const std::string CHANNEL_NAME_MWR_SELECTED_FM_OBJECTS="MWR_SELECTED_FM_OBJECTS";
const std::string CHANNEL_NAME_MWR_SELECTED_FL_OBJECTS="MWR_SELECTED_FL_OBJECTS";
const std::string CHANNEL_NAME_MWR_SELECTED_FR_OBJECTS="MWR_SELECTED_FR_OBJECTS";

const std::string CHANNEL_NAME_MWR_SELECTED_RM_OBJECTS="MWR_SELECTED_RM_OBJECTS";
const std::string CHANNEL_NAME_MWR_SELECTED_RL_OBJECTS="MWR_SELECTED_RL_OBJECTS";
const std::string CHANNEL_NAME_MWR_SELECTED_RR_OBJECTS="MWR_SELECTED_RR_OBJECTS";

//EP21
//const std::string CHANNEL_NAME_MWR_FRONT_ESR_OBJECTS="MWR_FRONT_ESR_OBJECTS";
//const std::string CHANNEL_NAME_MWR_LEFT_RSDS_OBJECTS="MWR_LEFT_RSDS_OBJECTS";
//const std::string CHANNEL_NAME_MWR_RIGHT_RSDS_OBJECTS="MWR_RIGHT_RSDS_OBJECTS";
//const std::string CHANNEL_NAME_MWR_LEFT_SRR_OBJECTS="MWR_LEFT_SRR_OBJECTS";
//const std::string CHANNEL_NAME_MWR_RIGHT_SRR_OBJECTS="MWR_RIGHT_SRR_OBJECTS";
//const std::string CHANNEL_NAME_MWR_REAR_ESR_OBJECTS="MWR_REAR_ESR_OBJECTS";


const std::string CHANNEL_NAME_MWR_RAW_FRONT_ESR_OBJECTS="MWR_RAW_FRONT_ESR_OBJECTS";
const std::string CHANNEL_NAME_MWR_RAW_LEFT_RSDS_OBJECTS="MWR_RAW_LEFT_RSDS_OBJECTS";
const std::string CHANNEL_NAME_MWR_RAW_RIGHT_RSDS_OBJECTS="MWR_RAW_RIGHT_RSDS_OBJECTS";
const std::string CHANNEL_NAME_MWR_RAW_LEFT_SRR_OBJECTS="MWR_RAW_LEFT_SRR_OBJECTS";
const std::string CHANNEL_NAME_MWR_RAW_RIGHT_SRR_OBJECTS="MWR_RAW_RIGHT_SRR_OBJECTS";
const std::string CHANNEL_NAME_MWR_RAW_REAR_ESR_OBJECTS="MWR_RAW_REAR_ESR_OBJECTS";

const std::string CHANNEL_NAME_MWR_FILTERED_FRONT_ESR_OBJECTS="MWR_FILTERED_FRONT_ESR_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_LEFT_RSDS_OBJECTS="MWR_FILTERED_LEFT_RSDS_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_RIGHT_RSDS_OBJECTS="MWR_FILTERED_RIGHT_RSDS_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_LEFT_SRR_OBJECTS="MWR_FILTERED_LEFT_SRR_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_RIGHT_SRR_OBJECTS="MWR_FILTERED_RIGHT_SRR_OBJECTS";
const std::string CHANNEL_NAME_MWR_FILTERED_REAR_ESR_OBJECTS="MWR_FILTERED_REAR_ESR_OBJECTS";
//=======================================================================================

//US related channels;
//=======================================================================================
const std::string CHANNEL_NAME_US_DISTS="US_DISTS";
const std::string CHANNEL_NAME_US_FILTED_DISTS="US_FILTED_DISTS";
const std::string CHANNEL_NAME_US_PARKLOTS="US_PARKLOTS";
const std::string CHANNEL_NAME_US_FREESPACE="US_FREESPACE";
const std::string CHANNEL_NAME_OFILM_PARKINGSEARCH = "OFILM_PARKINGSEARCH";
const std::string CHANNEL_NAME_PERCEPTION_PARKLOTS = "PERCEPTION_PARKLOTS";
const std::string CHANNEL_NAME_OFILM_PARKINGLOT_DISTS = "OFILM_PARKINGLOT_DISTS";
const std::string CHANNEL_NAME_US_ROAD_EDGE = "US_ROAD_EDGE";


//=======================================================================================


//mobileye related channels;
//=======================================================================================
const std::string CHANNEL_NAME_MOBILEYE_LANES="MOBILEYE_LANES";
const std::string CHANNEL_NAME_MOBILEYE_HPP="MOBILEYE_HPP";
const std::string CHANNEL_NAME_MOBILEYE_OBJECTS="MOBILEYE_OBJECTS";
const std::string CHANNEL_NAME_MOBILEYE_FREESPACE="MOBILEYE_FREESPACE";
const std::string CHANNEL_NAME_MOBILEYE_FILTERED_FREESPACE="MOBILEYE_FILTERED_FREESPACE";
const std::string CHANNEL_NAME_MOBILEYE_VIRTUAL_LANES="MOBILEYE_VIRTUAL_LANES";
const std::string CHANNEL_NAME_MOBILEYE_TFLS="MOBILEYE_TFLS";
const std::string CHANNEL_NAME_MOBILEYE_FILTERED_OBJECTS="MOBILEYE_FILTERED_OBJECTS";
//=======================================================================================

//ibeo related channels;
//=======================================================================================
const std::string CHANNEL_NAME_LUX_POINTS_FRONT="LUX_POINTS_FRONT";
const std::string CHANNEL_NAME_LUX_POINTS_LEFT="LUX_POINTS_LEFT";
const std::string CHANNEL_NAME_LUX_POINTS_BACK="LUX_POINTS_BACK";
const std::string CHANNEL_NAME_LUX_POINTS_RIGHT="LUX_POINTS_RIGHT";
const std::string CHANNEL_NAME_LUX_OBJECTS_FRONT="LUX_OBJECTS_FRONT";
const std::string CHANNEL_NAME_LUX_OBJECTS_LEFT="LUX_OBJECTS_LEFT";
const std::string CHANNEL_NAME_LUX_OBJECTS_BACK="LUX_OBJECTS_BACK";
const std::string CHANNEL_NAME_LUX_OBJECTS_RIGHT="LUX_OBJECTS_RIGHT";
const std::string CHANNEL_NAME_IBEO_OBJECTS="IBEO_OBJECTS";
const std::string CHANNEL_NAME_IBEO_POINTS="IBEO_POINTS";
const std::string CHANNEL_NAME_IBEO_FILTERED_OBJECTS="IBEO_FILTERED_OBJECTS";
//=======================================================================================

//velodyne related channels;
//=======================================================================================
const std::string CHANNEL_NAME_VELODYNE64="VELODYNE64";
const std::string CHANNEL_NAME_VELODYNE64_COMPENSATE="VELODYNE64_COMPENSATE";
const std::string CHANNEL_NAME_VELODYNE_GROUND_ESTIMATION="VELODYNE_GROUND_ESTIMATION";
const std::string CHANNEL_NAME_VELODYNE_OBJECT_CLUSTERS="VELODYNE_OBJECT_CLUSTERS";
const std::string CHANNEL_NAME_VELODYNE_OBJECT_CONVEX_HULLS="VELODYNE_OBJECTS_CONVEX_HULLS";
const std::string CHANNEL_NAME_VELODYNE_TRANSFORMED_CONVEX_HULLS="VELODYNE_OBJECTS_TRANSFORMED_CONVEX_HULLS";
const std::string CHANNEL_NAME_VELODYNE_OBJECTS_CORRELATIONS="VELODYNE_OBJECT_CORRELATIONS";
const std::string CHANNEL_NAME_VELODYNE_OBJECTS="VELODYNE_OBJECTS";
const std::string CHANNEL_NAME_VELODYNE_GRIDMAP_LABEL="VELODYNE_GRIDMAP_LABEL";
const std::string CHANNEL_NAME_VELODYNE_720POINTS="VELODYNE_720POINTS";
//=======================================================================================

//can2 proxy related channels;
//=======================================================================================
const std::string CHANNEL_NAME_CAN2_FRONTMWR2IPC="CAN2_FRONTMWR2IPC";
const std::string CHANNEL_NAME_CAN2_VCU2IPC="CAN2_VCU2IPC";
const std::string CHANNEL_NAME_CAN2_IPC2VCU_GPS="CAN2_IPC2VCU_GPS";
const std::string CHANNEL_NAME_CAN2_IPC2VCU_TRAJECTORY="CAN2_IPC2VCU_TRAJECTORY";
const std::string CHANNEL_NAME_CAN2_IPC2VCU_MELANES="CAN2_IPC2VCU_MELANES";
const std::string CHANNEL_NAME_CAN2_US="CAN2_US";
//=======================================================================================

//canet proxy related channels;
//=======================================================================================
const std::string CHANNEL_NAME_CANET_MWR_FRONT_ESR2IPC="CANET_MWR_FRONT_ESR2IPC";
const std::string CHANNEL_NAME_CANET_MWR_LEFT_RSDSPCAN2IPC="CANET_MWR_LEFT_RSDSPCAN2IPC";
const std::string CHANNEL_NAME_CANET_MWR_RIGHT_RSDSPCAN2IPC="CANET_MWR_RIGHT_RSDSPCAN2IPC";
const std::string CHANNEL_NAME_CANET_MWR_LEFT_SRR2IPC="CANET_MWR_LEFT_SRR2IPC";
const std::string CHANNEL_NAME_CANET_MWR_RIGHT_SRR2IPC="CANET_MWR_RIGHT_SRR2IPC";
const std::string CHANNEL_NAME_CANET_MWR_REAR_ESR2IPC="CANET_MWR_REAR_ESR2IPC";
//=======================================================================================


//iboe perception oct obstacles;
//=======================================================================================
const std::string CHANNEL_NAME_PERCEPTION_OCT_OBSTACLES="PERCEPTION_OCT_OBSTACLES";
//const std::string CHANNEL_NAME_PERCEPTION_OBJECTS="PERCEPTION_OBJECTS";
const std::string CHANNEL_NAME_PERCEPTION_OBSTACLES="PERCEPTION_OBSTACLES";
const std::string CHANNEL_NAME_PERCEPTION_GRID_MAPPING="PERCEPTION_GRID_MAPPING";
const std::string CHANNEL_NAME_PERCEPTION_HPP="PERCEPTION_HPP";
const std::string CHANNEL_NAME_PERCEPTION_TSR="PERCEPTION_TSR";
const std::string CHANNEL_NAME_PERCEPTION_LINES="PERCEPTION_LINES";
const std::string CHANNEL_NAME_IBEO_SELECTED_OBJECTS="IBEO_SELECTED_OBJECTS";
const std::string CHANNEL_NAME_TRACKED_OBJECTS="TRACKED_OBJECTS";
const std::string CHANNEL_NAME_MWR_CLUSTED_OBJECTS="MWR_CLUSTED_OBJECTS";
//=======================================================================================
//Prediction related
//=======================================================================================
const std::string CHANNEL_NAME_PREDICTION_OBSTACLES="PREDICTION_OBSTACLES";
const std::string CHANNEL_NAME_PREDICTION_INFOS="PREDICTION_INFOS";

//=======================================================================================
//HD Map related
//=======================================================================================

const std::string CHANNEL_NAME_DRIVER_HDMAP="DRIVER_HDMAP";
const std::string CHANNEL_NAME_DRIVER_MAP="DRIVER_MAP";
const std::string CHANNEL_NAME_DRIVER_MAP_LANE="DRIVER_MAP_LANE";
const std::string CHANNEL_NAME_DRIVER_MAP_TSR_DISTANCE = "DRIVER_MAP_TSR_DISTANCE";
const std::string CHANNEL_NAME_DRIVER_MAP_CHAIN_FURNITURE = "DRIVER_CHAIN_FURNITURE";


//surroundViewCamera related
//=======================================================================================
const std::string CHANNEL_NAME_SURROUNDVIEWCAMERA_INFO="SURROUNDVIEWCAMERA_INFO";
const std::string CHANNEL_NAME_OFILM_CTRL="OFILM_CTRL";
const std::string CHANNEL_NAME_OFILM_STATUS = "OFILM_STATUS";
const std::string CHANNEL_NAME_OFILM_OBJECTS = "OFILM_OBJECTS";
//=======================================================================================

//Freesapce
//=======================================================================================
const std::string CHANNEL_NAME_MWR_FREESPACE="MWR_FREESPACE";
const std::string CHANNEL_NAME_PERCEPTION_FREESPACE="PERCEPTION_FREESPACE";
const std::string CHANNEL_NAME_RECORD_FREESPACE = "RECORD_FREESPACE";
const std::string CHANNEL_NAME_SPARE_GRID_MAP = "SPARE_GRID_MAP";
//=======================================================================================

const std::string CHANNEL_NAME_GLOBAL_PARKLOTS = "GLOBAL_PARKLOTS";
const std::string CHANNEL_NAME_GLOBAL_ROAD_EDGES = "GLOBAL_ROAD_EDGES";
const std::string CHANNEL_NAME_MATCH_INS_POINT = "MATCH_INS_POINT";


enum COORINDATE_FRMAE_ID
{
    CFI_UNKONWN,
    CFI_WORLD,
    CFI_VEHICLE,
    CFI_VELODYNE64,
    CFI_MOBILEYE,    
    CFI_FRONT_MWR,
    CFI_LEFT_MWR,
    CFI_RIGHT_MWR,
    CFI_PIKE,
    CFI_LUX_FRONT,
    CFI_LUX_LEFT,
    CFI_LUX_BACK,
    CFI_LUX_RIGHT,
    CFI_IBEO,
    CFI_WORLD_VCUDR,
    CFI_SURROUNDVIEWCAMERA,
    CFI_OFILMCTRL,
    CFI_OFILMPARKINGSEARCH,
    CFI_OFILMSTATUS,
    CFI_FUSEDPARKINGLOT,
    CFI_ULTRASONIC_DISTS,
    CFI_MWR_FRONT_ESR,
    CFI_MWR_FRONT_LEFT_RSRS,
    CFI_MWR_FRONT_RIGHT_RSDS,
    CFI_MWR_REAR_LEFT_SRR,
    CFI_MWR_REAR_RIGHT_SRR,
    CFI_MWR_REAR_ESR
};

/*header example
//initialize example:
m_VelodyneFrame.header.nTimeStamp = GetGlobalTimeStampInMicroSec();
m_VelodyneFrame.header.nCounter = 0;
m_VelodyneFrame.header.nFrameID = CFI_VELODYNE64;//velodyne frame;
//publish example:
m_VelodyneFrame.header.nCounter = (static_cast<unsigned int>(m_VelodyneFrame.header.nCounter)+1)%65535;
m_VelodyneFrame.header.nTimeStamp = GetGlobalTimeStampInMicroSec();
//publish message example;
VELODYNE64_FRAME m_VelodyneFrame;
lcm::LCM* m_pLCM;
m_pLCM->publish(CHANNEL_NAME_VELODYNE64, &m_VelodyneFrame);
*/
#endif
