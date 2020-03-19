#ifndef MODULES_WORLDMODEL_GRID_MODEL_GRID_M_H_
#define MODULES_WORLDMODEL_GRID_MODEL_GRID_M_H_

#include <vector>
#include <iostream>

#include "modules/worldmodel/include/grid_model/quadtree.h"
#include "mt_cell.h"

namespace atd {
namespace worldmodel{
namespace gridmodel{

class Grid_M
{
public:
    Grid_M();
    virtual ~Grid_M();

    QuadTree<mt_cell> *searchSpace;

    void HypoDataClear(QuadNode* node);
    void ClearHypoSource(QuadNode* node);
    //    float get_mtype_score(MTYPE *mvalue, int mapcode);
    static float get_mtype_score1(float (*mavlue)[3], int mem_code);
    //    float estimateHypoData(Vector_GC particle);

    //dr
    void DrHistoryGrid(QuadNode *node, double delta_x,double delta_y,double delta_yaw);
    void ClearMainMemory(QuadNode* node);
    void ClearTemporyMemory(QuadNode* node);
    void ClearCellCollection(QuadNode *node);

    //debug
    void PrintBasicInfo();
    void PrintWholeNetwork(QuadNode* node);
    void PaintNetwork(QuadNode *node, std::vector<mt_cell *> &tmp, bool (*func)(QuadNode*));
    static void reset(float (*mvalue)[3]);
    static void tmp2cur(float (*tmp)[3],float (*cur)[3]);

private:

    //float getCellScore(int mem_code, const mPoint3F &tmp_result);
};

bool isHasTransData(QuadNode* node);
bool isHasData(QuadNode *node);
void* InsertTransData(QuadNode* node);
void *InsertHypoData(QuadNode *node);

bool isHasTransDataAndNoHypoData(QuadNode *node);

bool isHasHypoDataAndNoTransData(QuadNode *node);

bool isHasHypoDataAndHasTransData(QuadNode *node);

}  // namespace gridmodel
}  // namespace worldmodel
}  // namespace atd

#endif //MODULES_WORLDMODEL_GRID_MODEL_GRID_M_H_
