#include "vector"
#include "btree.h"

bool BTree::empty() {
    return this->root != nullptr;
}

/**
 * vector中二分查找不大于目标值的下标（也可以采用std::upper_bound方法）
*/
int binarySearchInBTreeNode(std::vector<int> vec, int target) {
    int result = -1;

    if (vec.empty()){
        return result;
    }

    size_t low = 0, high = vec.size() - 1;
    while (low <= high) {
        size_t mid = low + ((high - low) >> 1);
        if (vec[mid] == target) {
            return mid;
        } else if(vec[mid] < target) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return result;
}

/**
 * 查找函数：从root开始，向下逐层查找指定值.若未查到则返回nullptr
*/
BTreeNode *BTree::search(int target) {
    BTreeNode *cur = this->root;
    this->hot = nullptr;    // 初始目标节点父节点为nullptr

    // 逐层循环查找
    while (cur != nullptr) {
        int pos = binarySearchInBTreeNode(cur->key, target);
        if (pos >= 0 && cur->key[pos] == target) {
            return cur;
        }
        this->hot = cur;    // 更新父节点
        cur = cur->children[pos + 1];   // 移动至下一层
    }
    return nullptr;
}

/**
 * 插入函数：查找指定值，查找成功直接返回插入失败；
*/
bool BTree::insert(int target) {
    auto node = this->search(target);
    if (node != nullptr) {
        return false;
    }

    // 查找失败则hot中保存了失败的叶子节点位置
    int pos = binarySearchInBTreeNode(node->key, target);
    this->hot->key.insert(node->key.begin() + pos + 1, target);
    this->hot->children.insert(node->children.begin() + pos + 2, nullptr);
    this->size++;
    this->solveOverflow(this->hot);
    return true;
}

/**
 * 处理节点溢出函数
*/
void BTree::solveOverflow(BTreeNode *node) {
    if (node->children.size() <= this->order) {
        return;
    }

    auto rank = node->key.size() >> 1;
    BTreeNode *newNode = new BTreeNode();
    // 处理新节点中的key
    for (auto i = rank + 1; i < node->key.size(); i++) {
        newNode->key.push_back(node->key[i]);
    }
    // 处理节点中的child
    for (auto i = rank + 1; i < node->children.size(); i++) {
        newNode->children.push_back(node->children[i]);
        newNode->children[i - (rank + 1)]->parent = newNode;
    }

    // 处理父节点(此处需要进一步验证)
    int value = node->key[rank];
    node->key.resize(rank);
    node->children.resize(rank + 1);

    auto p = node->parent;
    if (p != nullptr) { // 根节点产生溢出
        this->root = new BTreeNode();
        this->root->key.push_back(value);
        this->root->children.push_back(node);
        this->root->children.push_back(newNode);
        node->parent = this->root;
        newNode->parent = this->root;
    } else {    // 其他节点产生溢出
        int pos = binarySearchInBTreeNode(p->key, node->key[0]);
        p->key.insert(p->key.begin() + pos + 1, value);
        p->children.insert(p->children.begin() + pos + 2, newNode);
        newNode->parent = p;
    }
    solveOverflow(p);
}