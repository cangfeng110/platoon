#include "modules/worldmodel/include/tf/WorldFrame/wcartesian_geodetic_conversion.h"

namespace atd {
namespace worldmodel  {
namespace tf {

using atd::worldmodel::math::Vec2d;

void WcartesianGeodeticConverter::GC2UTM(const double longitude_, const double latitude_, const double height_ellipsoid_, double *utm_x, double *utm_y, double *utm_z)
{
    int ProjNo = 0;
    int ZoneWide = 0;
    double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
    double a,f, e2,ee, NN, T,C,A, M, iPI;
    iPI = 0.017453293;//=DEGREE_TO_RADIAN;
    ZoneWide = 3; //3 DEGREE width

    a = 6378137;
    f = 1 / 298.257223563;//earth ellipse constant

    double longitude=longitude_;
    double latitude=latitude_;
    double altitude=height_ellipsoid_;

    ProjNo = (int)(longitude / ZoneWide) ;
    longitude0 = ProjNo * ZoneWide + ZoneWide / 2;
    longitude0 = longitude0 * iPI ;
    latitude0=0;
    longitude1 = longitude * iPI ; //经度转换为弧度
    latitude1 = latitude * iPI ; //纬度转换为弧度
    e2=2*f-f*f;
    ee=e2*(1.0-e2);
    NN=a/sqrt(1.0-e2*sin(latitude1)*sin(latitude1));
    T=tan(latitude1)*tan(latitude1);
    C=ee*cos(latitude1)*cos(latitude1);
    A=(longitude1-longitude0)*cos(latitude1);
    M=a*((1-e2/4-3*e2*e2/64-5*e2*e2*e2/256)*latitude1-(3*e2/8+3*e2*e2/32+45*e2*e2*e2/1024)*sin(2*latitude1)+(15*e2*e2/256+45*e2*e2*e2/1024)*sin(4*latitude1)-(35*e2*e2*e2/3072)*sin(6*latitude1));
    xval = NN*(A+(1-T+C)*A*A*A/6+(5-18*T+T*T+72*C-58*ee)*A*A*A*A*A/120);
    yval = M+NN*tan(latitude1)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                                +(61-58*T+T*T+600*C-330*ee)*A*A*A*A*A*A/720);
    X0 = 1000000L*(ProjNo+1)+500000L;
    Y0 = 0;
    xval = xval+X0; yval = yval+Y0;
    //    *X = xval;
    //    *Y = yval;

    *utm_x = xval;
    *utm_y = yval;
    *utm_z = altitude;
}

void WcartesianGeodeticConverter::UTM2GC(const double utm_x, const double utm_y, const double utm_z, double *longitude, double *latitude, double *height_ellipsoid)
{
    int ProjNo;
    int ZoneWide;
    double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
    double e1,e2,f,a, ee, NN, T,C, M, D,R,u,fai, iPI;
    iPI =0.017453293;// DEGREE_TO_RADIAN;

    a = 6378137;
    f = 1 / 298.257223563;

    ZoneWide = 3; // 3 degree width

    double X=utm_x;
    double Y=utm_y;
    double altitude=utm_z;

    ProjNo = (int)(X/1000000L) ; //find zone in earth
    longitude0 = (ProjNo-1) * ZoneWide + ZoneWide / 2;
    longitude0 = longitude0 * iPI ; //center longitude
    X0 = ProjNo*1000000L+500000L;
    Y0 = 0;
    xval = X-X0;
    yval = Y-Y0; //带内大地坐标
    e2 = 2*f-f*f;
    e1 = (1.0-sqrt(1-e2))/(1.0+sqrt(1-e2));
    ee = e2/(1-e2);
    M = yval;
    u = M/(a*(1-e2/4-3*e2*e2/64-5*e2*e2*e2/256));
    fai = u+(3*e1/2-27*e1*e1*e1/32)*sin(2*u)+(21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(
                4*u)
            +(151*e1*e1*e1/96)*sin(6*u)+(1097*e1*e1*e1*e1/512)*sin(8*u);
    C = ee*cos(fai)*cos(fai);
    T = tan(fai)*tan(fai);
    NN = a/sqrt(1.0-e2*sin(fai)*sin(fai));
    R = a*(1-e2)/sqrt((1-e2*sin(fai)*sin(fai))*(1-e2*sin(fai)*sin(fai))*(1-e2*sin
                                                                         (fai)*sin(fai)));
    D = xval/NN;
    //计算经度(Longitude) 纬度(Latitude)
    longitude1 = longitude0+(D-(1+2*T+C)*D*D*D/6+(5-2*C+28*T-3*C*C+8*ee+24*T*T)*D
                             *D*D*D*D/120)/cos(fai);
    latitude1 = fai -(NN*tan(fai)/R)*(D*D/2-(5+3*T+10*C-4*C*C-9*ee)*D*D*D*D/24
                                      +(61+90*T+298*C+45*T*T-256*ee-3*C*C)*D*D*D*D*D*D/720);

    *longitude = longitude1 / iPI;
    *latitude = latitude1 / iPI;
    *height_ellipsoid = altitude;
}

}  // namespace tf
}  // namespace worldmodel
}  // namespace atd
