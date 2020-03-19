#include "modules/worldmodel/include/grid_model/mt_cell.h"

namespace atd {
namespace worldmodel{
namespace gridmodel{

mt_cell:: mt_cell(int row, int column)
{
    rowindex=row;
    columnindex=column;
}

mt_cell::~mt_cell()
{

}

void mt_cell::printself()
{
    //AINFO <<"("<<"row:"<<rowindex<<"|colum:"<<columnindex<<")";
}

//void mt_cell::PrintQuadCell(mt_cell* tmp)
//{
//    cout<<"current node:"<<endl;
//    tmp->realobject->printself();
//    cout<<"upcell node:"<<endl;
//    if(tmp->up_cell!=0)
//    {
//        tmp->up_cell->printself();
//    }
//    cout<<"downcell node:"<<endl;
//    if(tmp->down_cell!=0)
//    {
//        tmp->down_cell->printself();
//    }
//    cout<<"leftcell node:"<<endl;
//    if(tmp->left_cell!=0)
//    {
//        tmp->left_cell->printself();
//    }
//    cout<<"rightcell node:"<<endl;
//    if(tmp->right_cell!=0)
//    {
//        tmp->right_cell->printself();
//    }
//}



}  // namespace gridmodel
}  // namespace worldmodel
}  // namespace atd

