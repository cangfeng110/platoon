#ifndef MODULES_WORLDMODEL_GRID_MODEL_QUADTREE_H_
#define MODULES_WORLDMODEL_GRID_MODEL_QUADTREE_H_

#include <cmath>
#include <vector>

/**
 * @namespace atd::worldmodel
 * @brief atd::worldmodel
 */
namespace atd {
namespace worldmodel{
namespace gridmodel{


/* 一个矩形区域的象限划分：:
             ^x
             ^
 UL(1)       |    UR(0)
y<---------- |-----------
 LL(2)       |    LR(3)
halfwidth
以下对该象限类型的枚举
*/

/*空间MapRect*/
typedef struct MapRect
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    int halfX;
    int halfY;

    MapRect(){minX=minY=maxX=maxY=halfX=halfY=0;}
    MapRect(int c)//FOR ROOT NODE
    {
        minX=-c;
        minY=-c;
        maxX=c;
        maxY=c;
        halfX=(maxX+minX)/2;
        halfY=(maxY+minY)/2;
    }
    MapRect(int t_maxX,int t_maxY,int t_minX,int t_minY)//FOR SUB NODE
    {
        minX=t_minX;
        minY=t_minY;
        maxX=t_maxX;
        maxY=t_maxY;
        halfX=(maxX+minX)/2;
        halfY=(maxY+minY)/2;
    }
    bool IsPointInRect(float cx,float cy)
    {
        if(cx>minX && cx<=maxX && cy>minY && cy<=maxY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}MapRect;



/* 四叉树节点类型结构 */
typedef struct QuadNode
{
    int     nChildCount;        //子节点个数
    int     transDataCount;
    int     tmpDataCount;
    int     drDataCount;
    int     hypoDataCount;
    int     depthLevel;

    MapRect     Box;            //节点所代表的矩形区域
    QuadNode  *father;          //父节点
    QuadNode  *children[4];     //指向节点的四个孩子
}QuadNode;

/* 四叉树节点类型结构 */
template<class T>
struct QuadCell:QuadNode
{  
    T* realobject;      //空间对象指针数组

    T* up_cell;
    T* down_cell;
    T* left_cell;
    T* right_cell;
};


template<class T>
class QuadTree
{
public:
    QuadTree(int t_depth,int t_rectSize);
    virtual ~QuadTree();

    int rectSize;//
    int halfwidth;  //one of the four root grid's real width
    int cellnumwidth; //cell max index;

    QuadNode  *root;  //root node;
    int depth;        // 四叉树的深度
    MapRect rect;//halfwidth=c

    std::vector<std::vector<T*> > cellCollection;

    //初始化四叉树节点
    QuadNode *InitQuadNode();
    QuadNode* InitQuadCell(MapRect &ur_rect);

    //层次创建四叉树方法（满四叉树）
    void CreateQuadTree(int depth);
    //创建各个分支
    void CreateQuadBranch(int depth, MapRect &rect, QuadNode** node, QuadNode *fathernode);

    //删除多余的节点
    bool DelFalseNode(QuadNode* node);
    //释放树的内存空间
    void ReleaseQuadTree(QuadNode** quadTree);
    //计算四叉树所占的字节的大小
    long CalByteQuadTree(QuadNode* quadTree,long& nSize);

    void CreateCellCollection(int row, int column);
    void ReleaseCellColltection(int row, int column);

    void RecursionNetwork_ConditionSearch(QuadNode* node, bool(*conditionfun)(QuadNode *));
    void RecursionNetwork(QuadNode* node,void *(*fun)(QuadNode*));
    void RecursionNetwork_SpaceSearch(QuadNode* node,int cx,int cy,void *(*fun)(QuadNode*));
    QuadCell<T>* getTdata(QuadNode* node, float cx, float cy, void *(*fun)(QuadNode *));
    bool getTdata1(QuadNode* node,QuadCell<T> &dst, float cx, float cy, void *(*fun)(QuadNode*));

private:


};

template<class T>
QuadTree<T>::QuadTree(int t_depth,int t_rectSize)
{
    depth=t_depth-1;
    rectSize=t_rectSize;
    halfwidth =rectSize*pow(2,depth);//c=50*2^5
    cellnumwidth=2*pow(2,depth)-1;

    //create index
    CreateCellCollection(2*pow(2,depth),2*pow(2,depth));
    rect=MapRect(halfwidth);
    CreateQuadBranch(depth,rect,&root,0);
}

template<class T>
QuadTree<T>::~QuadTree()
{
    DelFalseNode(root);//release index
    ReleaseCellColltection(2*pow(2,depth),2*pow(2,depth));//release cell data
}

template<class T>
QuadNode* QuadTree<T>::InitQuadNode()
{
    QuadNode *node = new QuadNode;
    node->Box.maxX = 0;
    node->Box.maxY = 0;
    node->Box.minX = 0;
    node->Box.minY = 0;

    for (int i = 0; i < 4; i ++)
    {
        node->children[i] = 0;
    }

    node->nChildCount = 0;
    node->father=0;
    node->hypoDataCount=0;
    node->transDataCount=0;
    node->tmpDataCount=0;
    node->drDataCount=0;
    node->depthLevel=-1;

    return node;
}

template<class T>
QuadNode* QuadTree<T>::InitQuadCell(MapRect &ur_rect)
{
    QuadCell<T> *node = new QuadCell<T>;

    //node
    node->Box=ur_rect;
    for (int i = 0; i < 4; i ++)
    {
        node->children[i] = 0;
    }
    node->nChildCount = 0;
    node->father=0;
    node->hypoDataCount=0;
    node->transDataCount=0;
    node->tmpDataCount=0;
    node->drDataCount=0;
    node->depthLevel=0;

    //object
    int indexrow=(int)(halfwidth-node->Box.maxY)/rectSize;
    int indexcolumn=(int)(halfwidth-node->Box.maxX)/rectSize;
    node->realobject = cellCollection[indexrow][indexcolumn];

    if(indexrow==0){node->up_cell=0;}
    else{node->up_cell=cellCollection[indexrow-1][indexcolumn];}

    if(indexcolumn==0){node->left_cell=0;}
    else{node->left_cell=cellCollection[indexrow][indexcolumn-1];}

    if(indexrow==cellnumwidth){node->down_cell=0;}
    else{node->down_cell=cellCollection[indexrow+1][indexcolumn];}

    if(indexcolumn==cellnumwidth){node->right_cell=0;}
    else{node->right_cell=cellCollection[indexrow][indexcolumn+1];}

    return (QuadNode*)node;
}

template<class T>
void QuadTree<T>::CreateQuadBranch(int depth,MapRect &rect,QuadNode** node,QuadNode* fathernode)
{
    *node = InitQuadNode(); //创建树根
    QuadNode *pNode = *node;
    pNode->father=fathernode;
    pNode->Box = rect;
    pNode->depthLevel=depth+1;
    pNode->nChildCount = 4;

    if (depth != 0)
    {
        pNode->children[0] = InitQuadNode();
        MapRect ur_rect(rect.maxX,rect.halfY,rect.halfX,rect.minY);//UR
        CreateQuadBranch(depth-1,ur_rect,&(pNode->children[0]),*node);

        pNode->children[1] = InitQuadNode();
        MapRect ul_rect(rect.maxX,rect.maxY,rect.halfX,rect.halfY);//UL
        CreateQuadBranch(depth-1,ul_rect,&(pNode->children[1]),*node);

        pNode->children[2] = InitQuadNode();
        MapRect ll_rect(rect.halfX,rect.maxY,rect.minX,rect.halfY);//LL
        CreateQuadBranch(depth-1,ll_rect,&(pNode->children[2]),*node);

        pNode->children[3] = InitQuadNode();
        MapRect lr_rect(rect.halfX,rect.halfY,rect.minX,rect.minY);//LR
        CreateQuadBranch(depth-1,lr_rect,&(pNode->children[3]),*node);
    }
    else
    {
        //finall leaf
        MapRect ur_rect(rect.maxX,rect.halfY,rect.halfX,rect.minY);//UR
        pNode->children[0] = InitQuadCell(ur_rect);

        MapRect ul_rect(rect.maxX,rect.maxY,rect.halfX,rect.halfY);//UL
        pNode->children[1] = InitQuadCell(ul_rect);

        MapRect ll_rect(rect.halfX,rect.maxY,rect.minX,rect.halfY);//LL
        pNode->children[2] = InitQuadCell(ll_rect);

        MapRect lr_rect(rect.halfX,rect.halfY,rect.minX,rect.minY);//LR
        pNode->children[3] = InitQuadCell(lr_rect);
    }
}


template<class T>
void QuadTree<T>::RecursionNetwork_ConditionSearch(QuadNode* node,bool(*conditionfun)(QuadNode*))
{
    if(conditionfun(node))
    {
        if (node->nChildCount >0)//节点
        {
            for (int i = 0; i < 4; i ++)
            {
                RecursionNetwork_ConditionSearch(node->children[i],conditionfun);
            }
        }
    }
}


template<class T>
QuadCell<T>* QuadTree<T>::getTdata(QuadNode* node, float cx, float cy, void *(*fun)(QuadNode*))
{
    fun(node);
    QuadCell<T>* result=0;

    if (node->nChildCount >0)//节点
    {
        for (int i = 0; i < 4; i ++)
        {
            if (node->children[i]->Box.IsPointInRect(cx,cy))
            {
                result=getTdata(node->children[i],cx,cy,fun);
                return result;
            }
        }
    }
    else
    {
        if (node->Box.IsPointInRect(cx,cy))
        {
            result=(QuadCell<T>*)node;
            return result;
        }
    }
}


template<class T>
bool QuadTree<T>::getTdata1(QuadNode* node, QuadCell<T> &dst, float cx, float cy, void *(*fun)(QuadNode*))
{
    fun(node);

    if (node->nChildCount >0)//节点
    {
        for (int i = 0; i < 4; i ++)
        {
            if (node->children[i]->Box.IsPointInRect(cx,cy))
            {
                return getTdata1(node->children[i],dst,cx,cy,fun);
            }
        }
    }
    else
    {
        if (node->Box.IsPointInRect(cx,cy))
        {
            dst=(*(QuadCell<T>*)node);
            return true;
        }
    }
    return false;
}



template<class T>
void QuadTree<T>::RecursionNetwork(QuadNode* node,void *(*fun)(QuadNode*))
{
    fun(node);
    if (node->nChildCount >0)//节点
    {
        for (int i = 0; i < 4; i ++)
        {
            RecursionNetwork(node->children[i],fun);
        }
    }
}

template<class T>
void QuadTree<T>::RecursionNetwork_SpaceSearch(QuadNode* node,int cx,int cy,void *(*fun)(QuadNode*))
{
    if (node->Box.IsPointInRect(cx,cy))
    {
        fun(node);
        if (node->nChildCount >0)//节点
        {
            for (int i = 0; i < 4; i ++)
            {
                RecursionNetwork_SpaceSearch(node->children[i],cx,cy,fun);
            }
        }
    }
}



template<class T>
bool QuadTree<T>::DelFalseNode(QuadNode* node)
{
    //如果没有子节点且没有要素
    if (node->nChildCount ==0)
    {
        ReleaseQuadTree(&node);
    }

    //如果有子节点
    else if (node->nChildCount > 0)
    {
        for (int i = 0; i < 4; i ++)
        {
            DelFalseNode(node->children[i]);
        }
    }

    return 1;
}

template<class T>
void QuadTree<T>::ReleaseQuadTree(QuadNode** quadTree)
{
    int i = 0;
    QuadNode* node = *quadTree;
    if (0 == node)
    {
        return;
    }
    else
    {
        for (i = 0; i < 4; i ++)
        {
            ReleaseQuadTree(&node->children[i]);
        }
        delete(node);
        node = 0;
    }

    node = 0;
}


template<class T>
long QuadTree<T>::CalByteQuadTree(QuadNode* quadTree,long& nSize)
{
    if (quadTree != 0)
    {
        nSize += sizeof(QuadNode)+quadTree->nChildCount*sizeof(T);
        for (int i = 0; i < 4; i ++)
        {
            if (quadTree->children[i] != 0)
            {
                nSize += CalByteQuadTree(quadTree->children[i],nSize);
            }
        }
    }

    return 1;
}


template<class T>
void QuadTree<T>::CreateCellCollection(int row, int column)
{
    for(int i = 0; i < row; i++)
    {
        std::vector<T *> onerow;
        for(int j=0;j<column;j++)
        {
            T* tmp_cell=new T(i,j);
            onerow.push_back(tmp_cell);
        }
        cellCollection.push_back(onerow);
    }
}


template<class T>
void QuadTree<T>::ReleaseCellColltection(int row, int column)
{
    for(int i = 0; i < row; i ++)
    {
        for(int j=0;j<column;j++)
        {
            delete cellCollection[i][j];
        }
    }
}

}  // namespace gridmodel
}  // namespace worldmodel
}  // namespace atd

#endif//MODULES_WORLDMODEL_GRID_MODEL_QUADTREE_H_
