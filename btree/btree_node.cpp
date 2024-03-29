#include "btree_node.h"

BTreeNode::BTreeNode() {
    this->parent = nullptr;
    this->children.push_back(nullptr);
}

BTreeNode::BTreeNode(int value, BTreeNode *lc, BTreeNode *rc) {
    this->parent = nullptr;
    this->key.push_back(value);
    this->children.push_back(lc);
    this->children.push_back(rc);
    if (lc != nullptr) {
        lc->parent = this;
    }
    if (rc != nullptr) {
        rc->parent = this;
    }
}