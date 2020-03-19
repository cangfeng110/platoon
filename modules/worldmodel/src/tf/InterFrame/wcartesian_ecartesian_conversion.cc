#include "modules/worldmodel/include/tf/InterFrame/wcartesian_ecartesian_conversion.h"

namespace atd {
namespace worldmodel  {
namespace tf {

using atd::worldmodel::math::Vec2d;
using atd::worldmodel::math::Arrow2d;
using atd::common::math::Angle32;

WcartesianEcartesianConverter::WcartesianEcartesianConverter() {}

void WcartesianEcartesianConverter::Set_OriginPoint(
        const double utm_x, const double utm_y, const double utm_z, const double heading_gps)
{
    WcartesianEcartesianConverter::instance()->inter_SetVariable_OriginPoint(utm_x,utm_y,utm_z,heading_gps);
}

void WcartesianEcartesianConverter::EC2UTM(const Vec2d ec_xy, double *utm_x, double *utm_y, double *utm_z)
{
    WcartesianEcartesianConverter::instance()->inter_ec2utm(ec_xy, utm_x, utm_y, utm_z);
}

void WcartesianEcartesianConverter::EC2UTM_Vector(const math::Arrow2d ori_vec, double *utm_x, double *utm_y, double *utm_z, double *heading)
{
    WcartesianEcartesianConverter::instance()->inter_ec2utm_vector(ori_vec, utm_x, utm_y, utm_z,heading);
}

void WcartesianEcartesianConverter::UTM2EC(const double utm_x, const double utm_y, const double utm_z, math::Vec2d *ec_xy)
{
    WcartesianEcartesianConverter::instance()->inter_utm2ec(utm_x,utm_y, utm_z, ec_xy);
}

void WcartesianEcartesianConverter::UTM2EC_Vector(const double utm_x, const double utm_y, const double utm_z, const double heading, math::Arrow2d &dst)
{
    WcartesianEcartesianConverter::instance()->inter_utm2ec_vector(utm_x,utm_y, utm_z,heading, dst);
}



void WcartesianEcartesianConverter::inter_SetVariable_OriginPoint(const double utm_x, const double utm_y, const double utm_z, const double heading_gps)
{
    UTM_OriginPoint_x=utm_x;
    UTM_OriginPoint_y=utm_y;
    UTM_OriginPoint_z=utm_z;
    UTM_OriginPoint_heading_gps=heading_gps; //gps heading
    //UTM_OriginPoint_absoult_speed=absoult_speed;// meter/second
}

void WcartesianEcartesianConverter::inter_ec2utm(const math::Vec2d ec_xy, double *utm_x, double *utm_y, double *utm_z)
{
    //todo::not verify
    double pointx = ec_xy.x();
    double pointy = ec_xy.y();
    float angle=-UTM_OriginPoint_heading_gps;

    *utm_y = cos(angle) * (pointx) + sin(angle) * (pointy)+UTM_OriginPoint_y;
    *utm_x = sin(angle) * (pointx) - cos(angle) * (pointy)+UTM_OriginPoint_x;
    *utm_z = UTM_OriginPoint_z;
}

void WcartesianEcartesianConverter::inter_ec2utm_vector(const math::Arrow2d ori_vec,
                                                        double *utm_x, double *utm_y, double *utm_z,double *heading)
{
    //todo::not verify
    double pointx = ori_vec.center_x();
    double pointy = ori_vec.center_y();
    double angle=-UTM_OriginPoint_heading_gps;

    *utm_y = cos(angle) * (pointx) + sin(angle) * (pointy)+UTM_OriginPoint_y;
    *utm_x = sin(angle) * (pointx) - cos(angle) * (pointy)+UTM_OriginPoint_x;
    *utm_z = UTM_OriginPoint_z;

    auto tmp=Angle32::from_rad(-angle+ori_vec.heading());
    double tmpdegree=tmp.to_rad();
    if(tmpdegree<0){
        *heading=tmpdegree+Angle32::deg2rad(360);
    }else{
        *heading=tmpdegree;
    }
}


void WcartesianEcartesianConverter::inter_utm2ec(const double utm_x, const double utm_y, const double utm_z, math::Vec2d *ec_xy)
{
    double originx = UTM_OriginPoint_y;
    double originy = -UTM_OriginPoint_x; //china area is opposite
    double angle=-UTM_OriginPoint_heading_gps;

    double pointx = utm_y;
    double pointy =-utm_x;

    ec_xy->set_x(cos(angle) * (pointx - originx) - sin(angle) * (pointy - originy));
    ec_xy->set_y(sin(angle) * (pointx - originx) + cos(angle) * (pointy - originy));

}

void WcartesianEcartesianConverter::inter_utm2ec_vector(const double utm_x, const double utm_y, const double utm_z,
                                                         const double heading, math::Arrow2d &dst)
{
    double originx = UTM_OriginPoint_y;
    double originy = -UTM_OriginPoint_x; //china area is opposite
    float angle=-UTM_OriginPoint_heading_gps;

    double pointx = utm_y;
    double pointy =-utm_x;

    dst.set_center_x(cos(angle) * (pointx - originx) - sin(angle) * (pointy - originy));
    dst.set_center_y(sin(angle) * (pointx - originx) + cos(angle) * (pointy - originy));

    auto tmp=Angle32::from_rad(UTM_OriginPoint_heading_gps)-Angle32::from_rad(heading);
    dst.set_heading(-tmp.to_rad());  //to_rad has a nagetive signal
}

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd
