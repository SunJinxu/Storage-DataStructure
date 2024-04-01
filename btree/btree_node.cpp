#include <iostream>
#include "btree_node.h"

BTreeNode::BTreeNode() {
    this->fa = nullptr;
    this->keys.clear();
    this->children.clear();
}

BTreeNode::BTreeNode(int key) {
    this->keys.push_back(key);
    this->children.push_back(nullptr);
    this->children.push_back(nullptr);
    this->fa = nullptr;
}

BTreeNode::~BTreeNode() {
    this->keys.clear();
    this->children.clear();
}

bool BTreeNode::isLeaf() {
    // 常规叶子节点以及根节点即为叶子节点的情况
    return this->children.empty() || (*this->children.begin() == nullptr);
}

bool BTreeNode::hasKey(int key) {
    for (auto it = this->keys.begin(); it != this->keys.end(); it++) {
        if (*it == key) {
            return true;
        }
    }
    return false;
}

bool BTreeNode::replace(int oldKey, int newKey) {
    for (auto it = this->keys.begin(); it != this->keys.end(); it++) {
        if (*it == oldKey) {
            *it = newKey;
            return true;
        }
    }
    return false;
}

bool BTreeNode::eraseKey(int key) {
    for (auto it = this->keys.begin(); it != this->keys.end(); it++) {
        if (*it == key) {
            this->keys.erase(it);
            return true;
        }
    }
    return false;
}

bool BTreeNode::eraseChild(BTreeNode *child) {
    for (auto it = this->children.begin(); it != this->children.end(); it++) {
        if (*it == child) {
            this->children.erase(it);
            return true;
        }
    }
    return false;
}

BTreeNode *BTreeNode::next(int key) {
    auto keyIt = this->keys.begin();
    auto childIt = this->children.begin();
    while (keyIt != this->keys.end() && key > *keyIt) {
        keyIt++;
        childIt++;
    }
    return *childIt;
}

void BTreeNode::insertKey(int key, BTreeNode *rightChild, BTreeNode *leftChild) {
    auto keyIt = this->keys.begin();
    auto childIt = this->children.begin();
    while (keyIt != this->keys.end() && key > *keyIt) {
        keyIt++;
        childIt++;
    }

    this->keys.insert(keyIt, key);
    if (this->children.empty()) {
        childIt = this->children.erase(childIt);
    }
    childIt = this->children.insert(childIt, rightChild);
    childIt = this->children.insert(childIt, leftChild);
}

void BTreeNode::updateChild() {
    for (auto it = this->children.begin(); it != this->children.end(); it++) {
        if (*it != nullptr) {
            (*it)->fa = this;
        }
    }
}

void BTreeNode::displayKeys() {
    for (auto it = this->keys.begin(); it != this->keys.end(); it++) {
        std::cout << *it << ' ';
    }
}

int BTreeNode::getKey(size_t pos) {
    auto it = this->keys.begin();
    for (int i = 0; i < pos; i++) {
        it++;
    }
    return *it;
}

int BTreeNode::size() {
    return static_cast<int>(this->keys.size());
}