#include <iostream>
#include <cstdlib>
#include <ctime>
#include "skiplist.h"

SkipList::SkipList() {
    this->header = new Node;
    this->header->value = INT_MIN;
    this->level = 1;
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) {
        this->header->forward[i] = nullptr;
    }
}

SkipList::~SkipList() {
    Node *current = this->header;
    while (current != nullptr) {
        Node *tmp = current;
        current = current->forward[0];
        delete tmp;
    }
}

int SkipList::randomLevel() {
    int level = 1;
    // 随机生成新节点高度
    while ((rand() % 2) && (level < SKIPLIST_MAX_LEVEL)) {
        level++;
    }
    return level;
}

void SkipList::insert(int target) {
    // 1 建立前节点数组
    Node *update[SKIPLIST_MAX_LEVEL] = {0};
    
    Node *current = this->header;
    for (int i = this->level - 1; i >= 0; i--) {
        // update指针本层循环都停止在 指针forward为空/指针forward刚好大于target 的节点
        while (current->forward[i] != nullptr && current->forward[i]->value < target) {
            current = current->forward[i];
        }
        // 本层update指针指向当前节点
        update[i] = current;
    }

    // 2 指向插入节点的下一个节点的位置
    current = current->forward[0];

    // 3 如果当前节点下一节点为null或不重复，则可以插入
    if (current == nullptr || current->value != target) {
        int newLevel = this->randomLevel();
        if (newLevel > this->level) {   // 新节点层数大于当前最高层
            for (int i = this->level; i < newLevel; i++) {
                update[i] = this->header;
            }
            this->level = newLevel;
        }

        Node *newNode = new Node;
        newNode->value = target;
        for (int i = 0; i < newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
}

void SkipList::remove(int target) {
    Node *update[SKIPLIST_MAX_LEVEL] = {0};
    Node *current = header;

    for (int i = this->level - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->value < target)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->value == target) {   // current即为目标节点
        for (int i = 0; i < this->level; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }
        delete current;
        // 如果必要，清除高层level
        while (level > 1 && header->forward[level - 1] == nullptr) {
            level--;
        }
    }
}

bool SkipList::search(int target) {
    Node *current = header;
    // 逐层查找当前小于target的最后一个节点，直至level0收敛到目标前一个节点
    for (int i = this->level - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->value < target) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    return (current != nullptr && current->value == target);
}

void SkipList::display() {
    std::cout << "Skip List: ++++++++++++++++++++++++++++++++++++++++" << std::endl;
    // 逐层打印
    for (int i = this->level - 1; i >= 0; i--) {
        Node *current = header->forward[i];
        std::cout << "Level " << i + 1 << ": \t";
        while (current != nullptr) {
            std::cout << current->value << " \t";
            current = current->forward[i];
        }
        std::cout << std::endl;
    }
}