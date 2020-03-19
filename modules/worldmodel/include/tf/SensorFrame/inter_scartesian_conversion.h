#ifndef MODULES_WORLDMODEL_TF_EGO_FRAME_INTER_SENSORCARTESIAN_CONVERSION_H_
#define MODULES_WORLDMODEL_TF_EGO_FRAME_INTER_SENSORCARTESIAN_CONVERSION_H_

#include <string>
#include "modules/common/include/macro.h"
#include "modules/common/include/util/util.h"
#include <cmath>

#include "modules/ipc/message/protobuf/common/geometry.pb.h"

namespace atd {
namespace worldmodel  {
namespace tf {

using atd::common::Point3D;

struct sttransform
{
    float transform[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
};

class InterScartesianConverter {
public:
    //delta move transform
    static void set_vector(const float x, const float y, const float z,
                           const float yaw, const float pitch, const float roll);
    static void SC2SPC(const Point3D &tmp_3D_point, Point3D *result);
    static void SPC2SC(const Point3D &tmp_3D_point, Point3D *result);

    DECLARE_SINGLETON(InterScartesianConverter);

private:

    float x_;
    float y_;
    float z_;
    float yaw_;
    float roll_;
    float pitch_;
    sttransform matrix_spc2sc;
    sttransform matrix_sc2spc;

    void inter_set_vector(const float x, const float y, const float z,
                          const float yaw, const float pitch, const float roll);

    /**@brief transform from sonser plante cartesian to sonser plante;
     * @in radian, right hand, in meter;
     * @initial without inpput, (0,0,0,0,0,0);
     * @initial with inpput, (x/y/z/yaw/pitch/roll);
     * @param yaw, pitch, roll in radian, right hand, in sensor plante cartesian
     * @return transform matrix from SPC to SC;
     * @for example: SPC2SC(car_SENSOR tmp_car_sensor,tmp_sttransform_SPC_SC );
     */
    void gen_matrix_spc2sc(const float x,const float y,const float z,
                           const float yaw,const float pitch, const float roll);


    ///**@brief transform from sonser plante cartesian to sonser plante;
    // * @param car_SENSOR: sensor fixed position in sensor plante;
    // * @param yaw, pitch, roll in radian, right hand, in sensor plante cartesian
    // * @return transform matrix from SC to SPC;
    // * @for example:SC2SPC(car_SENSOR tmp_car_sensor, tmp_sttransform_SPC_SC);
    // */
    void gen_matrix_sc2spc(const float x,const float y,const float z,
                           const float yaw,const float pitch, const float roll);



    /**@brief transform a point in SPC cartesian to a point in SC;
     * @param tmp_car_sensor, pitch, roll,in SPC cartesian;
     * @param tmp_3D_point, a point in sensor cartesian, include x, y, z;
     * @return point coordinate in SC
     * @for example: SPCpoint2SCpoint(car_SENSOR tmp_car_sensor, mPoint3F tmp_3D_point, mPoint3F);
     */
    void SPCpoint2SCpoint(const Point3D &tmp_3D_point, Point3D *result);

    ///**@brief transform a point in SC cartesian to a point in SPC;
    // * @param tmp_car_sensor, pitch, roll,in SPC cartesian;
    // * @param tmp_3D_point, a point in sensor plante cartesian, include x, y, z;
    // * @return point coordinate in SPC
    // * @for example: mPoint3F = SCpoint2SPCpoint(car_SENSOR tmp_car_sensor, mPoint3F tmp_3D_point);
    // */
    void SCpoint2SPCpoint(const Point3D &tmp_3D_point, Point3D *result);


    ///**@brief transform a 3d point in specific transform[4][4];
    // * @param tmp_3D_point, a point in cartesian, include x, y, z;
    // * @return point coordinate
    // */
    //void transpoint(const mPoint3F &tmp_3D_point, const float transform[4][4], mPoint3F &result);



};

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_TF_EGO_FRAME_INTER_SENSORCARTESIAN_CONVERSION_H_
