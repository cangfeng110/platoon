#ifndef MODULES_WORLDMODEL_VEHICLE_STATE_LOC_ESTIMATER_H_
#define MODULES_WORLDMODEL_VEHICLE_STATE_LOC_ESTIMATER_H_

#include <memory>
#include <string>

#include "modules/common/include/macro.h"
#include "modules/common/include/status/status.h"
#include "modules/worldmodel/include/math/arrow2d.h"

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
using atd::worldmodel::math::Arrow2d;


class LocEstimater {
public:

    virtual ~LocEstimater() = default;

    bool LocSync_ins(const double input_loc_timestamp, const Arrow2d &input_loc, const dr::DrList &drlistmsg, math::Arrow2d &per_loc, Arrow2d &cur_loc);

private:

    DECLARE_SINGLETON(LocEstimater);
};

}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_VEHICLE_STATE_LOC_ESTIMATER_H_
