#include <iostream>
using namespace std;

template <typename T>
struct DSNode{
    int key;
    int rank;
    T data;
    DSNode* parent;
};

template <typename T>
class DisjointSets{
    int nextKey;

    public:

    DisjointSets();

    typedef DSNode<T>* node_p;

    DSNode<T>* makeSet(T data);
    DSNode<T>* findSet(DSNode<T>* node);
    DSNode<T>* unionSet(DSNode<T>* node1, DSNode<T>* node2);
};

template <typename T>
DisjointSets<T>::DisjointSets(){
    this->nextKey = 0;
}

template <typename T>
DSNode<T>* DisjointSets<T>::makeSet(T data){
    DSNode<T>* newSet = new DSNode<T>;
    newSet->key = this->nextKey++;
    newSet->data = data;
    newSet->rank = 0;
    newSet->parent = newSet;
    return newSet;
}

template <typename T>
DSNode<T>* DisjointSets<T>::findSet(DSNode<T>* node){
    DSNode<T>* root = node;
    while(root->parent != root){
        root = root->parent;
    }

    DSNode<T>* nextNode = node;
    while(node->parent != node){
        nextNode = node->parent;
        node->parent = root;
        node = nextNode;
    }

    return root;
}

template <typename T>
DSNode<T>* DisjointSets<T>::unionSet(DSNode<T>* node1, DSNode<T>* node2){
    DSNode<T>* root1 = findSet(node1);
    DSNode<T>* root2 = findSet(node2);

    if(root1 == root2) return root1;

    if(root1->rank > root2->rank){
        root2->parent = root1;
        return root1;
    }
    if(root2->rank > root1->rank){
        root1->parent = root2;
        return root2;
    }

    root2->parent = root1;
    root1->rank += 1; 
    return root1;
}

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

    cout << set.findSet(p1)->data << endl;
    cout << set.findSet(p2)->data << endl;
    cout << set.findSet(p3)->data << endl;
    cout << set.findSet(p4)->data << endl;
    cout << set.findSet(p5)->data << endl;
    cout << set.findSet(p6)->data << endl;
    cout << set.findSet(p7)->data << endl;
    cout << set.findSet(p8)->data << endl;
    return 0;
}