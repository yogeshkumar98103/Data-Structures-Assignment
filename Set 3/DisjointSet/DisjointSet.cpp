#include "DisjointSet.h"

template <typename data_t>
DisjointSets<data_t>::DisjointSets(){
    this->nextKey = 0;
}

template <typename data_t>
DS_Node<data_t>* DisjointSets<data_t>::makeSet(data_t data){
    DS_Node<data_t>* newSet = new DS_Node<data_t>;
    newSet->key = this->nextKey++;
    newSet->data = data;
    newSet->rank = 0;
    newSet->parent = newSet;
    return newSet;
}

template <typename data_t>
DS_Node<data_t>* DisjointSets<data_t>::findSet(DS_Node<data_t>* node){
    DS_Node<data_t>* root = node;
    while(root->parent != root){
        root = root->parent;
    }

    DS_Node<data_t>* nextNode = node;
    while(node->parent != node){
        nextNode = node->parent;
        node->parent = root;
        node = nextNode;
    }

    return root;
}

template <typename data_t>
DS_Node<data_t>* DisjointSets<data_t>::unionSet(DS_Node<data_t>* node1, DS_Node<data_t>* node2){
    DS_Node<data_t>* root1 = findSet(node1);
    DS_Node<data_t>* root2 = findSet(node2);

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
