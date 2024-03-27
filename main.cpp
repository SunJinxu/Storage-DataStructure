#include <cstdlib>
#include <iostream>
#include <ctime>
#include "skiplist.h"

int main() {
    srand(time(0));
    SkipList skiplist;
    skiplist.insert(9);
    skiplist.insert(3);
    skiplist.insert(6);
    skiplist.insert(7);
    skiplist.insert(19);
    skiplist.insert(12);
    skiplist.insert(17);
    skiplist.display();
    std::cout << "Search 7: " << (skiplist.search(7) ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 8: " << (skiplist.search(8) ? "Found" : "Not Found") << std::endl;
    skiplist.remove(7);
    skiplist.remove(12);
    skiplist.display();
    std::cout << "Search 7: " << (skiplist.search(7) ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 8: " << (skiplist.search(8) ? "Found" : "Not Found") << std::endl;
    return 0;
}