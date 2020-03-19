#ifndef MODULES_WORLDMODEL_GRID_MODEL_MT_CELL_H_
#define MODULES_WORLDMODEL_GRID_MODEL_MT_CELL_H_

#include <vector>
#include <iostream>
#include "modules/worldmodel/include/grid_model/mt_match_struct.h"

namespace atd {
namespace worldmodel{
namespace gridmodel{

class mt_cell
{
public:
    mt_cell(int row,int column);
    virtual ~mt_cell();

    int rowindex;
    int columnindex;

    MTYPE cur_sensor_grid;
    MTYPE tmp_sensor_grid;

    void printself();

private:


};


}  // namespace gridmodel
}  // namespace worldmodel
}  // namespace atd

#endif //MODULES_WORLDMODEL_GRID_MODEL_MT_CELL_H_
