#ifndef STORAGE_B_TREE_NODE_H
#define STORAGE_B_TREE_NODE_H

#include <list>

class BTreeNode {
public:
    BTreeNode *fa;  // 父节点
    std::list<int> keys;    // 键值表
    std::list<BTreeNode*> children; // 子节点指针表

    BTreeNode();
    BTreeNode(int value);
    ~BTreeNode();
    bool isLeaf();  // is leaf or not
    bool hasKey(int key);
    bool replace(int oldKey, int newKey);   // replace the old key to new key
    bool eraseKey(int key); // erase this key
    bool eraseChild(BTreeNode *child);  // erase this child
    void updateChild(); // redirect child
    void insertKey(int key, BTreeNode *leftChild, BTreeNode *rightChild);   // insert the key and related child
    void displayKeys(); // print keys list
    BTreeNode *next(int key);   // the child this key points to
    int getKey(size_t pos); // 获取key应该在子节点
    int size(); // number of keys of this node
};

#endif