//
// Created by Yogesh Kumar on 2019-04-17.
//
#include "heap.h"

//MARK :- Base Class Methods

template <class T>
BaseHeap<T>::BaseHeap(){
    heap = new Node<T>[INITIAL_HEAP_SIZE];
    size = 0;
    capacity = INITIAL_HEAP_SIZE;
}

template <class T>
void BaseHeap<T>::buildFromArray(Node<T> array[],int size){
    while(capacity < size){
        expand();
    }
    for(int i = 0; i<size; i++){
        heap[i] = array[i];
    }
    this->size = size;
    this->capacity = size;
    buildHeap();
}

template <class T>
BaseHeap<T>::~BaseHeap(){
    delete[] heap;
}

template <class T>
void BaseHeap<T>::expand(){
    Node<T>* newHeap = new Node<T>[2*capacity];
    for(int i = 0; i<size; i++){
        newHeap[i] = heap[i];
    }
    delete[] heap;
    heap = newHeap;
    capacity*=2;
}

template <class T>
bool BaseHeap<T>::isEmpty(){
    return (size == 0);
}

// Parent/Child Index

template <class T>
int BaseHeap<T>::parentIndex(int childIndex){
    int parent = (childIndex-1)/2;
    if(parent < 0) return -1;
    return parent;
}

template <class T>
int BaseHeap<T>::leftChild(int parentIndex){
    int left = 2*parentIndex + 1;
    if(left > size - 1) return -1;
    return left;
}

template <class T>
int BaseHeap<T>::rightChild(int parentIndex){
    int right = 2*parentIndex + 2;
    if(right > size - 1) return -1;
    return right;
}

// Insertion
template <class T>
void BaseHeap<T>::insert(Node<T> data){
    if(size == capacity){
        // Double capacity
        expand();
    }
    // Add To end
    heap[size] = data;

    // Rise up
    riseUp(size++);
}

template <class T>
void BaseHeap<T>::riseUp(int bottom){
    int current, parent;
    Node<T> temp;
    current = bottom;

    while(true){
        parent = BaseHeap<T>::parentIndex(current);
        // heap[parent].priority < heap[current].priority => Max Heap
        if(parent != -1 && shouldSwap(parent,current)){
            // swap parent and current
            temp = BaseHeap<T>::heap[parent];
            BaseHeap<T>::heap[parent] = BaseHeap<T>::heap[current];
            BaseHeap<T>::heap[current] = temp;

            current = parent;
        }
        else{
            break;
        }
    }
}

// Change
template <class T>
bool BaseHeap<T>::change(Node<T> previous, Node<T> updated){
    // Search for it
    for(int i = 0; i<BaseHeap<T>::size; i++){
        if(BaseHeap<T>::heap[i].priority == previous.priority && BaseHeap<T>::heap[i].data == previous.data){
            BaseHeap<T>::heap[size] = heap[i]; // For temporary comparison;
            BaseHeap<T>::heap[i] = updated;
            if(shouldSwap(size,i)){
                BaseHeap<T>::riseUp(i);
            }
            else{
                BaseHeap<T>::percolateDown(i);
            }
            return true;
        }
    }
    return false;
}

// Deletion
template <class T>
Node<T>* BaseHeap<T>::pop(){
    if(size == 0) return nullptr;
    auto popped = new Node<T>;
    *popped = heap[0];

    // Bring last element to top
    heap[0] = heap[--size];

    // Heapify
    percolateDown(0);

    // Return popped value
    return popped;
}

template <class T>
void BaseHeap<T>::percolateDown(int root){
    int min, left, right, current;
    Node<T> temp;
    current = root;

    while(true){
        left = leftChild(current);
        right = rightChild(current);

        min = current;
        if(left != -1 && shouldSwap(min,left)){
            min = left;
        }
        if(right != -1 && shouldSwap(min,right)){
            min = right;
        }

        // If current if greater than both left and right child
        // Or if none of its child exist
        if(min == current) break;

        // swap elements at max and current
        temp = heap[min];
        heap[min] = heap[current];
        heap[current] = temp;
        current = min;
    }
}

// Heapify
template <class T>
void BaseHeap<T>::buildHeap(){
    for(int i = parentIndex(size-1); i >=0; i--){
        percolateDown(i);
    }
}

// Topmost element
template <class T>
Node<T>* BaseHeap<T>::top(){
    if(size != 0) return &heap[0];
    return nullptr;
}

// Heap Sort
template <class T>
void BaseHeap<T>::heapSort(){
    // MAX Heap => ascending Order
    // MIN Heap => descending order

    int originalSize = size;
    Node<T> temp;
    for(int i = 0;i<originalSize; i++) {
        temp = heap[0];
        heap[0] = heap[--size];
        heap[size] = temp;
        percolateDown(0);
    }
    size = originalSize;
}

// Traversal
template <class T>
void BaseHeap<T>::print(){
    for(int i = 0; i<size; i++){
        std::cout << heap[i].priority << " :: ";
        std::cout << heap[i].data << std::endl;
    }
}


// MARK :- Min Heap Methods

template <class T>
bool MinHeap<T>::shouldSwap(int x , int y){
    return BaseHeap<T>::heap[x].priority > BaseHeap<T>::heap[y].priority;
}

template <class T>
Node<T>* MinHeap<T>::min(){
    return BaseHeap<T>::top();
}

template <class T>
Node<T> MinHeap<T>::kthSmallestElement(int k){
    Node<T> element = *min();
    Node<T> kthElement;

    if(k > BaseHeap<T>::size || k <= 0){
        kthElement.priority = -1;
        return  kthElement;
    }

    MinHeap<T> auxHeap ;
    MinHeap<int> auxIndexHeap;

    int count = 0;
    int elementIndex = 0,left,right;

    auxHeap.insert(element);
    auxIndexHeap.insert({element.priority,elementIndex});


    while(true){
        element = *auxHeap.pop();
        elementIndex = auxIndexHeap.pop()->data;

        if(++count == k) {
            kthElement = element;
            return kthElement;
        }

        left = BaseHeap<T>::leftChild(elementIndex);
        right = BaseHeap<T>::rightChild(elementIndex);
        auxHeap.insert(BaseHeap<T>::heap[left]);
        auxHeap.insert(BaseHeap<T>::heap[right]);

        auxIndexHeap.insert({BaseHeap<T>::heap[left].priority,left});
        auxIndexHeap.insert({BaseHeap<T>::heap[right].priority,right});
    }
}


// MARK :- Max Heap Methods


template <class T>
bool MaxHeap<T>::shouldSwap(int x , int y){
    return BaseHeap<T>::heap[x].priority < BaseHeap<T>::heap[y].priority;
}

template <class T>
Node<T>* MaxHeap<T>::max(){
    return BaseHeap<T>::top();
}

template <class T>
Node<T> MaxHeap<T>::kthLargestElement(int k){
    Node<T> element = *max();
    Node<T> kthElement;

    if(k > BaseHeap<T>::size || k <= 0){
        kthElement.priority = -1;
        return  kthElement;
    }

    MaxHeap<T> auxHeap ;
    MaxHeap<int> auxIndexHeap;

    int count = 0;
    int elementIndex = 0,left,right;

    auxHeap.insert(element);
    auxIndexHeap.insert({element.priority,elementIndex});


    while(true){
        element = *auxHeap.pop();
        elementIndex = auxIndexHeap.pop()->data;

        if(++count == k) {
            kthElement = element;
            return kthElement;
        }

        left = BaseHeap<T>::leftChild(elementIndex);
        right = BaseHeap<T>::rightChild(elementIndex);
        auxHeap.insert(BaseHeap<T>::heap[left]);
        auxHeap.insert(BaseHeap<T>::heap[right]);

        auxIndexHeap.insert({BaseHeap<T>::heap[left].priority,left});
        auxIndexHeap.insert({BaseHeap<T>::heap[right].priority,right});
    }
}

