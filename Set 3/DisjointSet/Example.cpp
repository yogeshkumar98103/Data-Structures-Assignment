#include <iostream>
#include "DisjointSet.h"

int main(){
    DisjointSets<int> set;
    typedef DisjointSets<int>::node_p ptr_t;
    ptr_t p1 = set.makeSet(1);
    ptr_t p2 = set.makeSet(2);
    ptr_t p3 = set.makeSet(3);
    ptr_t p4 = set.makeSet(4);
    ptr_t p5 = set.makeSet(5);
    ptr_t p6 = set.makeSet(6);
    ptr_t p7 = set.makeSet(7);
    ptr_t p8 = set.makeSet(8);

    set.unionSet(p1,p2);
    set.unionSet(p4,p5);
    set.unionSet(p3,p5);
    set.unionSet(p6,p7);
    set.unionSet(p1,p7);
    set.unionSet(p1,p2);

    std::cout << set.findSet(p1)->data << std::endl;
    std::cout << set.findSet(p2)->data << std::endl;
    std::cout << set.findSet(p3)->data << std::endl;
    std::cout << set.findSet(p4)->data << std::endl;
    std::cout << set.findSet(p5)->data << std::endl;
    std::cout << set.findSet(p6)->data << std::endl;
    std::cout << set.findSet(p7)->data << std::endl;
    std::cout << set.findSet(p8)->data << std::endl;
    return 0;
}