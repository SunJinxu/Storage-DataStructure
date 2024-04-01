#include <list>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "bTree.h"

using namespace std;

BTree::BTree() {
    this->k = 3;
    this->nsize = 0;
    this->maxKey = this->k - 1;
    this->minKey = (this->k - 1) >> 1;
    root = nullptr;
}

BTree::BTree(int k):k(k){
    this->nsize = 0;
    this->maxKey = this->k - 1;
    this->minKey = (this->k - 1) >> 1;
    root = nullptr;
}

BTree::~BTree() {
    deleteNode(this->root);
}

void BTree::deleteNode(BTreeNode *node) {
    if (node == nullptr) {
        return;
    }
    for (auto it = node->children.begin(); it != node->children.end(); it++) {
        deleteNode(*it);
    }
    return;
}

bool BTree::insert(int key) {
    BTreeNode *node = this->root;
    // 当前为空树的情况
    if (root == nullptr) {
        root = new BTreeNode(key);
        this->nsize++;
        return true;
    }
    // 寻找到叶子节点处插入点
    while (!node->isLeaf()) {
        if (node->hasKey(key)) {    // 如果插入过程中发现重复，返回失败
            return false;
        }
        node = node->next(key);
    }
    if (node->hasKey(key)) {
        return false;
    }
    node->insertKey(key, nullptr, nullptr);
    maintainAfterInsert(node);
    this->nsize++;
    return true;
}

bool BTree::erase(int key) {
    BTreeNode *node = this->findNode(key);
    if (node == nullptr) {
        return false;
    }
    if (!node->isLeaf()) {
        int nextKey;
        BTreeNode *nextNode = this->findNextNode(node, key, nextKey);
        node->replace(key, nextKey);
        key = nextKey;
        node = nextNode;
    }
    node->eraseKey(key);
    node->eraseChild(nullptr);  // 暂时不理解为什么清楚nullptr
    this->maintainAfterErase(node);
    this->nsize--;
    return true;
}

void BTree::maintainAfterErase(BTreeNode *node) {
    BTreeNode *fa = node->fa;
    if (node == root) {
        if (node->size() == 0) {
            root = *(root->children.begin());
            if (root != nullptr) {
                root->fa = nullptr;
            }
        }
        return;
    }
    // 删除后仍然能够满足节点key数量要求
    if (node->size() >= minKey) {
        return;
    }
    // 删除后不能满足，则需要进行尝试与兄弟节点合并
    int faKey;
    BTreeNode *broNode;
    broNode = findBrotherNode(node, faKey, false);
    if (broNode != nullptr) {
        mergeNode(node, broNode, faKey, false);
    } else {
        broNode = findBrotherNode(node, faKey, true);
        mergeNode(node, broNode, faKey, true);
    }
    this->maintainAfterErase(fa);
}

void BTree::mergeNode(BTreeNode *node1, BTreeNode *node2, int faKey, bool flag) {
    BTreeNode *fa = node1->fa;
    if (node2->size() > minKey) {
        int replaceKey = flag ? *(node2->keys.begin()) : *(node2->keys.rbegin());
        BTreeNode *replaceChild = flag ? *(node2->children.begin()) : *(node2->children.rbegin());
        node2->eraseKey(replaceKey);
        node2->eraseChild(replaceChild);
        fa->replace(faKey, replaceKey);
        if (!flag) {
            node1->keys.push_front(faKey);
            node1->children.push_front(replaceChild);
        } else {
            node1->keys.push_back(faKey);
            node1->children.push_back(replaceChild);
        }
        node1->updateChild();
    } else {
        fa->eraseKey(faKey);
        fa->eraseChild(node1);
        if (!flag) {
            node2->keys.push_back(faKey);
            node2->keys.splice(node2->keys.end(), node1->keys);
            node2->children.splice(node2->children.end(), node1->children);
        } else {
            node2->keys.push_front(faKey);
            node2->keys.splice(node2->keys.begin(), node1->keys);
            node2->children.splice(node2->children.begin(), node1->children);
        }
        fa->updateChild();
        node2->updateChild();
        delete node1;
    }
}

BTreeNode *BTree::findBrotherNode(BTreeNode *node, int &key, bool flag) {
    BTreeNode *fa = node->fa;
    auto keyIt = fa->keys.begin();
    auto childIt = fa->children.begin();
    while (*childIt != node) {
        keyIt++;
        childIt++;
    }
    if (!flag) {
        if (childIt == fa->children.begin()) {
            return nullptr;
        }
        key = *(--keyIt);
        return *(--childIt);
    } else {
        if ((++childIt) == fa->children.end()) {
            return nullptr;
        }
        key = *keyIt;
        return *childIt;
    }
}

BTreeNode *BTree::findNextNode(BTreeNode *node, int key, int &nextKey) {
    if (node == nullptr) {
        return nullptr;
    }

    auto keyIt = node->keys.begin();
    auto childIt = node->children.begin();
    do {
        while (keyIt != node->keys.end()) {
            if (key < *keyIt) {
                break;
            }
            keyIt++;
            childIt++;
        }
        if (keyIt != node->keys.end()) {
            nextKey = *keyIt;
        }
        if (*childIt == nullptr) {
            return node;
        }
    } while (node = *childIt);
}

BTreeNode *BTree::findPreNode(BTreeNode *node, int key, int &preKey) {
    if (node == nullptr) {
        return nullptr;
    }

    auto keyIt = node->keys.rbegin();   
    auto childIt = node->children.rbegin();
    do {
        while (keyIt != node->keys.rend()) {
            if (*keyIt < key) {
                break;
            }
            keyIt++;
            childIt++;
        }
        if (keyIt != node->keys.rend()) {
            preKey = *keyIt;
        }
        if (*childIt == nullptr) {
            return node;
        }
    } while (node = *childIt);
}

BTreeNode *BTree::findNode(int key) {
    BTreeNode *node = this->root;
    while (node != nullptr && !node->hasKey(key)) {
        node = node->next(key);
    }
    return node;
}

void BTree::maintainAfterInsert(BTreeNode *node)
{
	while (node->size() >= this->k) {
		BTreeNode *leftNode = splitLeftNode(node, (this->k - 1) / 2);
		BTreeNode *rightNode = splitRightNode(node, this->k - (this->k - 1) / 2 - 1);
		int key = node->getKey((this->k - 1) / 2);	//上移的值
		delete node;
		node = leftNode->fa;	//父节点
		if (node == nullptr) {
			root = node = new BTreeNode();
			leftNode->fa = rightNode->fa = root;
		}
		node->insertKey(key, leftNode, rightNode);
	}
}

BTreeNode *BTree::splitLeftNode(BTreeNode * p, int pos)
{
	BTreeNode* node = new BTreeNode();
	node->fa = p->fa;
	auto keyIt = p->keys.begin();
	auto childIt = p->children.begin();
	node->children.push_back(*childIt);
	int i = 0;
	while (i++ < pos) {
		childIt++;
		node->keys.push_back(*keyIt);
		node->children.push_back(*childIt);
		keyIt++;
	}
	node->updateChild();
	return node;
}

BTreeNode *BTree::splitRightNode(BTreeNode *p, int pos)
{
	BTreeNode *node = new BTreeNode();
	node->fa = p->fa;
	auto keyIt = p->keys.rbegin();
	auto childIt = p->children.rbegin();
	node->children.push_front(*childIt);
	int i = 0;
	while (i++ < pos) {
		childIt++;
		node->keys.push_front(*keyIt);
		node->children.push_front(*childIt);
		keyIt++;
	}
	node->updateChild();
	return node;
}

int BTree::size() {
    return this->nsize;
}

int BTree::height() {
    int i = 0;
    BTreeNode *node = this->root;
    while (node != nullptr) {
        node = *(node->children.begin());
        ++i;
    }
    return i;
}

BTreeNode *BTree::getRoot() {
    return this->root;
}