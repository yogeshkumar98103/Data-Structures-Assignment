//
// Created by Yogesh Kumar on 2019-04-17.
//

#include "queue.h"

// Constructor/Destructor
template <typename T>
queue<T>::queue(){
    initialize();
    default_value_set = false;
}

template <typename T>
queue<T>::queue(T value){
    initialize();
    default_value_ = value;
    default_value_set = true;
}

template <typename T>
void queue<T>::initialize(){
    queue_ = new T[INITIAL_QUEUE_SIZE];
    front_ = -1;
    back_ = -1;
    size_ = INITIAL_QUEUE_SIZE;
}

template <typename T>
queue<T>::~queue(){
    delete[] queue_;
}

// Expand queue
template <typename T>
void queue<T>::expandLinear(){
    T *newqueue;
    size_ *= 2;
    newqueue = new T[size_];
    for(int i = 0; i<size_/2; i++){
        newqueue[i] = queue_[i];
    }
    delete[] queue_;
    queue_ = newqueue;
}

template <typename T>
void queue<T>::expandCircular(){
    T *newqueue;
    size_ *= 2;
    newqueue = new T[size_];
    for(int i = 0; i<=back_; i++){
        newqueue[i] = queue_[i];
    }
    for(int i = front_; i<size_/2; i++){
        newqueue[i+size_/2] = queue_[i];
    }
    delete[] queue_;
    queue_ = newqueue;
    front_ += size_/2;
}

// Data insertion/Retrieval
template <typename T>
void queue<T>::enQueue(T ele){
    if(front_ == back_ + 1) expandCircular();
    if(front_ == 0 && back_ == size_ - 1) expandLinear();

    if(front_ > 0 && back_ == size_ - 1){       // Round back
        back_ = 0;
        queue_[back_] = ele;
    }
    else if(front_ == -1){
        front_ = back_ = 0;
        queue_[front_] = ele;
    }
    else if(back_ < size_ - 1){
        queue_[++back_] = ele;
    }
}

template <typename T>
T queue<T>::deQueue(){
    T ret;
    if(front_ == size_ - 1){
        ret = queue_[size_ - 1];
        front_ = 0;
    }
    else if(back_ == front_){
        ret = queue_[back_];
        back_ = front_ = -1;
    }
    else if(front_ >= 0){
        ret = queue_[front_++];
    }
    else{
        if(default_value_set) ret = default_value_;
        else throw "Empty Queue";
    }
    return ret;
}

// Front/ Back
template <typename T>
T queue<T>::frontElement(){
    return queue_[front_];
}

template <typename T>
T queue<T>::backElement(){
    return queue_[back_];
}

template <typename T>
bool queue<T>::isEmpty(){
    return (front_ == -1);
}

// Data Traversal
template <typename T>
void queue<T>::print(){
    std::cout << "Size - " << size_ << "  ::  ";
    if(front_ == -1){
        std::cout << "Empty queue\n";
        return;
    }

    std::cout << " (FRONT) " << queue_[front_];

    if(front_ > back_){
        for(int i = front_ + 1; i < size_; i++){
            std::cout << " -> " << queue_[i];
        }
        for(int i = 0; i<=back_; i++){
            std::cout << " -> " << queue_[i];
        }
    }
    else{
        for(int i = front_ + 1; i <= back_; i++){
            std::cout << " -> " << queue_[i];
        }
    }

    std::cout << " (REAR) "<< std::endl;
}

