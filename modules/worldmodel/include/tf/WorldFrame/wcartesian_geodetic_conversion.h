#ifndef MODULES_TF_WORLD_FRAME_CONVERSION_WCARTESIAN_GEODETIC_CONVERSION_H_
#define MODULES_TF_WORLD_FRAME_CONVERSION_WCARTESIAN_GEODETIC_CONVERSION_H_

#include <string>
#include "modules/common/include/status/status.h"
#include "modules/worldmodel/include/math/vec2d.h"
#include <cmath>

namespace atd {
namespace worldmodel  {
namespace tf {

class WcartesianGeodeticConverter {
public:
    WcartesianGeodeticConverter() = delete;

    ///**@brief transform from Kruger-Gauss plane(2D projection plate) to longi&lati
    // * @param X,Y,Z(no effect to longi/lati transform, equal to altitude),Heading(in radian,anti_clock);
    // * @X,Y,standerd input, include zone number;
    // * @result Point_GC(longi,lati,alti,heading);
    // * @for example:GC2KGPRC(tmp_Point_KGPRC,tmp_Point_GC);
    // */
    static void GC2UTM(const double longitude, const double latitude,
                       const double height_ellipsoid, double* utm_x,
                       double* utm_y, double* utm_z);

    ///**@brief transform from longi&lati to Kruger-Gauss plane(2D projection plate);
    // * @param longi,lati, heading(in radian,anti_clock);
    // * @result Point_KGPRC(X,Y,Z,Heading);
    // * @for example:GC2KGPRC(tmp_Point_GC,tmp_Point_KGPRC);
    // */
    static void UTM2GC(const double utm_x, const double utm_y,
                       const double utm_z, double* longitude,
                       double* latitude, double* height_ellipsoid);

};

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_TF_WORLD_FRAME_CONVERSION_WCARTESIAN_GEODETIC_CONVERSION_H_
