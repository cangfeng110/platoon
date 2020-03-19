#ifndef MODULES_WORLDMODEL_TF_INTERFRAME_WORLDCARTESIAN_EGOCARTESIAN_H_
#define MODULES_WORLDMODEL_TF_INTERFRAME_WORLDCARTESIAN_EGOCARTESIAN_H_

#include <string>
#include <cmath>
#include "modules/worldmodel/include/math/vec2d.h"
#include "modules/worldmodel/include/math/arrow2d.h"
#include "modules/common/include/macro.h"
#include "modules/common/include/math/angle.h"

namespace atd {
namespace worldmodel  {
namespace tf {

class WcartesianEcartesianConverter {

public:
    //height-loss transform
    static void Set_OriginPoint(const double utm_x, const double utm_y,
                                        const double utm_z, const double heading_gps);

    static void EC2UTM(const math::Vec2d ec_xy, double* utm_x,
                       double* utm_y, double* utm_z);

    static void EC2UTM_Vector(const math::Arrow2d ori_vec,
                              double *utm_x, double *utm_y, double *utm_z,double *heading);

    static void UTM2EC(const double utm_x, const double utm_y,
                       const double utm_z, math::Vec2d* ec_xy);

    static void UTM2EC_Vector(const double utm_x, const double utm_y, const double utm_z,
                              const double heading, math::Arrow2d &dst);


    DECLARE_SINGLETON(WcartesianEcartesianConverter);

private:

    void inter_SetVariable_OriginPoint(const double utm_x, const double utm_y,
                                        const double utm_z, const double heading_gps);

    ///**@brief transform from a point in ego cartesian to KGPRC;
    // * @param tmp_car_CARTESIAN: the point(X,Y,Z,heading(radian with ego X coordirate,anticlock)) in car ego cartesian;
    // * @param tmp_car: the car's absolute location longi/lati/alti(do not matter)/heading;
    // * @result Point_KGPRC: the point absolute location ,X/Y/alti/heading(in radian,anti_clock);
    // * @for example: EC2KGPRC(tmp_target, tmp_car, result);
    // */
    void inter_ec2utm(const math::Vec2d ec_xy, double* utm_x,
                       double* utm_y, double* utm_z);

    void inter_ec2utm_vector(const math::Arrow2d ori_vec,
                             double *utm_x, double *utm_y, double *utm_z,double *heading);

    ///**@brief transform from a point in KGPRC to ego cartesian ;
    // * @param KGPRC_origin: the car(X,Y,Z,heading(radian with ego X coordirate,anticlock)) in KGPRC;
    // * @param KGPRC_target: the target point absolute location in X/Y/Z/heading in KGPRC;
    // * @result mPoint3F: the point absolute location ,X/Y/alti;
    // * @for example: KGPRC2E(KGPRC_origin, KGPRC_target, result);
    // */
    void inter_utm2ec(const double utm_x, const double utm_y,
                       const double utm_z, math::Vec2d* ec_xy);

    void inter_utm2ec_vector(const double utm_x, const double utm_y, const double utm_z,
                                                            const double heading, math::Arrow2d &dst);

    //UTM_OriginPoint
    double UTM_OriginPoint_x;
    double UTM_OriginPoint_y;
    double UTM_OriginPoint_z;
    double UTM_OriginPoint_heading_gps; //gps heading
    float UTM_OriginPoint_absoult_speed;// meter/second


};

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_TF_INTERFRAME_WORLDCARTESIAN_EGOCARTESIAN_H_
