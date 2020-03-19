#ifndef MODULES_WORLDMODEL_TF_INTERFRAME_SENSORCARTESIAN_EGOCARTESIAN_H_
#define MODULES_WORLDMODEL_TF_INTERFRAME_SENSORCARTESIAN_EGOCARTESIAN_H_

#include <string>
#include "modules/worldmodel/include/math/vec2d.h"
#include "modules/common/include/macro.h"
#include <cmath>

namespace atd {
namespace worldmodel  {
namespace tf {

class ScartesianEcartesianConverter {

public:
    //height-loss transform
    static void set_vector(const float ec_pitch, const double ec_roll,
                                        const double ec_z);

    DECLARE_SINGLETON(ScartesianEcartesianConverter);

private:

    void inter_set_vector(const float ec_pitch, const double ec_roll,
                                       const double ec_z);



    ///**@brief transform from ego cartesian to sonser plante cartesian(vehicle chassis cartesian,may has a pitch and roll to ego cartesian);
    // * @param car_SENSOR_PLANE,include pitch & roll;
    // * @param pitch roll get from ego cartesian, in radian, right hand;
    // * @return transform matrix from ego to SPC;
    // * @for example:EC2SPC(tmp_car_sensor_plane, tmp_sttransform_EC_SPC);
    // */
    //void EC2SPC(const car_SENSOR_PLANE &tmp_car_sensor_plane,sttransform_EC_SPC &tmp_transform);


    ///**@brief transform from sonser plante cartesian(vehicle chassis cartesian,may has a pitch and roll to ego cartesian) to ego cartesian
    // * @param car_SENSOR_PLANE,include pitch & roll;
    // * @param pitch roll get from ego cartesian, in radian, right hand;
    // * @return transform matrix from SPC to ego;
    // * @for example:SPC2EC(tmp_car_sensor_plane, tmp_sttransform_EC_SPC);
    // */
    //void SPC2EC(const car_SENSOR_PLANE &tmp_car_sensor_plane, sttransform_EC_SPC &tmp_transform);

    ///**@brief absolute position in pitch/roll;
    // * @in radian, right hand;
    // * @initial without inpput, (0,0);
    // * @initial with inpput, (pitch, roll);
    // */
    //struct car_SENSOR_PLANE
    //{
    //    float fcar_SENSOR_PLANE_pitch;
    //    float fcar_SENSOR_PLANE_roll;

    //    car_SENSOR_PLANE():fcar_SENSOR_PLANE_pitch(0.0f),fcar_SENSOR_PLANE_roll(0.0f){}

    //    car_SENSOR_PLANE(float a, float b):fcar_SENSOR_PLANE_pitch(a),fcar_SENSOR_PLANE_roll(b){}
    //};

    //struct sttransform_EC_SPC
    //{
    //    float transform[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

    //};


    ///**@brief transform a point in spc cartesian to ego point;
    // * @param tmp_car_sensor_plane, pitch, roll,in ego cartesian;
    // * @param tmp_3D_point, a point in ego cartesian, include x, y, z;
    // * @return point coordinate in ego
    // * @for example: SPCpoint2ECpoint(car_SENSOR_PLANE tmp_car_sensor_plane,mPoint3F tmp_3D_point, mPoint3F);
    // */
    //void SPCpoint2ECpoint(const mPoint3F &tmp_3D_point, const car_SENSOR_PLANE &tmp_car_sensor_plane, mPoint3F &result);

    ///**@brief transform a point in ego cartesian to spc point;
    // * @param tmp_car_sensor_plane, pitch, roll,in ego cartesian;
    // * @param tmp_3D_point, a point in ego cartesian, include x, y, z;
    // * @return point coordinate in SPC
    // * @for example: ECpoint2SPCpoint(car_SENSOR_PLANE tmp_car_sensor_plane,mPoint3F tmp_3D_point, mPoint3F);
    // */
    //void ECpoint2SPCpoint(const car_SENSOR_PLANE &tmp_car_sensor_plane,const mPoint3F &tmp_3D_point,mPoint3F &result);

};

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_TF_INTERFRAME_SENSORCARTESIAN_EGOCARTESIAN_H_
