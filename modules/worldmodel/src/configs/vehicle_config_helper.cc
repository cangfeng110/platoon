#include "modules/worldmodel/include/configs/vehicle_config_helper.h"

#include <algorithm>
#include <cmath>

#include "modules/common/include/util/file.h"

namespace atd {
namespace worldmodel {

using atd::common::VehicleConfig;

VehicleConfig VehicleConfigHelper::vehicle_config_;
bool VehicleConfigHelper::is_init_ = false;

VehicleConfigHelper::VehicleConfigHelper() {
}

bool VehicleConfigHelper::Init() {
    return Init("some url");
}

bool VehicleConfigHelper::Init(const std::string &config_file) {
//    VehicleConfig params;
//    if(atd::common::util::GetProtoFromFile(config_file, &params))
//    {
//        Init(params);
//        return true;
//    }
//    else
//    {
//        // "Unable to parse adapter config file " << config_file;
//        return false;
//    }
}

void VehicleConfigHelper::Init(const VehicleConfig &vehicle_params) {
    vehicle_config_ = vehicle_params;
    is_init_ = true;
}

const VehicleConfig &VehicleConfigHelper::GetConfig() {
    if (!is_init_) {
        Init();
    }
    return vehicle_config_;
}

double VehicleConfigHelper::MinSafeTurnRadius() {
    const auto &param = vehicle_config_.vehicle_param();
    double lat_edge_to_center =
            std::max(param.left_edge_to_center(), param.right_edge_to_center());
    double lon_edge_to_center =
            std::max(param.front_edge_to_center(), param.back_edge_to_center());
    return std::sqrt((lat_edge_to_center + param.min_turn_radius()) *
                     (lat_edge_to_center + param.min_turn_radius()) +
                     lon_edge_to_center * lon_edge_to_center);
}

}  // namespace worldmodel
}  // namespace atd
