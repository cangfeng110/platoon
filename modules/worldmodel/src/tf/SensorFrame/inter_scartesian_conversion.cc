#include "modules/worldmodel/include/tf/SensorFrame/inter_scartesian_conversion.h"

namespace atd {
namespace worldmodel  {
namespace tf {

InterScartesianConverter::InterScartesianConverter() {}

void InterScartesianConverter::set_vector(const float x, const float y, const float z,
                                          const float yaw, const float pitch, const float roll)
{
    InterScartesianConverter::instance()->inter_set_vector(x,y,z,yaw,pitch,roll);
}

void InterScartesianConverter::SPC2SC(const Point3D &tmp_3D_point, Point3D *result)
{
    InterScartesianConverter::instance()->SPCpoint2SCpoint(tmp_3D_point,result);
}

void InterScartesianConverter::SC2SPC(const Point3D &tmp_3D_point, Point3D *result)
{
    InterScartesianConverter::instance()->SCpoint2SPCpoint(tmp_3D_point,result);
}

void InterScartesianConverter::inter_set_vector(const float x, const float y, const float z,
                                                const float yaw, const float pitch, const float roll)
{
    x_=x;y_=y;z_=z;
    yaw_=yaw;pitch_=pitch;roll_=roll;
    gen_matrix_spc2sc(x,y,z,yaw,pitch,roll);
    gen_matrix_sc2spc(x,y,z,yaw,pitch,roll);
}

void InterScartesianConverter::gen_matrix_spc2sc(const float x, const float y, const float z, const float yaw,
                                      const float pitch, const float roll)
{
    // this is error we need to discuss
//    float cosyaw = cos ( -yaw );
//    float sinyaw = sin ( -yaw );
//    float cospitch = cos ( -pitch );
//    float sinpitch = sin ( -pitch );
//    float cosroll = cos ( -roll );
//    float sinroll = sin ( -roll );

//    //    sttransform_SPC_SC transform;
//    matrix_spc2sc.transform[0][0] = cosyaw*cospitch;
//    matrix_spc2sc.transform[0][1] = cosyaw*sinroll*sinpitch - cosroll*sinyaw;
//    matrix_spc2sc.transform[0][2] = sinyaw*sinroll + cosyaw*cosroll*sinpitch;
//    matrix_spc2sc.transform[0][3] = -x;

//    matrix_spc2sc.transform[1][0] = cospitch*sinyaw;
//    matrix_spc2sc.transform[1][1] = cosyaw*cosroll + sinyaw*sinroll*sinpitch;
//    matrix_spc2sc.transform[1][2] = cosroll*sinyaw*sinpitch - cosyaw*sinroll;
//    matrix_spc2sc.transform[1][3] = -y;

//    matrix_spc2sc.transform[2][0] = -sinpitch;
//    matrix_spc2sc.transform[2][1] = cospitch*sinroll;
//    matrix_spc2sc.transform[2][2] = cosroll*cospitch;
//    matrix_spc2sc.transform[2][3] = -z;

//    matrix_spc2sc.transform[3][0] = 0.0f;
//    matrix_spc2sc.transform[3][1] = 0.0f;
//    matrix_spc2sc.transform[3][2] = 0.0f;
    matrix_spc2sc.transform[3][3] = 1.0f;
}

void InterScartesianConverter::gen_matrix_sc2spc(const float x, const float y, const float z,
                                      const float yaw, const float pitch, const float roll)
{
    //A->move->R_yaw->R_pitch->R_roll->B, get the point B in A coordinate

    float cosyaw = cos ( yaw );
    float sinyaw = sin ( yaw );
    float cospitch = cos ( pitch );
    float sinpitch = sin ( pitch );
    float cosroll = cos ( roll );
    float sinroll = sin ( roll );

    matrix_sc2spc.transform[0][0] = cosyaw*cospitch;
    matrix_sc2spc.transform[0][1] = cosyaw*sinroll*sinpitch - cosroll*sinyaw;
    matrix_sc2spc.transform[0][2] = sinyaw*sinroll + cosyaw*cosroll*sinpitch;
    matrix_sc2spc.transform[0][3] = x;

    matrix_sc2spc.transform[1][0] = cospitch*sinyaw;
    matrix_sc2spc.transform[1][1] = cosyaw*cosroll + sinyaw*sinroll*sinpitch;
    matrix_sc2spc.transform[1][2] = cosroll*sinyaw*sinpitch - cosyaw*sinroll;
    matrix_sc2spc.transform[1][3] = y;

    matrix_sc2spc.transform[2][0] = -sinpitch;
    matrix_sc2spc.transform[2][1] = cospitch*sinroll;
    matrix_sc2spc.transform[2][2] = cosroll*cospitch;
    matrix_sc2spc.transform[2][3] = z;

    matrix_sc2spc.transform[3][0] = 0.0f;
    matrix_sc2spc.transform[3][1] = 0.0f;
    matrix_sc2spc.transform[3][2] = 0.0f;
    matrix_sc2spc.transform[3][3] = 1.0f;
}

void InterScartesianConverter::SPCpoint2SCpoint(const Point3D &tmp_3D_point, Point3D *result)
{
    result->set_x(matrix_spc2sc.transform[0][0] * tmp_3D_point.x() + matrix_spc2sc.transform[0][1] * tmp_3D_point.y()
            + matrix_spc2sc.transform[0][2] * tmp_3D_point.z() + matrix_spc2sc.transform[0][3]);
    result->set_y(matrix_spc2sc.transform[1][0] * tmp_3D_point.x() + matrix_spc2sc.transform[1][1] * tmp_3D_point.y()
            + matrix_spc2sc.transform[1][2] * tmp_3D_point.z() + matrix_spc2sc.transform[1][3]);
    result->set_z(matrix_spc2sc.transform[2][0] * tmp_3D_point.x() + matrix_spc2sc.transform[2][1] * tmp_3D_point.y()
            + matrix_spc2sc.transform[2][2] * tmp_3D_point.z() + matrix_spc2sc.transform[2][3]);
}

void InterScartesianConverter::SCpoint2SPCpoint(const Point3D &tmp_3D_point, Point3D *result)
{
    result->set_x(matrix_sc2spc.transform[0][0] * tmp_3D_point.x() + matrix_sc2spc.transform[0][1] * tmp_3D_point.y()
            + matrix_sc2spc.transform[0][2] * tmp_3D_point.z() + matrix_sc2spc.transform[0][3]);
    result->set_y(matrix_sc2spc.transform[1][0] * tmp_3D_point.x() + matrix_sc2spc.transform[1][1] * tmp_3D_point.y()
            + matrix_sc2spc.transform[1][2] * tmp_3D_point.z() + matrix_sc2spc.transform[1][3]);
    result->set_z(matrix_sc2spc.transform[2][0] * tmp_3D_point.x() + matrix_sc2spc.transform[2][1] * tmp_3D_point.y()
            + matrix_sc2spc.transform[2][2] * tmp_3D_point.z() + matrix_sc2spc.transform[2][3]);
}



}  // namespace tf
}  // namespace worldmodel
}  // namespace atd
