//
// Created by Yogesh Kumar on 2019-04-17.
//

#ifndef CLION_WORKSPACE_LINKEDLIST_H
#define CLION_WORKSPACE_LINKEDLIST_H

#include <cstddef>

template <typename data_t>
struct list_node{
    data_t data;
    list_node* next;
};

template <typename data_t>
class LinkedList{
    list_node<data_t> *head, *tail;
    std::size_t size;

    public:

    typedef list_node<data_t> list_node_t;

    // MARK:- Constructor/Destructor
    LinkedList();

    ~LinkedList();

    // MARK:- Insert Methods
    list_node_t* createNode(data_t data);

    void insertAtBeginning(data_t data);

    void insertAtEnd(data_t data);

    void insertAtPosition(data_t data, std::size_t position);

    // MARK:- Delete Methods
    data_t removeFromBeginning();

    data_t removeFromEnd();

    data_t removeAtPosition(std::size_t position);

    // MARK:- Sorting Methods
    void sort();

    void insertSorted(data_t data);

    // MARK:- Linked List Manipulation
    void reverse();

    void reverseRecursively();

    list_node_t* findMiddle();

    list_node_t* reverseKBlocks(int k);

    void reverseSecondHalf();

    // MARK:- Join Linked Lists
    
    
    static LinkedList<int> addDigitWise(const LinkedList<int> &list1, const LinkedList<int> &list2);

    void mergeSorted(const LinkedList &list1, const LinkedList &list2);


    // MARK:- Traversal Methods
    void print();

    void printReverse();

  private:

    // Helper function for reverseRecursively
    void reverseRecursivelyHelper(list_node<data_t> *current);

    // Helper function for printReverse
    void printReverseHelper(list_node<data_t>* current);

    // Helper function for reverseKBlocks
    list_node<data_t>* reverseKBlocksHelper(list_node<data_t> *current, int k);

    // Helper function for addDigitWise
    static list_node<int> *add(list_node<int>* h1, list_node<int>* h2, int s1, int s2);
};

#include "linkedList.cpp"


#endif
