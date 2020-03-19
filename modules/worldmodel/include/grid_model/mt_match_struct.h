#ifndef MODULES_WORLDMODEL_GRID_MODEL_MT_MATCH_STRUCT_H_
#define MODULES_WORLDMODEL_GRID_MODEL_MT_MATCH_STRUCT_H_

#include <vector>
#include <iostream>

namespace atd {
namespace worldmodel{
namespace gridmodel{

class MTYPE
{
public:
    float p0_loc_score; // the probability of location error (0-1)
    float p1_occupy_score;// the probability of the occupy (0-1)
    float p1_lane_score;// the probability of the lane (0-1)
    float p2_dashlane_score;// the probability of the lane (0-1)
    float p2_solidlane_score;// the probability of the lane (0-1)

    int source;  // default: -1 mobileye: 0 locmap: 1  both: 2

    MTYPE()
    {
        source = -1;
        p0_loc_score  = 0;
    }
    virtual ~MTYPE(){}

    void reset()
    {
        source=-1;
        p0_loc_score=0;
        p1_occupy_score=0;
        p1_lane_score=0;
        p2_dashlane_score=0;
        p2_solidlane_score=0;
    }

    void printIndex()
    {

    }
};

}  // namespace gridmodel
}  // namespace worldmodel
}  // namespace atd

#endif //MODULES_WORLDMODEL_GRID_MODEL_MT_MATCH_STRUCT_H_
