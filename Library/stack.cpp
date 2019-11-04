//
// Created by Yogesh Kumar on 2019-04-17.
//

#include "stack.h"

template <typename T>
stack<T>::stack(){
    initialize();
}

template <typename T>
stack<T>::stack(T value){
    initialize();
    default_value_ = value;
}

template <typename T>
void stack<T>::initialize(){
    top_ = -1;
    size_ = INITIAL_STACK_SIZE;
    stack_ = new T[INITIAL_STACK_SIZE];
}

template <typename T>
stack<T>::~stack(){
    delete[] stack_;
}

template <typename T>
void stack<T>::expand(){
    size_ *= 2;
    T *newstack = new T[size_];
    for(int i = 0; i< size_/2; i++){
        newstack[i] = stack_[i];
    }
    delete[] stack_;
    stack_ = newstack;
}

template <typename T>
void stack<T>::push(T item){
    if(top_ == size_ - 1) expand();
    stack_[++top_] = item;
}

template <typename T>
T stack<T>::pop(){
    if(top_ == -1) return default_value_;
    T ret = stack_[top_];
    top_--;
    return ret;
}

template <typename T>
T stack<T>::top(){
    return stack_[top_];
}

template <typename T>
bool stack<T>::isEmpty(){
    return (top_ == -1);
}

template <typename T>
void stack<T>::print(){
    std::cout << "Size - " << size_ << "  ::  ";
    if(top_ == -1){
        std::cout << "Empty stack" << std::endl;
        return;
    }
    std::cout << stack_[0];
    for(int i = 1; i <=top_; ++i){
        std::cout << " <- " << stack_[i] ;
    }
    std::cout << " (TOP) " << std::endl;
}
