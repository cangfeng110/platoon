#include "modules/worldmodel/include/grid_model/grid_m.h"
#include "modules/worldmodel/include/tf/EgoFrame/pdef_ecartesian_conversion.h"

namespace atd {
namespace worldmodel{
namespace gridmodel{

using atd::worldmodel::tf::PdefEcartesianConverter;
using atd::worldmodel::math::Vec2d;
void *InsertHypoData(QuadNode *node)
{
    node->hypoDataCount++;
    if (node->hypoDataCount > 9999)
    {
        node->hypoDataCount = 9999;
    }
}

bool isHypoDataZero(QuadNode *node)
{
    if (node->hypoDataCount != 0)
    {
        node->hypoDataCount = 0;
        return true;
    }
    else
    {
        return false;
    }
}

bool isHasTransData(QuadNode *node)
{
    return node->transDataCount != 0;
}

bool isHasTransDataAndNoHypoData(QuadNode *node)
{
    if(node->transDataCount>0 && node->nChildCount>0)
    {
        return true;
    }
    else
    {
        return (node->transDataCount>0 && node->hypoDataCount==0);
    }
}

bool isHasHypoDataAndNoTransData(QuadNode *node)
{
    if(node->hypoDataCount>0 && node->nChildCount>0)
    {
        return true;
    }
    else
    {
        return (node->hypoDataCount>0 && node->transDataCount==0);
    }

}

bool isHasHypoDataAndHasTransData(QuadNode *node)
{
    return (node->transDataCount>0 && node->hypoDataCount>0);
}

bool isHasData(QuadNode *node)
{
    return node->hypoDataCount != 0 || node->transDataCount != 0;
}

void* InsertTransData(QuadNode* node)
{
    node->transDataCount++;
    if(node->transDataCount>9999)
    {
        node->transDataCount = 9999;
    }
}

bool isTransDataCountZero(QuadNode *node)
{
    if (node->transDataCount != 0)
    {
        node->transDataCount = 0;

        return true;
    }
    else
    {
        return false;
    }
}

bool isHasTmpData(QuadNode *node)
{
    if (node->tmpDataCount != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isTmpDataCountZero(QuadNode *node)
{
    if (node->tmpDataCount != 0)
    {
        node->transDataCount = node->tmpDataCount;
        node->tmpDataCount = 0;
        return true;
    }
    else
    {
        return false;
    }
}


//bool isOutOfRect(const mPoint3F &point)
//{
//    return (fabs(point.x) > 5 || fabs(point.y) > 5);
//}

//====================================

void *PrintNodeParameter(QuadNode *node)
{
    //AINFO << "depth:" << node->depthLevel << "\tbox:" << node->Box.maxX << "\t|" << node->Box.maxY << "\t|"
    //     << node->Box.minX << "\t|" << node->Box.minY;
    //AINFO << "\tchildnodecount:" << node->nChildCount << "\new_tscount:" << node->nChildCount << "\thcount:"
    //     << node->hypoDataCount;
}


Grid_M::Grid_M()
{
    int t_depth=6;//6 layer iterator
    int t_rectSize=50;//50 cm per grid
    searchSpace = new QuadTree<mt_cell>(t_depth, t_rectSize);
    ClearCellCollection(searchSpace->root);
    PrintBasicInfo();
}

Grid_M::~Grid_M()
{
    delete searchSpace;
}

void Grid_M::PrintBasicInfo()
{
    //AINFO << "======net-info========";
    //AINFO << "cellwidth:" << searchSpace->rectSize << "cm";
    //AINFO << "whole rect:plus/minus" << searchSpace->halfwidth / 100.0f << "m";
    //AINFO << "0-" << searchSpace->depth << "layer,and the final layer is cell";
    //AINFO << "======================";
}

void Grid_M::ClearCellCollection(QuadNode *node)
{
    QuadCell<mt_cell> *aresult;

    if (node->nChildCount > 0)//节点
    {
        for (int i = 0; i < 4; i++)
        {
            ClearCellCollection(node->children[i]);
        }
    }
    else
    {
        aresult = (QuadCell<mt_cell> *) node;
        aresult->realobject->cur_sensor_grid.reset();
        aresult->realobject->tmp_sensor_grid.reset();
    }

}

void Grid_M::ClearTemporyMemory(QuadNode *node)
{
    QuadCell<mt_cell> *aresult;

    if (isTmpDataCountZero(node))
    {
        if (node->nChildCount > 0)//节点
        {
            for (int i = 0; i < 4; i++)
            {
                ClearTemporyMemory(node->children[i]);
            }
        }
        else
        {
            aresult = (QuadCell<mt_cell> *) node;
            aresult->realobject->cur_sensor_grid = aresult->realobject->tmp_sensor_grid;
            aresult->realobject->tmp_sensor_grid.reset();
        }
    }
}

void Grid_M::ClearMainMemory(QuadNode *node)
{
    QuadCell<mt_cell> *aresult;

    if (isTransDataCountZero(node))
    {
        if (node->nChildCount > 0)//节点
        {
            for (int i = 0; i < 4; i++)
            {
                ClearMainMemory(node->children[i]);
            }
        }
        else
        {
            aresult = (QuadCell<mt_cell> *) node;
            aresult->realobject->cur_sensor_grid.reset();
        }
    }
}

void Grid_M::PrintWholeNetwork(QuadNode *node)
{
    searchSpace->RecursionNetwork(node, PrintNodeParameter);
}

void Grid_M::PaintNetwork(QuadNode *node, std::vector<mt_cell*> &tmp, bool (*func)(QuadNode *))
{
    QuadCell<mt_cell> *aresult;
    if (func(node))
    {
        if (node->nChildCount > 0)//节点
        {
            for (int i = 0; i < 4; i++)
            {
                PaintNetwork(node->children[i], tmp, func);
            }
        }
        else
        {
            aresult = (QuadCell<mt_cell> *) node;
            tmp.push_back(aresult->realobject);
        }
    }
}


void Grid_M::HypoDataClear(QuadNode *node)
{
    searchSpace->RecursionNetwork_ConditionSearch(node, isHypoDataZero);
}

void Grid_M::ClearHypoSource(QuadNode *node)
{
    if (node->nChildCount > 0)
    {
        for (int i = 0; i < 4; i++)
        {
            ClearHypoSource(node->children[i]);
        }
    }
    else
    {
        QuadCell<mt_cell> *cell = (QuadCell<mt_cell> *) node;
        if (cell->realobject->cur_sensor_grid.source = 2)//
        {
            cell->realobject->cur_sensor_grid.source = 0;
        }
    }
}


void *InsertTmpData(QuadNode *node)
{
    node->tmpDataCount++;
    if (node->tmpDataCount > 9999)
    {
        node->tmpDataCount = 9999;
    }
}

float subscore[] = {0.6, 0.6, 0.8, 0.8, 0.95};
float yawfix[] = {-0.008, 0.008, -0.004, 0.004, 0};

void Grid_M::DrHistoryGrid(QuadNode *node, double delta_x,double delta_y,double delta_yaw)
{
    QuadCell<mt_cell> *aresult;

    if (isHasTransData(node))
    {
        if (node->nChildCount > 0)
        {
            for (int i = 0; i < 4; i++)
            {
                DrHistoryGrid(node->children[i], delta_x, delta_y, delta_yaw);
            }
        }
        else{
            //find leaf
            aresult = (QuadCell<mt_cell> *) node;
            float x = (float) aresult->Box.halfX;
            float y = (float) aresult->Box.halfY;

            MTYPE *tmp_type = &aresult->realobject->cur_sensor_grid;
            tmp_type->source = 0;

            PdefEcartesianConverter::set_vector(delta_x,delta_y,delta_yaw+yawfix[4]);
            Vec2d result;
            PdefEcartesianConverter::PEC2CEC_Point(Vec2d(x/100.0f, y/100.0f), result);

//            QuadCell<mt_cell> *tmp_cell =searchSpace->getTdata(searchSpace->root, result.x() * 100.0f,
//                                                               result.y() * 100.0f, InsertTmpData);
//            if (tmp_cell != 0)
//            {
//                tmp_cell->realobject->tmp_sensor_grid = *tmp_type;
//                tmp_cell->realobject->tmp_sensor_grid.p0_loc_score = tmp_type->p0_loc_score * subscore[4];
//            }
            QuadCell<mt_cell> tmp_cell;
            if (searchSpace->getTdata1(searchSpace->root,tmp_cell, result.x() * 100.0f,result.y() * 100.0f, InsertTmpData))
            {
                tmp_cell.realobject->tmp_sensor_grid = *tmp_type;
                tmp_cell.realobject->tmp_sensor_grid.p0_loc_score = tmp_type->p0_loc_score * subscore[4];
            }
        }
    }
}





//float Grid_M::get_mtype_score(MTYPE *mvalue, int mem_code)
//{
//    float final_value;
//    switch (mem_code)
//    {
//        case 1 :
//            final_value = mvalue->free_items.lane_items.sub_score[FREE_Lane_Dash];
//            break;
//        case 2 :
//            final_value = mvalue->free_items.lane_items.sub_score[FREE_Lane_Solid];
//            break;
//        case 3 :
//            final_value = mvalue->free_items.lane_items.sub_score[FREE_Lane_DSolid];
//            break;
//        case 4 :
//            final_value = mvalue->occupy_items.shoulder_items.sub_score[OCCUPY_Shoulder_CURB];
//            break;
//        case 5 :
//            final_value = mvalue->occupy_items.shoulder_items.sub_score[OCCUPY_Shoulder_TUNNEL_WALL];
//            break;
//        default:
//            final_value = 0;
//            break;
//    }
//    return final_value;
//}

float Grid_M::get_mtype_score1(float (*mvalue)[3], int mem_code)
{
    float final_value;
    switch (mem_code)
    {
    case 1 :
        final_value = mvalue[1][0];
        break;
    case 2 :
        final_value = mvalue[0][0];
        break;
    case 3 :
        final_value = mvalue[2][0];
        break;
    case 4 :
        final_value = mvalue[0][1];
        break;
    case 5 :
        final_value = mvalue[1][1];
        break;
    default:
        final_value = 0;
        break;
    }
    return final_value;
}

void Grid_M::reset(float (*mvalue)[3])
{
    float proc_reset[7][3] = {{0.4/6,0.12,0.05},
                              {0.4/6,0.12,0.05},
                              {0.4/6,0.02,0.05},
                              {0.1, 0.02,0.075},
                              {0.1, 0.02,0.075},
                              {0.4,0.3,0.3},
                              {1,-1,-1}};
    mvalue = proc_reset;
    //    for(int i=0;i<3;i++)
    //    {
    //        for(int j=0;j<7;j++)
    //        {
    //            mvalue[j][i] = proc_reset[j][i];
    //        }
    //    }
}

void Grid_M::tmp2cur(float (*tmp)[3],float (*cur)[3])
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<7;j++)
        {
            cur[j][i] = tmp[j][i];
        }
    }
}


}  // namespace gridmodel
}  // namespace worldmodel
}  // namespace atd
