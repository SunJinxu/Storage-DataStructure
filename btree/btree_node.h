#ifndef STORAGE_B_TREE_NODE_H
#define STORAGE_B_TREE_NODE_H

#include <vector>

class BTreeNode {
public:
    BTreeNode *parent;
    std::vector<int> key;
    std::vector<BTreeNode*> children;

    BTreeNode();
    BTreeNode(int i, BTreeNode *lc = nullptr, BTreeNode *rc = nullptr);
};

#endif