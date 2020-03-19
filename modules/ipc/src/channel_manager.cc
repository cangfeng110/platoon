#include "modules/ipc/include/channel_manager.h"


#include <thread>

namespace atd
{
namespace ipc
{

ChannelManager::ChannelManager() {}

bool ChannelManager::PROTO_Initialized()
{
    return instance()->proto_initialized_;
}

bool ChannelManager::LCM_Initialized()
{
    return instance()->lcm_initialized_;
}

void ChannelManager::Reset()
{
    instance()->lcm_initialized_ = false;
    instance()->proto_initialized_ = false;
    instance()->lcm_container_recv.clear();
    instance()->proto_container_recv.clear();
}

Status ChannelManager::Init(const std::string &channel_config_filename)
{


    ChannelManagerConfig configs;

    

    return Status(atd::common::ErrorCode::OK, "none");
}


//bopeng for xml-route==========================
Status ChannelManager::Init_isxmlroute(const std::string &channel_config_filename){

    /*using namespace tinyxml2;
    XMLDocument tDocXmlConfig;
    XMLError err = tDocXmlConfig.LoadFile(channel_config_filename.c_str());
    if (err != XML_SUCCESS) {
        return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR,
                      atd::common::util::StrCat("Unable to parse config xml file:", channel_config_filename));
    }

    XMLElement *ele = nullptr;
    bool tmp_is_xml_route;
    ele = tDocXmlConfig.FirstChildElement("is_xml_route");
    err = ele->QueryBoolText(&tmp_is_xml_route);
    if (err != XML_SUCCESS) {
        return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR, "xml config parse error, without value `is_xml_route`");
    }
    ChannelManager::set_is_xml_route(tmp_is_xml_route);*/

    return Status(atd::common::ErrorCode::OK, "none");
}
//bopeng for xml-route==========================

//bopeng for gridlog==========================
Status ChannelManager::Init_Gridlog(const std::string &channel_config_filename){

  

    return Status(atd::common::ErrorCode::OK, "none");
}

Status ChannelManager::Init(const ChannelManagerConfig &configs)
{

    if (configs.is_lcm())
    {
        if (LCM_Initialized())
        {
            return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR, "fail to init LCM");
        }
        instance()->lcm_initialized_ = true;
        for (const auto &config : configs.config())
        {

            switch (config.type())
            {
            case ChannelConfig::LCM_Vcu:
                EnableVcu("VCU_VEHICLE_INFO", 30, config);
                break;
            case ChannelConfig::LCM_RawIns:
                EnableRawIns("RAW_INS", 30, config);
                break;
            case ChannelConfig::LCM_RawIns300C:
                EnableRawIns300C("INS300C58", 30, config);
                break;
            case ChannelConfig::LCM_OWNSINS:
                EnableOWNSINS("OWNSINS", 30, config);
                break;
            case ChannelConfig::LCM_MELaneLines:
                EnableMELaneLines("MOBILEYE_LANES", 30, config);
                break;
            case ChannelConfig::LCM_Ultrasonic:
                EnableUSDists("US_DISTS", 30, config);
                break;
//              case ChannelConfig::LCM_USDistance:
//                  EnableUSDistance("USDistance", 30, config);
//                  break;
            case ChannelConfig::LCM_LogitechCamera:
                EnableLogitechCamera("LOGITECH_IMAGE", 30, config);
                break;
            case ChannelConfig::LCM_OFILMloc:
                EnableOFILMloc("OFILM_LOCATION", 30, config);
                break;
            case ChannelConfig::LCM_MomentaLoc:
                EnableMomentaLoc("VIEWLOC", 30, config);
                break;
            case ChannelConfig::LCM_Uwb:
                EnableUwb("UWBDATA_MSG", 30, config);
                break;
            case ChannelConfig::LCM_MapRDisOnePark:
                EnableMapRDisOnePark("DRIVER_MAP_ONE_PARKING_SITE", 30, config);
                break;
            case ChannelConfig::LCM_MapRDisTsr:
                EnableMapRDisTsr("DRIVER_MAP_TSR_DISTANCE", 30, config);
                break;
            case ChannelConfig::LCM_MapTargetLanes:
                EnableMapTargetLanes("DRIVER_MAP_LANE", 30, config);
                break;
            case ChannelConfig::LCM_NaviRoute:
                EnableNaviRoute("ADM_RPPOINT_LIST", 30, config);
                break;
            case ChannelConfig::LCM_ProxyRequest:
                EnableProxyRequest("APP_PROXY_REQUEST", 30, config);
                break;
            case ChannelConfig::LCM_OFILMParkingSearch:
                EnableOFILMParkingSearch("OFILM_PARKINGSEARCH", 1000, config);
                break;
            case ChannelConfig::LCM_Coupleout:
                EnableCoupleOut("COUPLE_LOCOLIZATION", 30, config);
                break;
            case ChannelConfig::LCM_EKFOut:
                EnableEKFOut("EKF_OUTDATA", 30, config);
                break;
            case ChannelConfig::LCM_AdpStatus:
                EnableAdpStatus("EXECUTE_BEHAVIOR_PLAN", 30, config);
                break;
            case ChannelConfig::LCM_MEObjects:
                EnableMEObjects(CHANNEL_NAME_MOBILEYE_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_MEFreeSpace:
                EnableMEFreeSpace(CHANNEL_NAME_MOBILEYE_FREESPACE, 30, config);
                break;
            case ChannelConfig::LCM_PerceptionLines:
                EnableFusionPerceptionLane(CHANNEL_NAME_PERCEPTION_LINES, 30, config);
                break;
            case ChannelConfig::LCM_BoschObjects:
                EnableMWRBoschObjects(CHANNEL_NAME_BOSCH_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_RDAObjects:
                EnableMWRRawObjectsRDA(CHANNEL_NAME_RDA_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_FSDAObjects:
                EnableMWRRawObjectsFSDA(CHANNEL_NAME_FSDA_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_PerceptionTSR:
                EnablePerceptionTSR(CHANNEL_NAME_PERCEPTION_TSR, 30, config);
                break;
            case ChannelConfig::LCM_SV51FrontMwr:
                EnableMWRRawObjectsSV51Front(CHANNEL_NAME_SV51Front_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_SV51FrontLeftMwr:
                EnableMWRRawObjectsSV51FrontLeft(CHANNEL_NAME_SV51FrontLeft_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_SV51FrontRightMwr:
                EnableMWRRawObjectsSV51FrontRight(CHANNEL_NAME_SV51FrontRight_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_SV51RearLeftMwr:
                EnableMWRRawObjectsSV51RearLeft(CHANNEL_NAME_SV51RearLeft_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_SV51RearRightMwr:
                EnableMWRRawObjectsSV51RearRight(CHANNEL_NAME_SV51RearRight_OBJECTS, 30, config);
                break;
            case ChannelConfig::LCM_PerceptionObjects:
                EnablePerceptionObstacles(CHANNEL_NAME_PERCEPTION_OBSTACLES, 30, config);
                break;
            case ChannelConfig::LCM_TRACKED_OBJECTS:
                EnableTrackedObjects(CHANNEL_NAME_TRACKED_OBJECTS, 30, config);
                break;
            }
        }
    }
    else
    {
        if (PROTO_Initialized())
        {
            return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR, "fail to init PROTO");
        }
        instance()->proto_initialized_ = true;
        for (const auto &config : configs.config())
        {
            switch (config.type())
            {
            case ChannelConfig::FixMsg:
                EnableFixMsg("FixMsg", config, 400);
                break;
            case ChannelConfig::DelimitedMsg:
                EnableDelimitedMsg("DelimitedMsg", config, 500);
                break;
            case ChannelConfig::Monitor:
                EnableMonitor("Monitor", config, 5000);
                break;
            case ChannelConfig::DrList:
                EnableDrList("DrList", config, 5000);
                break;
            case ChannelConfig::DrInsList:
                EnableDrInsList("DrInsList", config, 5000);
                break;
            case ChannelConfig::Pfs:
                EnablePfs("Pfs", config, 5000);
                break;
            case ChannelConfig::PfsAfterMatch:
                EnablePfsAfterMatch("PfsAfterMatch", config, 3000);
                break;
            case ChannelConfig::GridList:
                EnableGridList("GridList", config, 5000);
                break;
            case ChannelConfig::GridListMatchDisplay:
                EnableGridListMatchDisplay("GridListMatchDisplay", config, 30000);
                break;
            case ChannelConfig::Localization:
                EnableLocalization("Localization", config, 400);
                break;
            case ChannelConfig::LocalizationAbs:
                EnableLocalizationAbs("LocalizationAbs", config, 500);
                break;
            case ChannelConfig::DriverIns:
                EnableDriverIns("DriverIns", config, 1000);
                break;
            case ChannelConfig::DriverUwb:
                EnableDriverUwb("DriverUwb", config, 1000);
                break;
            case ChannelConfig::DriverVcu:
                EnableDriverVcu("DriverVcu", config, 1000);
                break;
            case ChannelConfig::Drivermomentaloc:
                EnableDrivermomentaloc("Drivermomentaloc", config, 1000);
                break;
            case ChannelConfig::DriverUltrasonic:
                EnableDriverUltrasonic("DriverUltrasonic", config, 1000);
                break;
            case ChannelConfig::DriverMobileyeLane:
                EnableDriverMobileyeLane("DriverMobileyeLane", config, 5000);
                break;
            case ChannelConfig::LocalizationMapMatch:
                EnableLocalizationMapMatch("LocalizationMapMatch", config, 3000);
                break;
            case ChannelConfig::EngineLocMap:
                EnableEngineLocMap("EngineLocMap", config, 4000);
                break;
            case ChannelConfig::EngineTargetLanes:
                EnableEngineTargetLanes("EngineTargetLanes", config, 25000);
                break;
            case ChannelConfig::EngineTargetArrows:
                EnableEngineTargetArrows("EngineTargetArrows", config, 25000);
                break;
            case ChannelConfig::EngineRemainDistance:
                EnableEngineRemainDistance("EngineRemainDistance", config, 3000);
                break;
            case ChannelConfig::EngineSurMap:
                EnableEngineSurMap("EngineSurMap", config, 25000);
                break;
            case ChannelConfig::EngineLocMapBuffer:
                EnableEngineLocMapBuffer("EngineLocMapBuffer", config, 4000);
                break;
            case ChannelConfig::EngineTargetLanesBuffer:
                EnableEngineTargetLanesBuffer("EngineTargetLanesBuffer", config, 25000);
                break;
            case ChannelConfig::EngineTargetArrowsBuffer:
                EnableEngineTargetArrowsBuffer("EngineTargetArrowsBuffer", config, 25000);
                break;
            case ChannelConfig::EngineRemainDistanceBuffer:
                EnableEngineRemainDistanceBuffer("EngineRemainDistanceBuffer", config, 25000);
                break;
            case ChannelConfig::EngineSurMapBuffer:
                EnableEngineSurMapBuffer("EngineSurMapBuffer", config, 25000);
                break;
            case ChannelConfig::EngineDestination:
                EnableEngineDestination("EngineDestination", config, 1000);
                break;
            case ChannelConfig::GridLog:
                EnableGridLog("GridLogInfo",config,1000);
                break;
            case ChannelConfig::ConstructionArea:
                EnableConstructionArea("ConstructionArea", config, 2000);
                break;
            case ChannelConfig::Lane_Support:
                EnableLane_Support("Lane_Support", config, 2000);
                break;
            case ChannelConfig::LS_ADV:
                EnableLS_ADV("LS_ADV", config, 2000);
                break;
            case ChannelConfig::MeFreeSpace:
                EnableMeFreeSpace("MeFreeSpace", config, 2000);
                break;
            case ChannelConfig::MeObjects:
                EnableMeObjects("MeObjects", config, 8000);
                break;
            case ChannelConfig::SematicLine:
                EnableSematicLine("SematicLine", config, 2000);
                break;
            case ChannelConfig::SematicMark:
                EnableSematicMark("SematicMark", config, 2000);
                break;
            case ChannelConfig::SL_Descrip:
                EnableSL_Descrip("SL_Descrip", config, 2000);
                break;
            case ChannelConfig::TFL:
                EnableTFL("TFL", config, 2000);
                break;
            case ChannelConfig::TSR:
                EnableTSR("TSR", config, 2000);
                break;
            case ChannelConfig::MeHLB:
                EnableMeHLB("MeHLB", config, 2000);
                break;
            case ChannelConfig::MeLDW:
                EnableMeLDW("MeLDW", config, 2000);
                break;
            case ChannelConfig::MeApplication:
                EnableMeApplication("MeApplication", config, 2000);
                break;
            case ChannelConfig::MeCommon:
                EnableMeCommon("MeCommon", config, 2000);
                break;
            case ChannelConfig::FCFVD:
                EnableFCFVD("FCFVD", config, 2000);
                break;
            case ChannelConfig::FCF_VRU:
                EnableFCF_VRU("FCF_VRU", config, 2000);
                break;
            case ChannelConfig::FailSafe:
                EnableFailSafe("FailSafe", config, 2000);
                break;
            }
        }
    }
    return Status(atd::common::ErrorCode::OK, "none");
}

void ChannelManager::inner_join_cpi_thread()
{
    if(LCM_Initialized())
    {
        int num = lcm_container_recv.size();
        std::thread threads[num];// default-constructed threads

        for (int i = 0; i < num; ++i)
            threads[i] = std::thread(&CLCMSubscriber::run, lcm_container_recv[i]);   // move-assign threads

        for (int i = 0; i < num; ++i)
            threads[i].detach();
    }

    if(PROTO_Initialized())
    {
        int num1 = proto_container_recv.size();
        std::thread threads1[num1];// default-constructed threads

        for (int i = 0; i < num1; ++i)
            threads1[i] = std::thread(&CPROTOSubscriber::run, proto_container_recv[i]);   // move-assign threads

        for (int i = 0; i < num1; ++i)
            threads1[i].detach();
    }
    //sleep for waiting the join finish;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

}  // namespace ipc
}  // namespace atd
