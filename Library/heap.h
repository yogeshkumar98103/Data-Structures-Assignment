//
// Created by Yogesh Kumar on 2019-04-17.
//

#ifndef CLION_WORKSPACE_HEAP_H
#define CLION_WORKSPACE_HEAP_H

#include <iostream>
#define INITIAL_HEAP_SIZE 20

// Node of Heap
template <class T> class Node{
public:
    T data;
    int priority;
};

// Heap Base Class
template <class T> class BaseHeap{
public:

    Node<T>* heap;
    int capacity;

public:
    int size;

    BaseHeap();

    void buildFromArray(Node<T> array[],int size);

    ~BaseHeap();

    void expand();

    bool isEmpty();

    // MARK :- Parent/Child Index
    int parentIndex(int childIndex);

    int leftChild(int parentIndex);

    int rightChild(int parentIndex);

    // MARK :- Insertion
    void insert(Node<T> data);

    void riseUp(int bottom);

    // MARK :- Deletion
    Node<T>* pop();

    void percolateDown(int root);

    // MARK :- Change Priority
    bool change(Node<T> previous, Node<T> updated);

    // MARK :- Heapify
    void buildHeap();

    // MARK :- topmost element
    Node<T>* top();

    // MARK:- Heap Sort
    void heapSort();

    // MARK :- Traversal
    void print();

    // MARK :- virtual functions
    virtual bool shouldSwap(int x, int y){};
};

template <class T> class MinHeap: public BaseHeap<T>{
public:
    bool shouldSwap(int x , int y);

    Node<T>* min();

    Node<T> kthSmallestElement(int k);
};

template <class T> class MaxHeap: public BaseHeap<T>{
public:

    bool shouldSwap(int x , int y);

    Node<T> kthLargestElement(int k);

    Node<T>* max();
};

#include "heap.cpp"

#endif //CLION_WORKSPACE_HEAP_H
