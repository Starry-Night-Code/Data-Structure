#include<cstdio>
#include<iostream>
#define stature(p) ((p) ? (p)->_height : -1)
#define tallerChild(x) ( \
stature((x)->lc)>stature((x)->rc) ? (x)->lc : (/*左高*/ \
stature((x)->lc)<stature((x)->rc) ? (x)->rc : (/*右高*/ \
IsLChild(*(x)) ? (x)->lc : (x)->rc \
/*等高：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
) \
) \
)

class BSTNode
{
    public:
        int data;
        BSTNode *left, *right;
};


// 二叉搜索树的搜索实现
BSTNode* Search1(int x, BSTNode *p)
{
    if (p == NULL) return NULL;
    BSTNode* temp = p;
    while (temp != NULL)
    {
        if (x == temp->data) return temp;
        if (x < temp->data)
            temp = temp->left;
        else temp = temp->right;
    }
    return NULL;
} // 迭代实现

BSTNode* Search2(int x, BSTNode *p)
{
    if (p == NULL) return NULL;
    else if (x < p->data) return Search2(x, p->left);
    else if (x > p->data) return Search2(x, p->right);
    else return p;
} // 递归实现
// 时间复杂度为O(h)

bool Insert(int x, BSTNode *&p)
{
    if (p == NULL)
    {
        p = new BSTNode;
        p->data = x;
        p->left = NULL;
        p->right = NULL;
        return true;
    }
    else if (x < p->data)
        Insert(x, p->left);
    else if (x > p->data)
        Insert(x, p->right);
    else return false;
} // 时间复杂度为O(h)，利用插入可构建二叉搜索树

void Construct()
{
    int x;
    BSTNode* root = NULL;
    std::cin >> x;
    while (x >= 0)
    {
        Insert(x, root);
        std::cin >> x;
    }
} // 二叉搜索树的构建

bool Remove(int x, BSTNode* &p) // 在以p为根节点的树中删除元素x
{
    if (p != NULL) // 若递归的各层子树不为空，说明顺利删除返回true
    {
        if (x < p->data) return Remove(x, p->left); //递归进入左子树进行删除
        else if (x > p->data) return Remove(x, p->right); //递归进入右子树删除
        else if (p->left == NULL||p->right == NULL) // 若至多有一个孩子
        {
            BSTNode* temp = p; // 以temp指向即将删除节点
            if (p->left == NULL)
            p = p->right; //若左子树空，p地址更新为其右孩子地址
            else p = p->left; //若右子树空，p地址更新为其左孩子地址
            delete temp; // 删除节点
        }
        else // 左右子树皆存在
        {
            BSTNode* temp = p->right;
            while (temp->left!=NULL) temp = temp->left; //递归寻找左孩子
            p->data = temp->data; //更新该被删除节点的关键码
            Remove(p->data, p->right); // 递归调用删除右子树中的p->data
        }
        return true; // 删除成功，返回true
    }
    return false; // 树中无关键码x，删除失败
}

void RotateR(BSTNode *& ptr) // 右单旋，左子树比右子树高, 旋转后新根在ptr
{
    BSTNode *subR = ptr; // 要右旋转的结点
    ptr = subR->left; // ptr成为新根
    subR->left = ptr->right; // 转移ptr右边负载
    ptr->right = subR;
}

void RotateL(BSTNode *& ptr) // 左单旋，右子树比左子树高, 旋转后新根在ptr
{
    BSTNode *subL = ptr; // 要左旋转的结点
    ptr = subL->right; // ptr成为新根
    subL->right = ptr->left; // 转移ptr左边负载
    ptr->left = subL;
}

template <typename T> // 统一重平衡算法实现
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
                                BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, 
                                BinNodePosi(T) T3) // 在确定a,b,c节点及四棵子树情况下组装重构平衡子树
{
    a->lc = T0; if ( T0 ) T0->parent = a;
    a->rc = T1; if ( T1 ) T1->parent = a;
    updateHeight ( a ); // 组装左子树并更新高度
    c->lc = T2; if ( T2 ) T2->parent = c;
    c->rc = T3; if ( T3 ) T3->parent = c;
    updateHeight ( c ); // 组装右子树并更新高度
    b->lc = a; a->parent = b;
    b->rc = c; c->parent = b;
    updateHeight ( b ); // 组装子树根节点
    return b; //返回该子树新的根节点
} // 在确定a,b,c节点及四棵子树情况下组装重构平衡子树

template <typename T>
BinNodePosi(T) BST<T>::rotateAt ( BinNodePosi(T) v ) // v为非空孙辈节点
{
    BinNodePosi(T) p = v->parent;
    BinNodePosi(T) g = p->parent; // 视v、p和g相对位置分四种情况
    if ( IsLChild ( *p ) )
        if ( IsLChild ( *v ) )
        {
            p->parent = g->parent; // 向上联接
            return connect34 (v, p, g, v->lc, v->rc, p->rc, g->rc);
        }
        else
        {
            v->parent = g->parent; // 向上联接
            return connect34 (p, v, g, p->lc, v->lc, v->rc, g->rc);
        }
    else
}

template <typename T>
bool AVL<T>::remove ( const T& e )
{
    BinNodePosi(T) & x = Search1 ( e ); if ( !x ) return false; // 确认目标存在
    removeAt ( x, _hot ); _size--;
    //先按BST规则删除之（此后，原节点之父_hot及其祖先均可能失衡）
    for ( BinNodePosi(T) g = _hot; g; g = g->parent )
    {
        //从_hot出发向上，逐层检查各代祖先g
        if ( !AvlBalanced ( *g ) ) //一旦发现g失衡
            g = FromParentTo (*g)=rotateAt(tallerChild(tallerChild(g)));//原父亲
        updateHeight ( g ); //并更新其高度（注意：即便g未失衡，高度亦可能降低）
    } //可能需做O(logn)次调整——无论是否做过调整，全树高度均可能降低
    return true; //删除成功
} //若目标节点存在且被删除，返回true；否则返回false
// 重平衡节点都是x的祖先，因此最多O(logn)次重平衡，总体复杂度O(logn)

template <typename T>
BinNodePosi(T) AVL<T>::insert ( const T& e )
{
    BinNodePosi(T) & x = search ( e ); if ( x ) return x; //确认目标节点不存在
    BinNodePosi(T) xx = x = new BinNode<T> ( e, _hot ); _size++; //插入新节点x
    // 此时，x的父亲_hot若增高，则其祖父有可能失衡
    for ( BinNodePosi(T) g = _hot; g; g = g->parent )
    {
        //从x之父出发向上，逐层检查各代祖先g
        if ( !AvlBalanced ( *g ) ) // 发现g失衡
        {
            FromParentTo ( *g ) = rotateAt (tallerChild(tallerChild(g)));
            //3+4组装重新接入原树，FromParentTo(*g)获得g的父亲节点到g的指针
            break; //g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
        }
        else //否则（g依然平衡），只需简单地
            updateHeight ( g ); //更新其高度（注意：即便g未失衡，高度亦可能增加）
    } //至多只需一次调整；若果真做过调整，则全树高度必然复原
    return xx; //返回新节点位置
} //无论e是否存在于原树中，总有AVL::insert(e)->data == e
// O(logn)搜索，最多O(logn)回溯，调整次数最多2次，故总体O(logn)