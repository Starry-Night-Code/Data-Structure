BinNode ( T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T)
rc = NULL, int h = 0, int l = 1, RBColor c = RB_RED ) :
data (e), parent (p), lc (lc), rc (rc), height (h), npl (l), color (c) { }

template <typename T>
BinNodePosi(T) RedBlack<T>::insert (const T& e) // 将e插入红黑树
{
    BinNodePosi(T) & x = search (e); if (x) return x; //确认目标不存在（留意对_hot的设置）
    x = new BinNode<T> (e, _hot, NULL, NULL, -1); _size++; //创建红节点x：以_hot为父，黑高度-1
    solveDoubleRed ( x ); return x ? x : _hot->parent; //经双红修正后，即可返回
} //无论e是否存在于原树中，返回时总有x->data == e