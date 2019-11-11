#include <iostream>
#include "AVLTree.h"

int main(){
    AVLTree<char, int> tree;
    tree.setDefaultValue(0);
    tree.setAccessBehaviour(Create_If_Not_Found);
    const std::string s = "gfghcafehtqfrqu3t4rq34fqerbfhqefvnbltrmhrlrptowiteqeg";
    for(char c: s){
        tree[c]++;
    }
    for(int i = 97; i < (97 + 26); i++){
        cout << (char)i << " : " << tree[(char)i] << endl;
    }
    return 0;
}