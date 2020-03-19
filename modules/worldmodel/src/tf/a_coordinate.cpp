////void ConverCartesian2Cartesian(const mPoint3F &tmp_2Dpoint_origin, const float &angle, const mPoint3F &tmp_2Dpoint, mPoint3F &result)
////{
////     double originx = tmp_2Dpoint_origin.x;
////     double originy = tmp_2Dpoint_origin.y;
////     double pointx = tmp_2Dpoint.x;
////     double pointy = tmp_2Dpoint.y;

////     result.y = cos(angle)*(pointx- originx) + sin(angle)*(pointy - originy);
////     result.x = -sin(angle)*(pointx- originx) + cos(angle)*(pointy - originy);
////     return ;
////}

//void EC2KGPRC(const mPoint3F &tmp_target, const Vector_KGPRC &tmp_absolute_car, Vector_KGPRC &result)
//{

//    float pointx = tmp_target.x;
//    float pointy = tmp_target.y;

//    float angle= -tmp_absolute_car.dKGPRC_heading;

//    result.dKGPRC_X = cos(angle)*(pointx) + sin(angle)*(pointy)+tmp_absolute_car.dKGPRC_X;
//    result.dKGPRC_Y = sin(angle)*(pointx) - cos(angle)*(pointy)+tmp_absolute_car.dKGPRC_Y;
//    result.dKGPRC_Z = tmp_target.z+tmp_absolute_car.dKGPRC_Z;

//    return;

////    //ConverCartesian2Cartesian(tmp_2D_car,angle, tmp_2D_target,tmp_2D_result);
////    float originx = tmp_target.x;
////    float originy = tmp_target.y;
////    double pointx = tmp_absolute_car.dKGPRC_X;
////    double pointy = tmp_absolute_car.dKGPRC_Y;

////    float angle = tmp_absolute_car.dKGPRC_heading;

////    result.dKGPRC_X = -sin(angle)*(pointx- originx) + cos(angle)*(pointy - originy);
////    result.dKGPRC_Y = -(cos(angle)*(pointx- originx) + sin(angle)*(pointy - originy));
////    result.dKGPRC_Z = 0;

////    return;
//}



//void EC2SPC(const car_SENSOR_PLANE &tmp_car_sensor_plane,sttransform_EC_SPC &tmp_transform)
//{

//    tmp_transform.transform[0][0] = cos(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch);
//    tmp_transform.transform[0][1] = sin(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch) * sin(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[0][2] = sin(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch)*cos(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[1][0] = 0.0f;
//    tmp_transform.transform[1][1] = cos(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[1][2] = -sin(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[2][0] = -sin(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch);
//    tmp_transform.transform[2][1] = cos(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch)*sin(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[2][2] = cos(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch)*cos(-tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    return ;
//}

//void SPC2EC(const car_SENSOR_PLANE &tmp_car_sensor_plane, sttransform_EC_SPC &tmp_transform)
//{

//    tmp_transform.transform[0][0] = cos(tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch);
//    tmp_transform.transform[0][1] = sin(tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch) * sin(tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[0][2] = sin(tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch)*cos(tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[1][0] = 0.0f;
//    tmp_transform.transform[1][1] = cos(tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[1][2] = -sin(tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[2][0] = -sin(tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch);
//    tmp_transform.transform[2][1] = cos(tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch)*sin(tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    tmp_transform.transform[2][2] = cos(tmp_car_sensor_plane.fcar_SENSOR_PLANE_pitch)*cos(tmp_car_sensor_plane.fcar_SENSOR_PLANE_roll);
//    return ;
//}

//void EC2EC_3D(const car_DR &tmp_DR, const mPoint3F &tmp_3D_point, mPoint3F &result)
//{
////    sttransform_SPC_SC tmp_transform;
////    car_SENSOR tmp_car_sensor(tmp_DR.fcar_DR_x, tmp_DR.fcar_DR_y, tmp_DR.fcar_DR_z, tmp_DR.fcar_DR_yaw, tmp_DR.fcar_DR_pitch, tmp_DR.fcar_DR_roll);

////    SPC2SC(tmp_car_sensor, tmp_transform);

////    result.x = tmp_transform.transform[0][0] * tmp_3D_point.x + tmp_transform.transform[0][1] * tmp_3D_point.y
////            + tmp_transform.transform[0][2] * tmp_3D_point.z + tmp_transform.transform[0][3];
////    result.y = tmp_transform.transform[1][0] * tmp_3D_point.x + tmp_transform.transform[1][1] * tmp_3D_point.y
////            + tmp_transform.transform[1][2] * tmp_3D_point.z + tmp_transform.transform[1][3];
////    result.z = tmp_transform.transform[2][0] * tmp_3D_point.x + tmp_transform.transform[2][1] * tmp_3D_point.y
////            + tmp_transform.transform[2][2] * tmp_3D_point.z + tmp_transform.transform[2][3];
////    return ;
//}



//void transpoint(const mPoint3F &tmp_3D_point, const float transform[4][4], mPoint3F &result)
//{

//    result.x = transform[0][0]*tmp_3D_point.x + transform[0][1]*tmp_3D_point.y + transform[0][2]*tmp_3D_point.z + transform[0][3];
//    result.y = transform[1][0]*tmp_3D_point.x + transform[1][1]*tmp_3D_point.y + transform[1][2]*tmp_3D_point.z + transform[1][3];
//    result.z = transform[2][0]*tmp_3D_point.x + transform[2][1]*tmp_3D_point.y + transform[2][2]*tmp_3D_point.z + transform[2][3];
//}


//void ECpoint2SPCpoint(const car_SENSOR_PLANE &tmp_car_sensor_plane,const mPoint3F &tmp_3D_point,mPoint3F &result)
//{
//    sttransform_EC_SPC tmp_transform;
//    EC2SPC(tmp_car_sensor_plane, tmp_transform);

//    result.x = tmp_transform.transform[0][0] * tmp_3D_point.x + tmp_transform.transform[0][1] * tmp_3D_point.y
//            + tmp_transform.transform[0][2] * tmp_3D_point.z;
//    result.y = tmp_transform.transform[1][0] * tmp_3D_point.x + tmp_transform.transform[1][1] * tmp_3D_point.y
//            + tmp_transform.transform[1][2] * tmp_3D_point.z;
//    result.z = tmp_transform.transform[2][0] * tmp_3D_point.x + tmp_transform.transform[2][1] * tmp_3D_point.y
//            + tmp_transform.transform[2][2] * tmp_3D_point.z;

//    return ;
//}


//void SPCpoint2ECpoint(const mPoint3F &tmp_3D_point,const car_SENSOR_PLANE &tmp_car_sensor_plane,mPoint3F &result)
//{
//    sttransform_EC_SPC tmp_transform;
//    SPC2EC(tmp_car_sensor_plane, tmp_transform);

//    result.x = tmp_transform.transform[0][0] * tmp_3D_point.x + tmp_transform.transform[0][1] * tmp_3D_point.y
//            + tmp_transform.transform[0][2] * tmp_3D_point.z;
//    result.y = tmp_transform.transform[1][0] * tmp_3D_point.x + tmp_transform.transform[1][1] * tmp_3D_point.y
//            + tmp_transform.transform[1][2] * tmp_3D_point.z;
//    result.z = tmp_transform.transform[2][0] * tmp_3D_point.x + tmp_transform.transform[2][1] * tmp_3D_point.y
//            + tmp_transform.transform[2][2] * tmp_3D_point.z;

//    return ;

//}

