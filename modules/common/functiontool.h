#ifndef _FUNCTION_TOOL_H_
#define _FUNCTION_TOOL_H_

#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Dense>

// extern const double PI;

namespace platoon {

namespace common {

void TransfromGpsAbsoluteToEgoRelaAzimuth(double &relativeAzimuth,
                                          double sHeading,
                                          double oHeading);


void TransfromGpsAbsoluteToEgoRelaCoord(double &oRelativeX, double &oRelativeY,
                                        double Ego_Heading,
                                        double Ego_Long, double Ego_Lati, double Ego_Alti,
                                        double Remote_Long, double Remote_Lati, double Remote_Alti);


double GpsDistance(double longi1, double lati1,
                   double longi2, double lati2);
                   
void transfromGpsAbsoluteToEgoRelaCoord(double &oRelativeX, double &oRelativeY,
                                        double Heading,
                                        double sLong, double sLati,
                                        double oLong, double oLati);
} // namespace common

} // namespace platoon

#endif // _FUNCTION_TOOL_H_
