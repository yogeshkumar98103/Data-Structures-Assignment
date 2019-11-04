//
// Created by Yogesh Kumar on 2019-04-17.
//

#ifndef DS_LINKEDLIST_H
#define DS_LINKEDLIST_H

#include <cstddef>

template <typename data_t>
struct list_node{
    data_t data;
    list_node* next;
};

template <typename data_t>
class LinkedList{
    list_node<data_t> *head_, *tail_;
    size_t size_;

    public:

    typedef list_node<data_t> list_node_t;

    // MARK:- Constructor/Destructor
    LinkedList();
    LinkedList(list_node_t* head);
    LinkedList(list_node_t* head, size_t size);

    ~LinkedList();

    // MARK:- Insert Methods
    list_node_t* createNode(data_t data);

    void insertAtBeginning(data_t data);

    void insertAtEnd(data_t data);

    void insertAtPosition(data_t data, std::size_t position);

    void insertSorted(data_t data);

    // MARK:- Delete Methods
    data_t removeFromBeginning();

    data_t removeFromEnd();

    data_t removeAtPosition(std::size_t position);

    // MARK:- Getters
    size_t size() const;
    list_node_t* head() const;
    list_node_t* tail() const;

    // MARK:- Linked List Manipulation
    void reverse();

    list_node_t* findMiddle();

    list_node_t* reverseKBlocks(int k);

    // MARK:- Join Linked Lists
    
    
    static LinkedList<int> addDigitWise(const LinkedList<int> &list1, const LinkedList<int> &list2);

    void mergeSorted(const LinkedList &list1, const LinkedList &list2);


    // MARK:- Traversal Methods
    void print();

    void printReverse();

  private:

    // Helper function for printReverse
    void printReverseHelper(list_node<data_t>* current);

    // Helper function for reverseKBlocks
    list_node<data_t>* reverseKBlocksHelper(list_node<data_t> *current, int k);
};

#include "LinkedList.cpp"


#endif
