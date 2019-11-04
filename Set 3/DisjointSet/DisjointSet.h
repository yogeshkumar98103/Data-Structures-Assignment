#ifndef DS__DisjointSet
#define DS__DisjointSet

template <typename data_t>
struct DS_Node{
    int key;
    int rank;
    data_t data;
    DS_Node* parent;
};

template <typename data_t>
class DisjointSets{
    int nextKey;

public:

    DisjointSets();

    typedef DS_Node<data_t>* node_p;

    node_p makeSet(data_t data);
    node_p findSet(DS_Node<data_t>* node);
    node_p unionSet(DS_Node<data_t>* node1, DS_Node<data_t>* node2);
};

#include "DisjointSet.cpp"
#endif