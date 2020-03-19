#include "modules/worldmodel/include/tf/EgoFrame/cdef_ecartesian_conversion.h"

namespace atd {
namespace worldmodel  {
namespace tf {

CdefEcartesianConverter::CdefEcartesianConverter() {}

void CdefEcartesianConverter::set_vector(const double delta_x,const double delta_y, const double delta_yaw)
{
    CdefEcartesianConverter::instance()->inter_set_vector(delta_x,delta_y,delta_yaw);
}

void CdefEcartesianConverter::PEC2CEC_Point(const math::Vec2d ori_ec_xy, math::Vec2d &dst_ec_xy)
{
    CdefEcartesianConverter::instance()->inter_pec2cec(ori_ec_xy,dst_ec_xy);
}

void CdefEcartesianConverter::PEC2CEC_Vector(const math::Arrow2d ori_arrow, math::Arrow2d &dst_arrow)
{
    CdefEcartesianConverter::instance()->inter_pec2cec_vector(ori_arrow,dst_arrow);
}

void CdefEcartesianConverter::inter_set_vector(const double delta_x, const double delta_y, const double delta_yaw)
{
    delta_x_=delta_x;
    delta_y_=delta_y;
    delta_yaw_=delta_yaw;
}

void CdefEcartesianConverter::inter_pec2cec(const math::Vec2d ori_ec_xy, math::Vec2d &dst_ec_xy)
{
    double ax=ori_ec_xy.x();
    double ay=ori_ec_xy.y();
    double ar = atan2(-ay, ax);

    double r = sqrt(ax * ax + ay * ay);
    double adx = r * cos(ar) * cos(delta_yaw_) + r * sin(ar) * sin(delta_yaw_);
    double ady = r * sin(ar) * cos(delta_yaw_) - r * cos(ar) * sin(delta_yaw_);
    double dx = delta_x_ + adx;
    double dy = delta_y_ - ady;
    dst_ec_xy.set_x(dx);
    dst_ec_xy.set_y(dy);
}

void CdefEcartesianConverter::inter_pec2cec_vector(const math::Arrow2d ori_arrow, math::Arrow2d &dst_arrow)
{
    double ax=ori_arrow.center_x();
    double ay=ori_arrow.center_y();
    double ar = atan2(-ay, ax);

    double r = sqrt(ax * ax + ay * ay);
    double adx = r * cos(ar) * cos(delta_yaw_) + r * sin(ar) * sin(delta_yaw_);
    double ady = r * sin(ar) * cos(delta_yaw_) - r * cos(ar) * sin(delta_yaw_);
    double dx = delta_x_ + adx;
    double dy = delta_y_ - ady;
    dst_arrow.set_center_x(dx);
    dst_arrow.set_center_y(dy);
    dst_arrow.set_heading(delta_yaw_+ori_arrow.heading());
}

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd
