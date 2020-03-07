#ifndef _UDP_VEHICLE_H
#define _UDP_VEHICLE_H

#include <stdint.h>

struct UDPVehicle
{
        int32_t    vehicle_id; 

        float      vehicle_length;

        float      vehicle_width;

        float      vehicle_height;

        int8_t     desire_drive_mode;

        int8_t     actual_drive_mode;

        int8_t     cut_in_flag;

        double     longitude;

        double     latitude;

        float      altitude;

        float      heading;

        int8_t     gps_status;

        double    gps_time;

        double     relative_x;

        double     relative_y;

        double     relative_heading;

        float      longtitude_acc;

        float      lateral_acc;

        float      speed;

        float      steering_wheel_angle;

        float      yaw_rate;

        float      desire_long_acc;

}__attribute__ ((packed)) ;

#endif