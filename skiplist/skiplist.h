#ifndef STORAGE_SKIP_LIST_H
#define STORAGE_SKIP_LIST_H

#define SKIPLIST_MAX_LEVEL 16

struct Node {
    int value;
    Node *forward[SKIPLIST_MAX_LEVEL];
};

class SkipList {
private:
    Node *header;
    int level;
public:
    SkipList();
    ~SkipList();

    int randomLevel();
    void insert(int target);
    void remove(int target);
    bool search(int target);
    void display();
};

#endif