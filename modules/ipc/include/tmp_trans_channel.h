#ifndef MODULES_IPC_PROTOBUF_TMP_TRANS_CHANNEL_H_
#define MODULES_IPC_PROTOBUF_TMP_TRANS_CHANNEL_H_

#include "modules/ipc/include/message_channel.h"
#include "modules/ipc/include/channel_manager.h"
#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"

namespace atd {
namespace ipc {

void LCM2PROTO_RawInsLcm_TO_DriverInsProto(RawInsLcm &ins_msg);
void LCM2PROTO_RawIns300CLcm_TO_DriverInsProto(RawIns300CLcm &ins_msg);
void LCM2PROTO_RawUwbLcm_TO_DriverUwbProto(UwbLcm &uwb_msg);
void LCM2PROTO_VcuLcm_To_DriverVcuProto(VcuLcm &vcu_msg);
void LCM2PROTO_MomentaLocLcm_TO_Drivermomentaloc(MomentaLocLcm &momenta_msg);
void LCM2PROTO_NaviRoute_TO_EngineDestinationProto(NaviRouteLcm &debug_msg);
void LCM2PROTO_AppRoute_TO_EngineDestinationProto(ProxyRequestLcm &input_proxyrequest);

void LCM2PROTO_USDIST_TO_Perception_ULTRASONICProto(atd::ipc::USDistsLcm &usd_msg);
void LCM2PROTO_MELANE_TO_Perception_MOBILANESProto(atd::ipc::MELaneLinesLcm &me_msg);
void GenerateProtoLane(MOBILEYE_LANE_LINE &line,atd::drivers::MOBILEYE_LANE_LINE* tmpMOBILEYE_LANE_LINE);

bool LCM2PROTO_NaviRoute_TO_EngineDestinationProtoNew(NaviRouteLcm &debug_msg);
bool LCM2PROTO_AppRoute_TO_EngineDestinationProtoNew(ProxyRequestLcm &input_proxyrequest);

}  // namespace ipc
}  // namespace atd

#endif  // MODULES_IPC_PROTOBUF_TMP_TRANS_CHANNEL_H_
