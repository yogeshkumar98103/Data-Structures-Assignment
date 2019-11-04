//
// Created by Yogesh Kumar on 2019-04-17.
//

#ifndef LIBRARY_QUEUE_H
#define LIBRARY_QUEUE_H

#define INITIAL_QUEUE_SIZE 20
#include <iostream>

template <typename T>
class queue{
    T *queue_;
    int front_;
    int back_;
    int size_;
    T default_value_; // This will be returned when we pop in an empty queue
    bool default_value_set;

  public:

    // ---------------- Constructor/Destructor ------------------
    queue();
    queue(T value);
    ~queue();

    // ---------------- Data insertion/Retrieval ----------------
    void enQueue(T ele);
    T deQueue();

    // ----------------------- Front/Back -----------------------
    T frontElement();
    T backElement();

    bool isEmpty();

    // --------------------- Data Traversal ---------------------
    void print();

  private:
    void initialize();

    // ---------------------- Expand Queue ----------------------
    void expandLinear();

    void expandCircular();
};

#include "queue.cpp"

#endif // LIBRARY_QUEUE_H
