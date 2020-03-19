#ifndef MODULES_WORLDMODEL_TF_EGO_FRAME_CDEF_ECARTESIAN_CONVERSION_H_
#define MODULES_WORLDMODEL_TF_EGO_FRAME_CDEF_ECARTESIAN_CONVERSION_H_

#include <string>
#include <cmath>
#include "modules/worldmodel/include/math/vec2d.h"
#include "modules/worldmodel/include/math/arrow2d.h"
#include "modules/common/include/macro.h"

namespace atd {
namespace worldmodel  {
namespace tf {

class CdefEcartesianConverter {
public:
    static void set_vector(const double delta_x, const double delta_y, const double delta_yaw);
    static void PEC2CEC_Point(const math::Vec2d ori_ec_xy, math::Vec2d &dst_ec_xy);
    static void PEC2CEC_Vector(const math::Arrow2d ori_arrow, math::Arrow2d &dst_arrow);

    DECLARE_SINGLETON(CdefEcartesianConverter);


private:
    void inter_set_vector(const double delta_x, const double delta_y, const double delta_yaw);

    void inter_pec2cec(const math::Vec2d ori_ec_xy, math::Vec2d &dst_ec_xy);

    void inter_pec2cec_vector(const math::Arrow2d ori_arrow, math::Arrow2d &dst_arrow);

    double delta_x_;
    double delta_y_;
    double delta_yaw_;

};

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_TF_EGO_FRAME_CDEF_ECARTESIAN_CONVERSION_H_
