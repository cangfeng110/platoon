#include "modules/worldmodel/include/vehicle_state/loc_estimater.h"

#include "modules/ipc/message/protobuf/dr/dr.pb.h"
#include "modules/common/include/time/time_util.h"

#include "modules/worldmodel/include/tf/InterFrame/wcartesian_ecartesian_conversion.h"
#include "modules/worldmodel/include/tf/EgoFrame/pdef_ecartesian_conversion.h"
#include "modules/worldmodel/include/tf/EgoFrame/cdef_ecartesian_conversion.h"
#include "modules/common/include/math/angle.h"
#include "modules/ipc/include/monitor_log/monitor_global.h"


namespace atd {
namespace worldmodel {

using atd::dr::DrStep;
using atd::dr::DrList;
using atd::worldmodel::tf::WcartesianEcartesianConverter;
using atd::worldmodel::tf::PdefEcartesianConverter;
using atd::worldmodel::tf::CdefEcartesianConverter;
using atd::common::math::Angle32;

LocEstimater::LocEstimater(){}

bool LocEstimater::LocSync_ins(const double input_loc_timestamp, const math::Arrow2d &input_loc,
                               const dr::DrList &drlistmsg, Arrow2d &per_loc, Arrow2d &cur_loc)
{
    if(!drlistmsg.has_header())
    {
        atd::ipc::monitor::MonitorGlobal::mlog()->ERROR() <<"dr error 1 drlistmsg has no header";
        return false;

    }
    //find the time match
//    AERROR<<"cur_input_msg_timestamp:"<<GLOG_TIMESTAMP(input_loc_timestamp);
//    AERROR<<"latest_dr_timestamp:"<<GLOG_TIMESTAMP(drlistmsg.latest_timestamp());
//    AERROR<<"oldest_dr_timestamp:"<<GLOG_TIMESTAMP(drlistmsg.oldest_timestamp());
    //fprintf(stderr,"latest_dr_timestamp",drlistmsg.latest_timestamp());
    if(input_loc_timestamp>drlistmsg.latest_timestamp())
    {
        cur_loc=input_loc;
        return true;
    }

    if(input_loc_timestamp<drlistmsg.oldest_timestamp())
    {
        atd::ipc::monitor::MonitorGlobal::mlog()->ERROR() <<"dr error 2 drlistmsg inputtimestamp < droldeststamp";
        atd::ipc::monitor::MonitorGlobal::mlog()->ERROR() <<"input_loc_timestamp:"<<std::to_string(input_loc_timestamp);
        atd::ipc::monitor::MonitorGlobal::mlog()->ERROR() <<"drlistmsg.oldest_timestamp:"<<std::to_string(drlistmsg.oldest_timestamp());

       // fprintf(stderr,"dr fail input_loc_timestamp<drlistmsg.oldest_timestamp\n");
        return false;
    }
    int size = drlistmsg.onedr_ins_size();
    int timematch_index=-1;
    for(int i=0; i<size-1; i++)
    {
        const atd::dr::DrStep_ins*tmp = &drlistmsg.onedr_ins(i);
        const atd::dr::DrStep_ins*next_tmp = &drlistmsg.onedr_ins(i+1);
        double tmp_timestamp=tmp->cur_timestamp();
        double next_tmp_timestamp=next_tmp->cur_timestamp();
        if(input_loc_timestamp<=tmp_timestamp && input_loc_timestamp>next_tmp_timestamp)
        {
            timematch_index=i;
            break;
        }
    }
    //std::cout<<"timematch_index:"<<timematch_index<<std::endl;
    if(timematch_index==-1)
    {
        atd::ipc::monitor::MonitorGlobal::mlog()->ERROR() <<"dr error 3 drlistmsg timematch_index = -1";
       // fprintf(stderr,"dr error 3 drlistmsg timematch_index = -1");
        return false;

    }
    else
    {
        const atd::dr::DrStep_ins* tmp = &drlistmsg.onedr_ins(timematch_index);

        PdefEcartesianConverter::set_vector(-tmp->sum_x(),-tmp->sum_y(),-tmp->sum_yaw());
        Arrow2d tmpvec;//get the input loc in its recv timestamp in cur coordinate
        PdefEcartesianConverter::PEC2CEC_Vector(input_loc,per_loc);
        //std::cout<<"x:"<<per_loc.x()<<"y:"<<per_loc.y()<<"yaw:"<<Angle32::rad2deg(per_loc.yaw)<<std::endl;

        WcartesianEcartesianConverter::Set_OriginPoint(tmp->utm_x(),tmp->utm_y(),0.0,tmp->heading());

        Arrow2d tmpvec_path;//get the predict path in inputloc coordinate
        const atd::dr::DrStep_ins* tmp_0 = &drlistmsg.onedr_ins(0);
        WcartesianEcartesianConverter::UTM2EC_Vector(tmp_0->utm_x(),tmp_0->utm_y(),0.0,tmp_0->heading(),tmpvec_path);

        //Arrow2d tmpvec_test(1,0,Angle32::deg2rad(30));
        CdefEcartesianConverter::set_vector(per_loc.center_x(),per_loc.center_y(),per_loc.heading());
        CdefEcartesianConverter::PEC2CEC_Vector(tmpvec_path,cur_loc);

        return true;
    }
}

//void LocEstimater::ConvertDesk2Desk(double center_x,double center_y, double angl, math::Arrow2d &inputPoint)
//{
//    double ax=inputPoint.center_x();
//    double ay=inputPoint.center_y();
//    double ar = atan2(-ay, ax);

//    double r = sqrt(ax * ax + ay * ay);
//    double adx = r * cos(ar) * cos(angl) + r * sin(ar) * sin(angl);
//    double ady = r * sin(ar) * cos(angl) - r * cos(ar) * sin(angl);
//    double dx = center_x + adx;
//    double dy = center_y - ady;
//    inputPoint.set_center_x(dx);
//    inputPoint.set_center_y(dy);
//    inputPoint.set_heading(angl+inputPoint.heading());
//}

}  // namespace worldmodel
}  // namespace atd
