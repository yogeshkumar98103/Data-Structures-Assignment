#include <iostream>
#include "Treap.h"

int main(){
    Treap<int, int, int> treap1;  // Manual Mode

    treap1.insert(5,9,1);
    treap1.insert(67,7,2);
    treap1.insert(23,5,3);
    treap1.insert(12,87,4);
    treap1.insert(4,45,5);
    treap1.insert(2,4,6);
    treap1.insert(3,2,7);
    treap1.insert(50,8,8);
    treap1.insert(6,2,9);
    treap1.insert(24,10,10);
    treap1.insert(50,88,10);
    treap1.insert(12,1,4);
    treap1.insert(2,80,10);
    treap1[{10,4}] = 8;


    for(auto x = treap1.begin(); x != treap1.end(); x++){
        std::cout << x->tree_key << " : " << x->heap_key << " : " << x->value << std::endl;
    }

    Treap<int, int> treap2;       // Randomised Mode
    treap2.insert(5,1);
    treap2.insert(67,2);
    treap2.insert(23,3);
    treap2.insert(12,4);
    treap2.insert(4,5);
    treap2.insert(2,6);
    treap2.insert(3,7);
    treap2.insert(50,8);
    treap2.insert(6,9);
    treap2.insert(24,10);
    treap2.set_default_value(0);


    return 0;
}