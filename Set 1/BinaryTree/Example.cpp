#include <iostream>
#include "BinaryTree.cpp"

int main(){
    BinaryTree bt;
    bt.insert(1);
    bt.insert(2);
    bt.insert(3);
    bt.insert(4);
    bt.insert(5);
    bt.insert(6);
    bt.insert(7);

    bt.preOrderIterative();
    bt.postOrderIterative();
    bt.inOrderIterative();
    bt.levelOrderIterative();

    return 0;
}