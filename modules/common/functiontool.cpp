#include "functiontool.h"

#include <math.h>
#include <iostream>
#include <iomanip>



namespace platoon {

namespace common {

const double PI = 3.1415926;

void TransfromGpsAbsoluteToEgoRelaAzimuth(double &relativeAzimuth,
                                          double sHeading,
                                          double oHeading)
{
    sHeading = sHeading / PI * 180.0;
    oHeading = oHeading / PI * 180.0;
    relativeAzimuth = sHeading - oHeading;
    if(relativeAzimuth < -180) {
        relativeAzimuth += 360;
    }
    if(relativeAzimuth > 180) {
        relativeAzimuth = relativeAzimuth - 360;
    }
    relativeAzimuth = relativeAzimuth * PI / 180.0;
}

void TransfromGpsAbsoluteToEgoRelaCoord(double &oRelativeX, double &oRelativeY,
                                        double Ego_Heading,
                                        double Ego_Long, double Ego_Lati, double Ego_Alti,
                                        double Remote_Long, double Remote_Lati, double Remote_Alti)
 {
    //change by zhangshuai
    const double RL = 6378137.0;  
    const double RS = 6356752.3142; 
    double e = sqrt( ( pow(RL,2) - pow(RS,2) ) / pow(RL,2) );  
    double ego_lati = Ego_Lati * PI / 180 ; 
    double ego_long = Ego_Long * PI / 180 ; 
    double remote_lati = Remote_Lati * PI / 180;
    double remote_long = Remote_Long * PI / 180;
    double ego_v = RL / sqrt( 1 - ( pow(e*sin(ego_lati),2)) ); 
    double remote_v = RL / sqrt( 1 - ( pow(e*sin(remote_lati),2) ) ); 
    double ego_xtemp = (ego_v + Ego_Alti) * cos(ego_lati) * cos(ego_long); 
    double ego_ytemp = (ego_v + Ego_Alti) * cos(ego_lati) * sin(ego_long);
    double ego_ztemp = (ego_v * ( 1 - e * e) + Ego_Alti) * sin(ego_lati);
    double remote_xtemp = (remote_v + Remote_Alti) * cos(remote_lati) * cos(remote_long);
    double remote_ytemp = (remote_v + Remote_Alti) * cos(remote_lati) * sin(remote_long);
    double remote_ztemp = (remote_v * (1 - e * e) + Remote_Alti) * sin(remote_lati);
    // trans wgs_84 to enu coordinate system , left_hand
    double enu_x = -sin(remote_long) * (remote_xtemp -ego_xtemp) + cos(remote_long) * (remote_ytemp - ego_xtemp);
    double enu_y = -sin(remote_lati) * cos(remote_long) * (remote_xtemp -ego_xtemp) 
                    - sin(remote_lati) * sin(remote_long) * (remote_ytemp -ego_xtemp) 
                    + cos(remote_lati) * (remote_ztemp - ego_ztemp);
    double enu_z = cos(remote_lati) * cos(remote_long) * (remote_xtemp - ego_xtemp)
                    + sin(remote_long) * (remote_ytemp - ego_ytemp)
                    + sin(remote_lati) * (remote_ztemp - ego_ztemp);
    // trans enu to vehicle coordinate system
    /*if(Ego_Heading <= 0) 
        Ego_Heading = fabs(Ego_Heading);
    else  
        Ego_Heading = 2 * PI - Ego_Heading;*/
    double right_x = enu_y; // left hand system to right hand system 
    double right_y = enu_x;
    oRelativeX = right_x * cos(Ego_Heading) + right_y * sin(Ego_Heading);
    oRelativeY = right_y * cos(Ego_Heading) - right_x * sin(Ego_Heading);       
}
                                  
double GpsDistance(double longi1, double lati1,
                   double longi2, double lati2)
{
    //r = 6371.004km (earth)
    //ratios of longitude and latitude
    double LatiRatio = 111712.69150641056;   //6371004*PI/180
    double LonRatio = LatiRatio*cos(lati1/180.0*PI);

    double gps_x = (longi2  - longi1) * LonRatio;
    double gps_y = (lati2 - lati1) * LatiRatio;

    return sqrt(gps_x * gps_x + gps_y * gps_y);
}

} // namespace common

} // namespace platoon

