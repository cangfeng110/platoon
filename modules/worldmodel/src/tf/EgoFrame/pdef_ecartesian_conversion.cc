#include "modules/worldmodel/include/tf/EgoFrame/pdef_ecartesian_conversion.h"

namespace atd {
namespace worldmodel  {
namespace tf {

PdefEcartesianConverter::PdefEcartesianConverter() {}

void PdefEcartesianConverter::set_vector(const double delta_x,const double delta_y, const double delta_yaw)
{
    PdefEcartesianConverter::instance()->inter_set_vector(delta_x,delta_y,delta_yaw);
}

void PdefEcartesianConverter::PEC2CEC_Point(const math::Vec2d ori_ec_xy, math::Vec2d &dst_ec_xy)
{
    PdefEcartesianConverter::instance()->inter_pec2cec(ori_ec_xy,dst_ec_xy);
}

void PdefEcartesianConverter::PEC2CEC_Vector(const math::Arrow2d ori_arrow, math::Arrow2d &dst_arrow)
{
    PdefEcartesianConverter::instance()->inter_pec2cec_vector(ori_arrow,dst_arrow);
}

void PdefEcartesianConverter::inter_set_vector(const double delta_x, const double delta_y, const double delta_yaw)
{
    delta_x_=delta_x;
    delta_y_=delta_y;
    delta_yaw_=delta_yaw;
}

void PdefEcartesianConverter::inter_pec2cec(const math::Vec2d ori_ec_xy, math::Vec2d &dst_ec_xy)
{
    double originx = delta_x_;
    double originy = delta_y_;
    double angle= delta_yaw_;
    double pointx = ori_ec_xy.x();
    double pointy = ori_ec_xy.y();

    dst_ec_xy.set_x(cos(angle)*(pointx- originx) + sin(angle)*(pointy - originy));
    dst_ec_xy.set_y(-sin(angle)*(pointx- originx) + cos(angle)*(pointy - originy));
}

void PdefEcartesianConverter::inter_pec2cec_vector(const math::Arrow2d ori_arrow, math::Arrow2d &dst_arrow)
{
    double originx = delta_x_;
    double originy = delta_y_;
    double angle= delta_yaw_;
    double pointx = ori_arrow.center_x();
    double pointy = ori_arrow.center_y();

    // original
    dst_arrow.set_center_x(cos(angle)*(pointx- originx) + sin(angle)*(pointy - originy));
    dst_arrow.set_center_y(-sin(angle)*(pointx- originx) + cos(angle)*(pointy - originy));
    dst_arrow.set_heading(ori_arrow.heading() - delta_yaw_);
}

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd
