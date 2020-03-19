#ifndef MODULES_IPC_PROTOBUF_CHANNEL_MANAGER_MANAGER_H_
#define MODULES_IPC_PROTOBUF_CHANNEL_MANAGER_MANAGER_H_

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "modules/common/include/macro.h"
#include "modules/common/include/status/status.h"

#include "modules/ipc/include/protobuf/adapter_once.h"
#include "modules/ipc/include/lcm/clcmsubscriber.h"

#include "modules/ipc/include/message_channel.h"
#include "modules/ipc/include/channel_manager_macro.h"
#include "modules/ipc/include/tmp_trans_channel.h"
#include "modules/ipc/conf/channel_config.pb.h"

#include "modules/common/include/util/util.h"
#include "modules/common/include/util/file.h"
#include "modules/common/include/util/3rd/tinyxml2.h"
#include "modules/ipc/include/lcm/lcm_channel.h"

/**
 * @namespace atd::ipc
 * @brief atd::ipc
 */
namespace atd {
namespace ipc {

using atd::common::Status;
using adapter::AdapterOnce;

/**
 * @class ChannelManager
 *
 * @brief this class hosts all the specific adapters and manages them.
 * It provides APIs for the users to initialize, access and interact
 * with the adapters that they are interested in.
 *
 * \par
 * Each (potentially) useful adapter needs to be registered here with
 * the macro REGISTER_ADAPTER.
 *
 * \par
 * The ChannelManager is a singleton.
 */
class ChannelManager {
public:
    /**
   * @brief Initialize the /class ChannelManager singleton with the
   * provided configuration. The configuration is specified by the
   * file path.
   * @param adapter_config_filename the path to the proto file that
   * contains the adapter manager configuration.
   */
    static Status Init(const std::string &adapter_config_filename);

    /**
   * @brief Initialize the /class ChannelManager singleton with the
   * provided configuration.
   * @param configs the adapter manager configuration proto.
   */
    static Status Init(const ChannelManagerConfig &configs);

    /**
   * @brief Resets the /class ChannelManager so that it could be
   * re-initiailized.
   */
    static void Reset();
    /**
   * @brief check if the ChannelManager is initialized
   */
    static bool LCM_Initialized();
    /**
   * @brief check if the ChannelManager is initialized
   */
    static bool PROTO_Initialized();

    /**
   * @brief Join all the channel threads
   */
    static void join_cpi_thread() {
        instance()->inner_join_cpi_thread();
    }

    static bool get_is_debugmsg_send() {
        return instance()->is_debugmsg_send;
    }

    static bool get_is_monitor_send() {
        return instance()->is_monitor_send;
    }

    static Status Init_isxmlroute(const std::string &adapter_config_filename);

    static bool get_is_xml_route(){
        return instance()->is_xml_route;
    }

    static void set_is_debugmsg_send(bool is_debugmsg_send_) {
        instance()->is_debugmsg_send=is_debugmsg_send_;
    }

    static void set_is_monitor_send(bool is_monitor_send_) {
        instance()->is_monitor_send=is_monitor_send_;
    }

    //bopeng
    static void set_is_xml_route(bool is_xml_route_){
        instance()->is_xml_route = is_xml_route_;
    }
    //bopeng for xml-route===================================

    //bopeng for gridlog=====================================
    static Status Init_Gridlog(const std::string &adapter_config_filename);

    static bool get_has_map(){
        return instance()->has_map;
    }

    static void set_has_map(bool has_map_){
        instance()->has_map = has_map_;
    }
    //bopeng for gridlog===================================


private:
    void inner_join_cpi_thread();

    std::vector<ipc::CPROTOSubscriber*> proto_container_recv;
    bool proto_initialized_ = false;

    std::vector<ipc::CLCMSubscriber*> lcm_container_recv;
    bool lcm_initialized_ = false;

    bool is_debugmsg_send;
    bool is_monitor_send;
    //bopeng
    bool is_xml_route;

    //bopeng for gridlog
    bool has_map;

    /// The following code registered all the adapters of interest.
    REGISTER_CHANNEL_PROTO(EngineLocMap);
    REGISTER_CHANNEL_PROTO(EngineTargetLanes);
    REGISTER_CHANNEL_PROTO(EngineTargetArrows);
    REGISTER_CHANNEL_PROTO(EngineRemainDistance);
    REGISTER_CHANNEL_PROTO(EngineSurMap);
    REGISTER_CHANNEL_PROTO(EngineLocMapBuffer);
    REGISTER_CHANNEL_PROTO(EngineTargetLanesBuffer);
    REGISTER_CHANNEL_PROTO(EngineTargetArrowsBuffer);
    REGISTER_CHANNEL_PROTO(EngineRemainDistanceBuffer);
    REGISTER_CHANNEL_PROTO(EngineSurMapBuffer);
    REGISTER_CHANNEL_PROTO(EngineDestination);

    //GRIDLOG
    REGISTER_CHANNEL_PROTO(GridLog)


    REGISTER_CHANNEL_PROTO(FixMsg);
    REGISTER_CHANNEL_PROTO(DrList);
    REGISTER_CHANNEL_PROTO(DrInsList);
    REGISTER_CHANNEL_PROTO(Pfs);
    REGISTER_CHANNEL_PROTO(PfsAfterMatch);
    REGISTER_CHANNEL_PROTO(GridList);
    REGISTER_CHANNEL_PROTO(GridListMatchDisplay);
    REGISTER_CHANNEL_PROTO(DriverIns);
    REGISTER_CHANNEL_PROTO(DriverUwb);
    REGISTER_CHANNEL_PROTO(DriverVcu);
    REGISTER_CHANNEL_PROTO(Drivermomentaloc);
    REGISTER_CHANNEL_PROTO(DriverUltrasonic);
    REGISTER_CHANNEL_PROTO(DriverMobileyeLane);
    REGISTER_CHANNEL_PROTO(Localization);
    REGISTER_CHANNEL_PROTO(LocalizationAbs);
    REGISTER_CHANNEL_PROTO(LocalizationMapMatch);
    //REGISTER_CHANNEL_PROTO(SystemStatus);
    REGISTER_CHANNEL_PROTO(Monitor);
    REGISTER_CHANNEL_PROTO(DelimitedMsg);

    // EyeQ4 related messages
    REGISTER_CHANNEL_PROTO(ConstructionArea);
    REGISTER_CHANNEL_PROTO(Lane_Support);
    REGISTER_CHANNEL_PROTO(LS_ADV);
    REGISTER_CHANNEL_PROTO(LS_Road_Edge);
    REGISTER_CHANNEL_PROTO(MeFreeSpace);
    REGISTER_CHANNEL_PROTO(MeObjects);
    REGISTER_CHANNEL_PROTO(SematicLine);
    REGISTER_CHANNEL_PROTO(SematicMark);
    REGISTER_CHANNEL_PROTO(SL_Descrip);
    REGISTER_CHANNEL_PROTO(TFL);
    REGISTER_CHANNEL_PROTO(TSR);
    REGISTER_CHANNEL_PROTO(MeHLB);
    REGISTER_CHANNEL_PROTO(MeLDW);
    REGISTER_CHANNEL_PROTO(MeApplication);
    REGISTER_CHANNEL_PROTO(MeCommon);
    REGISTER_CHANNEL_PROTO(FCFVD);
    REGISTER_CHANNEL_PROTO(FCF_VRU);
    REGISTER_CHANNEL_PROTO(FailSafe);


    REGISTER_CHANNEL_LCM(Vcu);
    REGISTER_CHANNEL_LCM(RawIns);
    REGISTER_CHANNEL_LCM(RawIns300C);
    REGISTER_CHANNEL_LCM(OWNSINS);
    REGISTER_CHANNEL_LCM(OFILMloc);
    REGISTER_CHANNEL_LCM(MomentaLoc);
    REGISTER_CHANNEL_LCM(Uwb);
    REGISTER_CHANNEL_LCM(MapRDisTsr);
    REGISTER_CHANNEL_LCM(MapRDisOnePark);
    REGISTER_CHANNEL_LCM(MapTargetLanes);
    REGISTER_CHANNEL_LCM(NaviRoute);
    REGISTER_CHANNEL_LCM(ProxyRequest);
    REGISTER_CHANNEL_LCM(CoupleOut);
    REGISTER_CHANNEL_LCM(AdpStatus);
	REGISTER_CHANNEL_LCM(EKFOut);
	REGISTER_CHANNEL_LCM(OFILMParkingSearch);
    REGISTER_CHANNEL_LCM(MEFreeSpace);
    REGISTER_CHANNEL_LCM(MEObjects);

    REGISTER_CHANNEL_LCM(MWRRawObjectsFSDA);
    REGISTER_CHANNEL_LCM(MWRRawObjectsRDA);

    REGISTER_CHANNEL_LCM(FusionPerceptionLane);
    REGISTER_CHANNEL_LCM(MWRRawObjectsSV51Front);
    REGISTER_CHANNEL_LCM(MWRRawObjectsSV51FrontLeft);
    REGISTER_CHANNEL_LCM(MWRRawObjectsSV51FrontRight);
    REGISTER_CHANNEL_LCM(MWRRawObjectsSV51RearLeft);
    REGISTER_CHANNEL_LCM(MWRRawObjectsSV51RearRight);

    REGISTER_CHANNEL_LCM(MWRBoschObjects);
    REGISTER_CHANNEL_LCM(PerceptionTSR);

    REGISTER_CHANNEL_LCM(PerceptionObstacles);
    REGISTER_CHANNEL_LCM(TrackedObjects);

//    REGISTER_CHANNEL_LCM(MapTargetLanes);



    //------ MobileyeLanes ------
    REGISTER_CHANNEL_LCM(LogitechCamera)  // receive
    REGISTER_CHANNEL_LCM(MELaneLines)  // internal lcm send
    //------ MobileyeLanes ------

    //------ US_DISTANCE ------
    REGISTER_CHANNEL_LCM(USDists); //receive
    REGISTER_CHANNEL_LCM(USDistance); // internal lcm send
    //------ US_DISTANCE ------

    DECLARE_SINGLETON(ChannelManager);
};

}  // namespace ipc
}  // namespace atd

#endif
