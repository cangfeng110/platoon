#include "modules/ipc/include/tmp_trans_channel.h"
#include "modules/common/include/time/time.h"
#include "modules/ipc/include/channel_manager.h"
#include "modules/ipc/include/monitor_log/monitor_global.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>

namespace atd {
namespace ipc {

using atd::common::SLPoint;
using atd::common::PointENU;
using atd::common::SpeedPoint;
using atd::common::PathPoint;

using atd::ipc::ChannelManager;
using atd::ipc::monitor::MonitorGlobal;
using namespace std;

SLPoint MakeSLPoint(const double s, const double l) {
    SLPoint sl;
    sl.set_s(s);
    sl.set_l(l);
    return sl;
}

PointENU MakePointENU(const double x, const double y, const double z) {
    PointENU point_enu;
    point_enu.set_x(x);
    point_enu.set_y(y);
    point_enu.set_z(z);
    return point_enu;
}

//atd::perception::Point MakePerceptionPoint(const double x, const double y,
//                                              const double z) {
//  perception::Point point3d;
//  point3d.set_x(x);
//  point3d.set_y(y);
//  point3d.set_z(z);
//  return point3d;
//}

atd::common::Point3D MakePoint3D(const double x, const double y, const double z)
{
    atd::common::Point3D point3d;
    point3d.set_x(x);
    point3d.set_y(y);
    point3d.set_z(z);
    return point3d;
}

SpeedPoint MakeSpeedPoint(const double s, const double t, const double v,
                          const double a, const double da) {
    SpeedPoint speed_point;
    speed_point.set_s(s);
    speed_point.set_t(t);
    speed_point.set_v(v);
    speed_point.set_a(a);
    speed_point.set_da(da);
    return speed_point;
}

PathPoint MakePathPoint(const double x, const double y, const double z,
                        const double theta, const double kappa,
                        const double dkappa, const double ddkappa) {
    PathPoint path_point;
    path_point.set_x(x);
    path_point.set_y(y);
    path_point.set_z(z);
    path_point.set_theta(theta);
    path_point.set_kappa(kappa);
    path_point.set_dkappa(dkappa);
    path_point.set_ddkappa(ddkappa);
    return path_point;
}


double DriverIns_timestamp=0;
void LCM2PROTO_RawInsLcm_TO_DriverInsProto(RawInsLcm &ins_msg)
{
    double cur_timestamp=atd::common::time::ToSecond(ins_msg.stHeader.nTimeStamp);
    int is_valid=false;
    //set the status in driverins
    if(DriverIns_timestamp!=cur_timestamp)
    {
        DriverIns_timestamp=cur_timestamp;
        is_valid=true;
        atd::ipc::DriverInsProto* t_driverins=ChannelManager::GET_DriverIns_Adapter()->GetData();
        ChannelManager::GET_DriverIns_Adapter()->FillHeader(t_driverins,cur_timestamp);
        atd::common::PointLLH* t_pointllh=t_driverins->mutable_position();
        t_pointllh->set_height(ins_msg.fAltitude);
        t_pointllh->set_lon(ins_msg.fLongitude);
        t_pointllh->set_lat(ins_msg.fLatitude);
        atd::common::Point3D* t_euler_angles=t_driverins->mutable_euler_angles();
        t_euler_angles->set_x(ins_msg.fRoll);
        t_euler_angles->set_y(ins_msg.fPitch);
        t_euler_angles->set_z(ins_msg.fHeading);
    }
    ChannelManager::GET_DriverIns_Adapter()->SetValid(is_valid);
}

void LCM2PROTO_RawIns300CLcm_TO_DriverInsProto(RawIns300CLcm &ins_msg)
{
    double cur_timestamp=common::time::ToSecond(ins_msg.stheader.nTimeStamp);///10^6
    //cur_timestamp=cur_timestamp*0.001;
    int is_valid=false;
    int is_correct=true;
    //set the status in driverins
    if(DriverIns_timestamp!=cur_timestamp)
    {
        DriverIns_timestamp=cur_timestamp;
        is_valid=true;
        atd::ipc::DriverInsProto* t_driverins=ChannelManager::GET_DriverIns_Adapter()->GetData();
        ChannelManager::GET_DriverIns_Adapter()->FillHeader(t_driverins,cur_timestamp);
        atd::common::PointLLH* t_pointllh=t_driverins->mutable_position();
        t_pointllh->set_height(ins_msg.fAltitude);
        t_pointllh->set_lon(ins_msg.dLongitude);
        t_pointllh->set_lat(ins_msg.dLatitude);
        atd::common::Point3D* t_euler_angles=t_driverins->mutable_euler_angles();
        t_euler_angles->set_x(ins_msg.fRoll);
        t_euler_angles->set_y(ins_msg.fPitch);
        t_euler_angles->set_z(ins_msg.fHeading);
        atd::common::Point3D* t_linear_velocity=t_driverins->mutable_linear_velocity();
        t_linear_velocity->set_x(ins_msg.fVelNorth);
        t_linear_velocity->set_y(ins_msg.fVelWest);
        t_linear_velocity->set_z(ins_msg.fVelUp);
        atd::common::Point3D* t_angular_velocity=t_driverins->mutable_angular_velocity();
        t_angular_velocity->set_x(ins_msg.fAngRateX);
        t_angular_velocity->set_y(ins_msg.fAngRateY);
        t_angular_velocity->set_z(ins_msg.fAngRateZ);
        atd::common::Point3D* t_linear_acceleration=t_driverins->mutable_linear_acceleration();
        t_linear_acceleration->set_x(ins_msg.fAccX);
        t_linear_acceleration->set_y(ins_msg.fAccY);
        t_linear_acceleration->set_z(ins_msg.fAccZ);
        t_driverins->set_nnavstatus(ins_msg.nNavStatus);
        t_driverins->set_binitstatusposition(ins_msg.bInitStatusPosition);
        t_driverins->set_binitstatusvel(ins_msg.bInitStatusVel);
        t_driverins->set_binitstatusrp(ins_msg.bInitStatusRP);
        t_driverins->set_binitstatush(ins_msg.bInitStatusH);
        t_driverins->set_dgpstime(ins_msg.dGPSTime);

        t_driverins->clear_stdstd();

      for( int i = 0; i < 9; i++ )

        {
           t_driverins->add_stdstd(ins_msg.stdstd[i]);
        }

//        std::cout<<t_driverins->stdstd_size()<<std::endl;
//        t_driverins->set_stdstd(0,ins_msg.stdstd[0]);
//        t_driverins->set_stdstd(1,ins_msg.stdstd[1]);
//        t_driverins->set_stdstd(2,ins_msg.stdstd[2]);
//        t_driverins->set_stdstd(3,ins_msg.stdstd[3]);
//        t_driverins->set_stdstd(4,ins_msg.stdstd[4]);
//        t_driverins->set_stdstd(5,ins_msg.stdstd[5]);
//        t_driverins->set_stdstd(6,ins_msg.stdstd[6]);
//        t_driverins->set_stdstd(7,ins_msg.stdstd[7]);
//        t_driverins->set_stdstd(8,ins_msg.stdstd[8]);

        t_driverins->set_igpspositionmode(ins_msg.iGPSPositionMode);
        t_driverins->set_igpsvelmode(ins_msg.iGPSVelMode);
        t_driverins->set_idaulantennaorientationmode(ins_msg.iDaulAntennaOrientationMode);
        t_driverins->set_isinsmode(ins_msg.iSINSMode);

    }
    ChannelManager::GET_DriverIns_Adapter()->SetValid(is_valid);
}


double momenta_timestamp=0;
void LCM2PROTO_MomentaLocLcm_TO_Drivermomentaloc(MomentaLocLcm &momenta_msg)
{
    //double cur_timestamp=common::time::ToSecond(momenta_msg.send_time);
    double cur_timestamp=momenta_msg.send_time;
    int is_valid=false;
    //set the status in driverins
    if(momenta_timestamp!=cur_timestamp)
    {
        momenta_timestamp=cur_timestamp;
        is_valid=true;
        atd::ipc::DrivermomentalocProto* t_drivermomentaloc=ChannelManager::GET_Drivermomentaloc_Adapter()->GetData();
        ChannelManager::GET_Drivermomentaloc_Adapter()->FillHeader(t_drivermomentaloc,cur_timestamp);

        //t_drivermomentaloc->set_allocated_header(cur_timestamp);
        t_drivermomentaloc->set_allocated_reservedmsg(0);
        t_drivermomentaloc->set_image_time(momenta_msg.image_time);
        t_drivermomentaloc->set_send_time(momenta_msg.send_time);
        t_drivermomentaloc->set_x(momenta_msg.x);
        t_drivermomentaloc->set_y(momenta_msg.y);
        t_drivermomentaloc->set_z(momenta_msg.z);
        t_drivermomentaloc->set_theta(momenta_msg.theta);
        t_drivermomentaloc->set_init_valid(momenta_msg.init_valid);

        t_drivermomentaloc->set_now_valid(momenta_msg.now_valid);
        t_drivermomentaloc->set_std_pos(momenta_msg.std_pos);
        t_drivermomentaloc->set_quality(momenta_msg.quality);
        t_drivermomentaloc->clear_reseved_float();
        t_drivermomentaloc->clear_reseved_int();
        for( int i = 0; i < 5; i++ )
        {
            t_drivermomentaloc->add_reseved_float(momenta_msg.reseved_float[i]);

            t_drivermomentaloc->add_reseved_int(momenta_msg.reseved_int[i]);
        }


#if 0
        ofstream aaa;
        aaa.open("probufinit.csv",ios::out | ios::app);

        aaa <<setprecision(16) << cur_timestamp<< ","\
        <<setprecision(16) << momenta_msg.image_time<< ","\
        <<setprecision(16) << momenta_msg.send_time<< ","\
        <<setprecision(16) << t_drivermomentaloc->reseved_float(0)<< ","\
<<setprecision(16) << t_drivermomentaloc->reseved_float(1)<< ","\
<<setprecision(16) << t_drivermomentaloc->reseved_float(2)<< ","\
<<setprecision(16) << t_drivermomentaloc->reseved_float(3)<< ","\
<<setprecision(16) << t_drivermomentaloc->reseved_float(4)<< ","\
        <<setprecision(16) << t_drivermomentaloc->reseved_int(0)<< ","\
        <<setprecision(16) << t_drivermomentaloc->reseved_int(1)<< ","\
        <<setprecision(16) << t_drivermomentaloc->reseved_int(2)<< ","\
        <<setprecision(16) << t_drivermomentaloc->reseved_int(3)<< ","\
        <<setprecision(16) << t_drivermomentaloc->reseved_int(4)<< ","\
        <<setprecision(5) << t_drivermomentaloc->init_valid()<< ","\
        <<setprecision(5) << t_drivermomentaloc->now_valid()<< ","\
        <<setprecision(5) << t_drivermomentaloc->std_pos()<< ","\
        <<setprecision(5) << t_drivermomentaloc->quality()<< ","\
        <<setprecision(16) << momenta_msg.reseved_float[0]<< ","\
        <<setprecision(16) << momenta_msg.reseved_float[1]<< ","\
        <<setprecision(16) << momenta_msg.reseved_float[2]<< ","\
        <<setprecision(16) << momenta_msg.reseved_float[3]<< ","\
        <<setprecision(16) << momenta_msg.reseved_float[4]<< ","\
        <<setprecision(16) << momenta_msg.reseved_int[0]<< ","\
        <<setprecision(16) << momenta_msg.reseved_int[1]<< ","\
        <<setprecision(16) << momenta_msg.reseved_int[2]<< ","\
        <<setprecision(16) << momenta_msg.reseved_int[3]<< ","\
        <<setprecision(16) << momenta_msg.reseved_int[4]<< ","\
        <<setprecision(5) << momenta_msg.init_valid<< ","\
        <<setprecision(5) << momenta_msg.now_valid<< ","\
        <<setprecision(5) << momenta_msg.std_pos<< ","\
        <<setprecision(5) << momenta_msg.quality<< ","\
        << endl;//ins.std ,gps.std

        aaa.close();
#endif

    }
    ChannelManager::GET_Drivermomentaloc_Adapter()->SetValid(is_valid);

    //cout << " momenta_msg.header.nTimeStamp = " << momenta_msg.header.nTimeStamp << is_valid <<endl;
}

double DriverUwb_timestamp=0;
void LCM2PROTO_RawUwbLcm_TO_DriverUwbProto(UwbLcm &uwb_msg)
{
    double cur_timestamp=common::time::ToSecond(uwb_msg.header.nTimeStamp);
    int is_valid=false;
    //set the status in driverins
    if(DriverUwb_timestamp!=cur_timestamp)
    {
        DriverUwb_timestamp=cur_timestamp;
        is_valid=true;
        atd::ipc::DriverUwbProto* t_driveruwb=ChannelManager::GET_DriverUwb_Adapter()->GetData();
        ChannelManager::GET_DriverUwb_Adapter()->FillHeader(t_driveruwb,cur_timestamp);
        atd::common::PointLLH* t_pointllh=t_driveruwb->mutable_uwbpoint();
        t_pointllh->set_height(0);
        t_pointllh->set_lon(uwb_msg.longitude);
        t_pointllh->set_lat(uwb_msg.latitude);
        t_driveruwb->set_heading(uwb_msg.heading);
        t_driveruwb->set_uwb_id(uwb_msg.uwb_id);
        t_driveruwb->set_uwb_sendtime(uwb_msg.uwb_sendtime);
        t_driveruwb->set_longitude_std(uwb_msg.longitude_std);
        t_driveruwb->set_latitude_std(uwb_msg.latitude_std);
        t_driveruwb->set_heading_std(uwb_msg.heading_std);
        t_driveruwb->set_paddinggap(uwb_msg.heading_std);
       // t_driveruwb->set_ncredit(uwb_msg.);
    }
    ChannelManager::GET_DriverUwb_Adapter()->SetValid(is_valid);

    //cout << " uwb_msg.header.nTimeStamp = " << uwb_msg.header.nTimeStamp << is_valid <<endl;
}

double DriverVcu_timestamp=0;
void LCM2PROTO_VcuLcm_To_DriverVcuProto(VcuLcm &vcu_msg)
{
    double cur_timestamp=common::time::ToSecond(vcu_msg.stHeader.nTimeStamp);
    int is_valid=false;
    //set the status in driverins
    //if(DriverVcu_timestamp!=cur_timestamp)
    if( vcu_msg.nShifterPosition > 0 )
    {
        DriverVcu_timestamp=cur_timestamp;
        is_valid=true;
        atd::ipc::DriverVcuProto* t_drivervcu=ChannelManager::GET_DriverVcu_Adapter()->GetData();
        ChannelManager::GET_DriverVcu_Adapter()->FillHeader(t_drivervcu,cur_timestamp);
        t_drivervcu->set_fsteeringangle(vcu_msg.fSteeringAngle);
        t_drivervcu->set_fspeed(vcu_msg.fSpeed);
        t_drivervcu->set_fyawrate(vcu_msg.fYawRate);
        t_drivervcu->set_ffrontleftwheelspeed(vcu_msg.fFrontLeftWheelSpeed);
        t_drivervcu->set_ffrontrightwheelspeed(vcu_msg.fFrontRightWheelSpeed);
        t_drivervcu->set_frearleftwheelspeed(vcu_msg.fRearLeftWheelSpeed);
        t_drivervcu->set_frearrightwheelspeed(vcu_msg.fRearRightWheelSpeed);
        t_drivervcu->set_nshifterposition(vcu_msg.nShifterPosition);
        t_drivervcu->set_nleftdirectionlamp(vcu_msg.nLeftDirectionLamp);
        t_drivervcu->set_nrightdirectionlamp(vcu_msg.nRightDirectionLamp);
        t_drivervcu->set_nmainbeamlamp(vcu_msg.nMainBeamLamp);
        t_drivervcu->set_ndippedbeamlamp(vcu_msg.nDippedBeamLamp);
        t_drivervcu->set_nwiperstate(vcu_msg.nWiperState);
        t_drivervcu->set_flateralaccel(vcu_msg.fLateralAccel);
        t_drivervcu->set_flongituaccel(vcu_msg.fLongituAccel);
        t_drivervcu->set_nleftdrivenwheelpulsecounters(vcu_msg.nLeftDrivenWheelPulseCounters);
        t_drivervcu->set_nrightdrivenwheelpulsecounters(vcu_msg.nRightDrivenWheelPulseCounters);
        t_drivervcu->set_nleftnondrivenwheelpulsecounters(vcu_msg.nLeftNonDrivenWheelPulseCounters);
        t_drivervcu->set_nrightnondrivenwheelpulsecounters(vcu_msg.nRightNonDrivenWheelPulseCounters);
        t_drivervcu->set_ndrivemode(vcu_msg.nDriveMode);
    }
    ChannelManager::GET_DriverVcu_Adapter()->SetValid(is_valid);
}

double EngineDestination_timestamp=0;
void LCM2PROTO_NaviRoute_TO_EngineDestinationProto(NaviRouteLcm &debug_msg)
{
    double cur_timestamp=common::time::ToSecond(common::time::Clock::Now());
    int is_valid=false;
    if(debug_msg.Count < 0 || debug_msg.Count > 10)
    {
        debug_msg.Count = 0;
    }
    //set the status in driverins
    if(EngineDestination_timestamp!=cur_timestamp)
    {
        EngineDestination_timestamp=cur_timestamp;
        is_valid=true;
        auto* t_dest=ChannelManager::GET_EngineDestination_Adapter()->GetData();
        ChannelManager::GET_EngineDestination_Adapter()->FillHeader(t_dest,cur_timestamp);

        atd::map::DesPointList*tmp_pointlist= t_dest->mutable_des_pointlist();
        tmp_pointlist->clear_destpoint();
        t_dest->set_source(atd::map::EngineDestination_NaviSource_DEBUG);
        for(int i=1; i<debug_msg.Count;i++)
        {
            //adm_kotei::ADM_RPPOINT* tmp_rppoint=&debug_msg.lstPoints[i];
           //// atd::map::DesPoint* tmp_point=tmp_pointlist->add_destpoint();
     //       std::cout<<"destination point:"<<i<<"Longitude:"<<tmp_rppoint->Longitude<<"Latitude:"<<tmp_rppoint->Latitude<<"Level:"<<tmp_rppoint->Level<<std::endl;
            ////tmp_point->set_longi(tmp_rppoint->Longitude);
            ////tmp_point->set_lati(tmp_rppoint->Latitude);
            ////tmp_point->set_level(tmp_rppoint->Level);
        }
    }
    ChannelManager::GET_EngineDestination_Adapter()->SetValid(is_valid);
}

double EngineDestination_timestamp_1=0;
void LCM2PROTO_AppRoute_TO_EngineDestinationProto(ProxyRequestLcm &input_proxyrequest)
{
    if (input_proxyrequest.command == 0)
    {
        return;
    }
    double cur_timestamp=common::time::ToSecond(input_proxyrequest.header.nTimeStamp);
    int is_valid=false;
    //set the status in driverins
   // fprintf(stderr, "%f\t%f\n", EngineDestination_timestamp_1, cur_timestamp);
    //if(EngineDestination_timestamp_1!=cur_timestamp)
    if(true)
    {
        EngineDestination_timestamp_1=cur_timestamp;
        is_valid=true;
        auto* t_dest=ChannelManager::GET_EngineDestination_Adapter()->GetData();
        ChannelManager::GET_EngineDestination_Adapter()->FillHeader(t_dest,cur_timestamp);
        t_dest->set_source(input_proxyrequest.is_from_app?atd::map::EngineDestination_NaviSource_APP:atd::map::EngineDestination_NaviSource_FICM);
        auto* taskid=t_dest->mutable_des_taskid();
        taskid->set_command(input_proxyrequest.command);
        taskid->set_value(input_proxyrequest.command_value);
       /* if(input_proxyrequest.is_from_app)
        {
            taskid->set_carrier_id(input_proxyrequest.carrier_id);
            taskid->set_parkinglot_id(input_proxyrequest.parkinglot_id);
            taskid->set_area_id(input_proxyrequest.area_id);
            taskid->set_layer_id(input_proxyrequest.layer_id);
            taskid->set_parkingspace_id(input_proxyrequest.parkingspace_id);
        }
        else
        {
            taskid->set_value(input_proxyrequest.command_value);
        }*/
    }
    ChannelManager::GET_EngineDestination_Adapter()->SetValid(is_valid);
}


double Perception_ULTRASONIC_timestamp_1 = 0;
void LCM2PROTO_USDIST_TO_Perception_ULTRASONICProto(USDistsLcm &usd_msg)
{
    double cur_timestamp=common::time::ToSecond(usd_msg.stHeader.nTimeStamp);
    if(cur_timestamp>0)
    {
        int is_valid=false;
        if(Perception_ULTRASONIC_timestamp_1 != cur_timestamp)
        {
            Perception_ULTRASONIC_timestamp_1 = cur_timestamp;
            is_valid = true;
            auto* t_dest=ChannelManager::GET_DriverUltrasonic_Adapter()->GetData();
            t_dest->clear_gstprobes();
            t_dest->clear_header();
            t_dest->clear_reservedmsg();
            ChannelManager::GET_DriverUltrasonic_Adapter()->FillHeader(t_dest,cur_timestamp);
            for(int i = 0;i < 12;i++)
            {
                atd::drivers::ULTRASONIC_DIS* tmpULTRASONIC_DIS = t_dest->add_gstprobes();
                for(int j = 0;j < 7;j++)
                {
                    tmpULTRASONIC_DIS->add_exparam(usd_msg.gstProbes[i].ExParam[j]);
                }
                tmpULTRASONIC_DIS->set_boutrange(usd_msg.gstProbes[i].bOutRange);
                tmpULTRASONIC_DIS->set_fdist(usd_msg.gstProbes[i].fDist);
            }
            ChannelManager::GET_DriverUltrasonic_Adapter()->SetValid(is_valid);
        }
    }
}

double MOBI_LANES_timestamp_1 = 0;
void LCM2PROTO_MELANE_TO_Perception_MOBILANESProto(atd::ipc::MELaneLinesLcm &me_msg)
{
    double cur_timestamp=common::time::ToSecond(me_msg.header.nTimeStamp);
    int is_valid=false;
    if(MOBI_LANES_timestamp_1 != cur_timestamp)
    {
        MOBI_LANES_timestamp_1 = cur_timestamp;
        is_valid = true;
        auto* t_dest=ChannelManager::GET_DriverMobileyeLane_Adapter()->GetData();
        ChannelManager::GET_DriverMobileyeLane_Adapter()->FillHeader(t_dest,cur_timestamp);

        t_dest->set_nconstructionflag(me_msg.nConstructionFlag);
        t_dest->set_nroadtype(me_msg.nRoadType);
        t_dest->set_bhighwayexitright(me_msg.bHighwayExitRight);
        t_dest->set_bhighwayexitleft(me_msg.bHighwayExitLeft);
        t_dest->set_fleftlanedrivingspeed(me_msg.fLeftLaneDrivingSpeed);
        t_dest->set_frightlanedrivingspeed(me_msg.fRightLaneDrivingSpeed);
        t_dest->set_fleftlaneprobability(me_msg.fLeftLaneProbability);
        t_dest->set_frightlaneprobability(me_msg.fRightLaneProbability);

        atd::drivers::MOBILEYE_LANE_LINE* tmpMOBILEYE_LANE_LINEleft = t_dest->mutable_stleftline();
        GenerateProtoLane(me_msg.stLeftLine,tmpMOBILEYE_LANE_LINEleft);

        atd::drivers::MOBILEYE_LANE_LINE* tmpMOBILEYE_LANE_LINEright = t_dest->mutable_strightline();
        GenerateProtoLane(me_msg.stRightLine,tmpMOBILEYE_LANE_LINEright);

        atd::drivers::MOBILEYE_LANE_LINE* tmpMOBILEYE_LANE_LINEnextleft = t_dest->mutable_stnextleftline();
        GenerateProtoLane(me_msg.stNextLeftLine,tmpMOBILEYE_LANE_LINEnextleft);

        atd::drivers::MOBILEYE_LANE_LINE* tmpMOBILEYE_LANE_LINEnextright = t_dest->mutable_stnextrightline();
        GenerateProtoLane(me_msg.stNextRightLine,tmpMOBILEYE_LANE_LINEnextright);

        ChannelManager::GET_DriverMobileyeLane_Adapter()->SetValid(is_valid);
    }
}

void GenerateProtoLane(MOBILEYE_LANE_LINE &line,atd::drivers::MOBILEYE_LANE_LINE* tmpMOBILEYE_LANE_LINE)
{
    tmpMOBILEYE_LANE_LINE->set_nlinetype(line.nLineType);
    tmpMOBILEYE_LANE_LINE->set_nquality(line.nQuality);
    tmpMOBILEYE_LANE_LINE->set_fwidth(line.fWidth);
    tmpMOBILEYE_LANE_LINE->set_blinecrossing(line.bLineCrossing);
    tmpMOBILEYE_LANE_LINE->set_nlinecolor(line.nLineColor);
    tmpMOBILEYE_LANE_LINE->set_ftlc(line.fTLC);
    tmpMOBILEYE_LANE_LINE->set_a(line.a);
    tmpMOBILEYE_LANE_LINE->set_b(line.b);
    tmpMOBILEYE_LANE_LINE->set_c(line.c);
    tmpMOBILEYE_LANE_LINE->set_d(line.d);
    tmpMOBILEYE_LANE_LINE->set_x0(line.x0);
    tmpMOBILEYE_LANE_LINE->set_x1(line.x1);
}


}  // namespace ipc
}  // namespace atd
