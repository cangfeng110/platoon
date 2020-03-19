#ifndef MODULES_WORLDMODEL_VEHICLE_STATE_DR_STATE_PROVIDER_H_
#define MODULES_WORLDMODEL_VEHICLE_STATE_DR_STATE_PROVIDER_H_

#include <memory>
#include <string>

#include "modules/common/include/macro.h"
#include "modules/common/include/status/status.h"
#include "modules/worldmodel/include/math/vec2d.h"

#include "modules/ipc/message/protobuf/dr/dr.pb.h"

/**
 * @namespace atd::worldmodel
 * @brief atd::worldmodel
 */
namespace atd {
namespace worldmodel {

using atd::dr::DrStep;
using atd::dr::DrList;
using atd::dr::DrStep_ins;
using atd::common::Status;

class DrStateProvider {
public:

    virtual ~DrStateProvider() = default;

    bool DrTimeSync(const double input_sensor_timestamp, const dr::DrList &drlistmsg, dr::DrStep &result);
    Status DrTimeSync_ins(const double input_sensor_timestamp, const dr::DrList &drlistmsg, dr::DrStep_ins &result);
    void DrFuturePosition() const;

private:

    DECLARE_SINGLETON(DrStateProvider);
};

}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_VEHICLE_STATE_DR_STATE_PROVIDER_H_
