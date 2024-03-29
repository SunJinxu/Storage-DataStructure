#ifndef STORAGE_B_TREE_H
#define STORAGE_B_TREE_H

#include "btree_node.h"

class BTree {
public:
    BTreeNode *root;    // 根节点
    BTreeNode *hot; // 搜索到节点的父亲，如果没有搜索到，返回空节点的父亲
    int order;  // BTree的阶
    int size;   // 关键码总数
    void solveOverflow(BTreeNode *value);   // 解决插入产生的上溢问题
    void solveUnderflow(BTreeNode *value);  // 解决删除产生的下溢问题

    BTree(int order):order(order),size(0),root(new BTreeNode){};
    bool empty();
    BTreeNode *search(int value);
    bool insert(int value);
    bool remove(int value);
};

#endif