#include <iostream>
#include "BinarySearchTree.cpp"
using namespace std;

int main(){
    BinarySearchTree bst;
    bst.insertRecursive(4);
    bst.insertIteratively(2);
    bst.insertIteratively(8);
    bst.insertRecursive(5);
    bst.insertIteratively(9);
    bst.insertRecursive(7);
    bst.insertIteratively(6);

    bst.levelOrderIterative();
    bst.inOrderIterative();
    
    bst.NthSmallestRecursive(2);
    bst.NthSmallestRecursive(5);
    return 0;
}