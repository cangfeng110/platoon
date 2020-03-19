#include "modules/worldmodel/include/vehicle_state/dr_state_provider.h"
#include "modules/ipc/message/protobuf/dr/dr.pb.h"
#include "modules/common/include/time/time_util.h"
#include "modules/common/include/util/string_util.h"

namespace atd {
namespace worldmodel {

using atd::dr::DrStep;
using atd::dr::DrList;
using atd::common::Status;

DrStateProvider::DrStateProvider() {}

bool DrStateProvider::DrTimeSync(const double input_sensor_timestamp,const DrList &drlistmsg, DrStep &result)
{
    if(!drlistmsg.has_header())
    {
        return false;
    }
    //find the time match
    //    AERROR<<"cur_input_msg_timestamp:"<<GLOG_TIMESTAMP(input_sensor_timestamp);
    //    AERROR<<"latest_dr_timestamp:"<<GLOG_TIMESTAMP(drlistmsg.latest_timestamp());
    //    AERROR<<"oldest_dr_timestamp:"<<GLOG_TIMESTAMP(drlistmsg.oldest_timestamp());

    if(input_sensor_timestamp>drlistmsg.latest_timestamp())
    {
        result.set_delta_x(0);
        result.set_delta_y(0);
        result.set_delta_yaw(0);
        result.set_belief(0.99);
        result.set_sum_x(0);
        result.set_sum_y(0);
        result.set_sum_yaw(0);
        return true;
    }

    if(input_sensor_timestamp<drlistmsg.oldest_timestamp())
    {
        result.set_delta_x(0);
        result.set_delta_y(0);
        result.set_delta_yaw(0);
        result.set_belief(0);
        result.set_sum_x(-500);
        result.set_sum_y(0);
        result.set_sum_yaw(0);
        return false;
    }

    int size = drlistmsg.onedr_size();
    int timematch_index=-1;
    for(int i=0; i<size-1; i++)
    {
        const atd::dr::DrStep*tmp = &drlistmsg.onedr(i);
        const atd::dr::DrStep*next_tmp = &drlistmsg.onedr(i+1);
        double tmp_timestamp=tmp->cur_timestamp();
        double next_tmp_timestamp=next_tmp->cur_timestamp();
        if(input_sensor_timestamp<=tmp_timestamp && input_sensor_timestamp>next_tmp_timestamp)
        {
            timematch_index=i;
            break;
        }
    }
    //std::cout<<"timematch_index:"<<timematch_index<<std::endl;
    if(timematch_index==-1)
    {
        result.set_delta_x(0);
        result.set_delta_y(0);
        result.set_delta_yaw(0);
        result.set_belief(0);
        result.set_sum_x(-500);
        result.set_sum_y(0);
        result.set_sum_yaw(0);
        return false;
    }
    else
    {
        const DrStep* tmp = &drlistmsg.onedr(timematch_index);
        result.set_delta_x(tmp->delta_x());
        result.set_delta_y(tmp->delta_y());
        result.set_delta_yaw(tmp->delta_yaw());
        result.set_belief(tmp->belief());
        result.set_sum_x(tmp->sum_x());
        result.set_sum_y(tmp->sum_y());
        result.set_sum_yaw(tmp->sum_yaw());
        return true;
    }
}


Status DrStateProvider::DrTimeSync_ins(const double input_sensor_timestamp, const dr::DrList &drlistmsg, dr::DrStep_ins &result)
{
    if(!drlistmsg.has_header())
    {
        return Status(atd::common::ErrorCode::LOCALIZATION_ERROR,"no drlist msg");
    }

    if(input_sensor_timestamp>drlistmsg.latest_timestamp())
    {
        result.set_utm_x(0);
        result.set_utm_y(0);
        result.set_heading(0);
        result.set_belief(0.99);
        result.set_sum_x(0);
        result.set_sum_y(0);
        result.set_sum_yaw(0);
        return Status(atd::common::ErrorCode::OK,"none");
    }

    if(input_sensor_timestamp<drlistmsg.oldest_timestamp())
    {
        result.set_utm_x(0);
        result.set_utm_y(0);
        result.set_heading(0);
        result.set_belief(0);
        result.set_sum_x(-500);
        result.set_sum_y(0);
        result.set_sum_yaw(0);
        return Status(atd::common::ErrorCode::LOCALIZATION_ERROR,
                      atd::common::util::StrCat(
                          "the sensor timestamp is older than dr range! dr_oldest_t:",
                          std::to_string(drlistmsg.oldest_timestamp())," input_t:",std::to_string(input_sensor_timestamp)));
    }

    int size = drlistmsg.onedr_ins_size();
    int timematch_index=-1;
    for(int i=0; i<size-1; i++)
    {
        const atd::dr::DrStep_ins*tmp = &drlistmsg.onedr_ins(i);
        const atd::dr::DrStep_ins*next_tmp = &drlistmsg.onedr_ins(i+1);
        double tmp_timestamp=tmp->cur_timestamp();
        double next_tmp_timestamp=next_tmp->cur_timestamp();
        if(input_sensor_timestamp<=tmp_timestamp && input_sensor_timestamp>next_tmp_timestamp)
        {
            timematch_index=i;
            break;
        }
    }
    if(timematch_index==-1)
    {
        result.set_utm_x(0);
        result.set_utm_y(0);
        result.set_heading(0);
        result.set_belief(0);
        result.set_sum_x(-500);
        result.set_sum_y(0);
        result.set_sum_yaw(0);
        return Status(atd::common::ErrorCode::LOCALIZATION_ERROR,"found no match timestamp in the range");
    }
    else
    {
        const atd::dr::DrStep_ins* tmp = &drlistmsg.onedr_ins(timematch_index);
        result.set_utm_x(tmp->utm_x());
        result.set_utm_y(tmp->utm_y());
        result.set_heading(tmp->heading());
        result.set_belief(tmp->belief());
        result.set_sum_x(tmp->sum_x());
        result.set_sum_y(tmp->sum_y());
        result.set_sum_yaw(tmp->sum_yaw());
        return Status(atd::common::ErrorCode::OK,"none");
    }
}

void DrStateProvider::DrFuturePosition() const
{

}

}  // namespace worldmodel
}  // namespace atd
