//
// Created by Yogesh Kumar on 2019-04-17.
//

#ifndef LIBRARY_STACK_H
#define LIBRARY_STACK_H

#define INITIAL_STACK_SIZE 5

#include <iostream>

template <typename T>
class stack{
    T* stack_;
    int top_;
    int size_;
    T default_value_;

  public:
    /**
     * Default Constructor
     */
    stack();

    /**
     * Overloaded Constructor
     */
    stack(T value);

    // Destructor
    ~stack();

    /**
     * Expand dynamic array to incorporate more elements
     */
    void expand();

    /**
     * Add new element to stack
     */
    void push(T item);

    /**
     * Delete topmost element in the stack
     */
    T pop();

    /**
     * Get topmost element in the stack
     */
    T top();

    /**
     * Check whether stack is empty
     */
    bool isEmpty();

    /** 
     * Getter for Default Value
     */
    T getDefaultValue();

    /**
     * Prints the contents of stack
     */
    void print();

  private: 
    /**
     * Helper function which performs some initial setup
     */
    void initialize();
};

#include "stack.cpp"

#endif // LIBRARY_STACK_H
