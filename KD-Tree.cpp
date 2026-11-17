// 2d-树的实现
class Coordinate
{
    public:
        double m_vector[2]; // 点的坐标
};

class PointStruct: public Coordinate
{
    public:
        char m_data; // 点数据的类型
};

class TreeNode: public PointStruct
{
    public:
        int m_dimesion;
        // 0为x方向划分，1为y方向划分
        TreeNode* m_pLeftChild; // 左孩子
        TreeNode* m_pRightChild; // 右孩子
};

class KdTree
{
    public:
        TreeNode* m_pRoot;
};

class RecArea
{
    // 存放矩形区域左下和右上坐标
    Coordinate m_minCoor;
    Coordinate m_maxCoor;
};

// 2d-树构造实现
TreeNode* KdTree::buildTree(std::vector<PointStruct>& pList, int d) // 在深度为d的层次，
// 构造一棵对应于（子）集合pointList（pL)的(子）2d-树
{
    if (pList.empty()) return NULL;
    int dimension = d % 2; // 确定划分方向
    PointStruct medianP = findMedialPoint(dimension, pList);
    // 确定中位点
    std::vector<PointStruct> pListLeft, pListRight; // 左右子树的点集
    for (int i = 0; i < pList.size(); i++)
        if (medianP.isBigger(dimension, pList[i]) //点的dimension轴坐标小于中位点
            pListLeft.push_back(pList[i]); // 归入左子树
        else if (medianP.isSmaller(dimension, pList[i])) // 轴坐标小于中位点
            pListRight.push_back(pList[i]); // 归入右子树
        else if (! medianP.isEqual(pList[i])) // 多个点落在中位线的情况
            pListRight.push_back(pList[i]); // 除了中位点外其它归入右子树
    TreeNode* pTreeNode = new TreeNode(mediaP); //创建子树根,复制节点数据为mediaP
    pTreeNode->m_pLeft = buildTree(pListLeft, d + 1); // 深度d+1构建左子树
    pTreeNode->m_pRight = buildTree(pListRight, d + 1); // 深度d+1构建右子树
    pTreeNode->m_dimesion = dimension; // 设置节点的分割轴方向
    return pTreeNode;
}

// 2d-树范围查询实现
void KdTree::Search_R(TreeNode* pNode, int d, const RecArea& R,
std::vector<PointStruct>& result) // 递归搜索R范围内的点，返回于result
{
    if (!pNode) return; // 节点为空返回
    int dimension = d % 2; // 获得当前分割轴方向
    int flag = R.intersect(dimension, *pNode); //判断节点与R交叠情况
    if (flag < 0) // 只有左子树相交的情况
        Search_R(pNode->m_pLeft, d + 1, R, result);
    else if (flag > 0) // 只有右子树相交的情况
        Search_R(pNode->m_pRight, d + 1, R, result);
    else // 左右子树都相交的情况
    {
        RecArea smallerR, biggerR;
        R.split(dimension, *pNode, smallerR, biggerR); // 划分区域
        Search_R(pNode->m_pLeft, d + 1, smallerR, result);
        Search_R(pNode->m_pRight, d + 1, biggerR, result);
        // 分别在左右子树搜索，搜索区域划分为小区域
        if (area.include(*pNode)) result.push_back(*pNode);
    } // 后序遍历判断，节点是否属于R
}

// 2d-树最近邻查询实现
void KdTree::recurSearch(TreeNode* pNode, int depth, const Coordinate& point,
PointStruct& nearest)
{
    if (!pNode) return;
    int dimension = depth % 2;
    if (point.distance(*pNode) < point.distance(nearest))
        nearest = *pNode; // 查询过程，即递归中的先序遍历，先访问根节点数据
    if (point.isSmaller(dimension, *pNode))
    {
        recurSearch(pNode->m_pLChild, depth + 1, point, nearest);
        if (point.distance(nearest) >= point.distance(dimension,*pNode))
            recurSearch(pNode->m_pRChild, depth + 1, point, nearest);
    } // 分情况进行左右子树的查询与回溯，目标位于左子树则左子树必进入，右子树视回溯过程的垂直距离
    else
    {
        recurSearch(pNode->m_pRChild, depth + 1, point, nearest);
        if (point.distance(nearest) >= point.distance(dimension,*pNode))
            recurSearch(pNode->m_pLChild, depth + 1, point, nearest);
}
} // 先序遍历，必进入其中一棵子树分支，另一个分支进入与否依垂直距离判断