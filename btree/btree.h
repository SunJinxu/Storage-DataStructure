#ifndef STORAGE_B_TREE_H
#define STORAGE_B_TREE_H

#include "map"
#include "btree_node.h"

class BTree {
protected:
    BTreeNode *root;    // 根节点
    int k;    // B树的阶
    int minKey, maxKey; // 最少键值和最多键值
    int nsize;  // B树中的键总数
    void maintainAfterInsert(BTreeNode *node);  // 插入后保持树形结构
    void maintainAfterErase(BTreeNode *node);   // 删除后保持树形结构
    void mergeNode(BTreeNode *node1, BTreeNode *node2, int faKey, bool flag);    // 合并节点，flag为true合并右节点；否则合并左
    void deleteNode(BTreeNode *node);   // 删除节点，释放空间
    void print(BTreeNode *node);    // 打印节点
    BTreeNode *splitLeftNode(BTreeNode *node, int val); // 分裂获取左节点
    BTreeNode *splitRightNode(BTreeNode *node, int val);    // 分裂获取右节点
    BTreeNode *findBrotherNode(BTreeNode *node, int &key, bool flag);   // 查找兄弟节点，true时寻找左
    BTreeNode *findNode(int value); // 查找保存value的节点
public:
    BTree();
    BTree(int k);
    ~BTree();
    bool insert(int key);   // 插入key
    bool erase(int key);    // 删除key
    BTreeNode *findNextNode(BTreeNode *node, int key, int &nextKey);    // 获取后序节点(retVal)和键值（j）
    BTreeNode *findPreNode(BTreeNode *node, int key, int &preKey);     // 获取前缀节点和键值
    int size(); // 获取树元素总数
    int height();   // 获取树的高度
    BTreeNode *getRoot();   // 获取根节点指针
};

#endif