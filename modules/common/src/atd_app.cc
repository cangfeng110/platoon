#include "modules/common/include/atd_app.h"

#include <string>
#include "modules/common/include/time/time.h"
#include "modules/common/include/status/status.h"
#include <iostream>

namespace atd {
namespace common {

using common::time::Clock;


void AtdApp::onTimer(int64_t intervaltimetime)
{
    double before_time_=0.0;
    double after_time_=0.0;
    int64_t eclipsedtime=0;
    while(1)
    {
        before_time_ = common::time::ToSecond(Clock::Now())*1000;
        //==========================
        Step();  // subclass Step()
        //==========================
        after_time_ = common::time::ToSecond(Clock::Now())*1000;
        eclipsedtime=(int64_t)(after_time_-before_time_);
        if(eclipsedtime>=intervaltimetime)
        {
            //std::cout<< "time eclipsed the normal fix time error!"<<std::endl;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(intervaltimetime-eclipsedtime));
        }
        //<< "eclipsedtime:"<< eclipsedtime << "ms,intervaltime:"<<intervaltimetime<<"ms"<<std::endl;
    }
}



AtdApp::AtdApp()
{

}

int AtdApp::Spin() {
    Init();
    Start();
    //    if (!status.ok()) {
    //        return -2;// Start failed
    //    }
    Stop();
    return 0;
}

}  // namespace common
}  // namespace atd
