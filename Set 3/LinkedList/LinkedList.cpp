//
// Created by Yogesh Kumar on 2019-04-17.
//

#include "LinkedList.h"

// MARK:- Constructor/Destructor
template <typename data_t>
LinkedList<data_t>::LinkedList(){
    head_ = tail_ = nullptr;
    size_ = 0;
}

template <typename data_t>
LinkedList<data_t>::LinkedList(list_node_t* head){
    this->head_ = head;
    this->tail_ = head;

    if(head == nullptr){
        this->size_ = 0;
    }
    else{
        this->size_ = 1;
        while(this->tail_->next != nullptr){
            this->tail_ = this->tail_->next;
            this->size_++;
        }
    }
}

template <typename data_t>
LinkedList<data_t>::LinkedList(list_node_t* head, size_t size){
    this->head_ = head;
    this->tail_ = head;

    if(head == nullptr){
        this->size_ = 0;
    }
    else{
        this->size_ = 1;
        for(size_t i = 1; (i < size_ && this->tail_->next != nullptr); i++){
            this->tail_ = this->tail_->next;
            this->size_++;
        }
    }
}


template <typename data_t>
LinkedList<data_t>::~LinkedList(){
    list_node_t* current = head_;
    list_node_t* next = head_;
    while(current != nullptr){
        next = current->next;
        delete current;
        current = next;
    }
}

// MARK:- Insert Methods
template <typename data_t>
list_node<data_t>* LinkedList<data_t>::createNode(data_t data){
    list_node_t* newNode = new list_node_t;
    newNode->data = data;
    newNode->next = nullptr;
    ++size_;
    return newNode;
}

template <typename data_t>
void LinkedList<data_t>::insertAtBeginning(data_t data){
    list_node_t* newNode = createNode(data);
    if(head_ == nullptr){
        head_ = tail_ = newNode;
        return;
    }

    newNode->next = head_;
    head_ = newNode;
}

template <typename data_t>
void LinkedList<data_t>::insertAtEnd(data_t data){
    list_node_t* newNode = createNode(data);
    if(head_ == nullptr){
        head_ = tail_ = newNode;
        return;
    }

    //Insert at End
    tail_->next = newNode;
    tail_ = newNode;
}

template <typename data_t>
void LinkedList<data_t>::insertAtPosition(data_t data, std::size_t position){
    if(position > size_+1 || position < 1) return;
    if(position == 1) insertAtBeginning(data);
    else if(position == size_+1) insertAtEnd(data);

    else{
        list_node_t* newNode = createNode(data);
        list_node_t* current = head_;

        // Go to position
        for(int i = 2; i<position; i++){
            current = current->next;
        }

        // Insert
        newNode->next = current->next;
        current->next = newNode;
    }
}

template <typename data_t>
void LinkedList<data_t>::insertSorted(data_t data){
    if(head_ == nullptr) {
        head_ = tail_ = createNode(data);
        return;
    }
    else if(data < head_->data) insertAtBeginning(data);
    else if(data > tail_->data) insertAtEnd(data);
    else{
        list_node_t* newNode = createNode(data);
        list_node_t* previous = head_, *current = head_->next;
        while(current != nullptr){
            if(data < current->data){
                previous->next = newNode;
                newNode->next = current;
                return;
            }
            previous = current;
            current = current->next;
        }
    }
}

// MARK:- Delete Methods

template <typename data_t>
data_t LinkedList<data_t>::removeFromBeginning(){
    if(head_ == nullptr)return nullptr;
    if(size_ == 1){
        delete head_;
        head_ = tail_ = nullptr;
        return;
    }

    data_t deletedValue = head_->data;
    list_node_t* start = head_;
    head_ = head_->next;
    delete start;
    --size_;
    return deletedValue;
}

template <typename data_t>
data_t LinkedList<data_t>::removeFromEnd(){
    if(head_ == nullptr) return nullptr;

    data_t deletedValue;

    if(size_ == 1){
        deletedValue = head_->data;
        delete head_;
        head_ = tail_ = nullptr;
        return deletedValue;
    }

    list_node_t* current = head_;

    for(int i = 2; i< size_; i++){
        current = current->next;
    }

    deletedValue = tail_->data;
    current->next = nullptr;
    delete tail_;
    tail_ = current;
    --size_;
    return deletedValue;
}

template <typename data_t>
data_t LinkedList<data_t>::removeAtPosition(std::size_t position){
    if(head_ == nullptr || position < 0 || position > size_) return;
    if(position == 0) return removeFromBeginning();
    if(position == size_) return removeFromEnd();

    list_node<data_t>* current = head_;
    for(int i = 2; i<position; i++){
        current = current->next;
    }
    list_node<data_t>* deletedNode = current->next;
    data_t deletedValue = deletedNode->data;
    current->next = deletedNode->next;
    delete deletedNode;
    return deletedValue;
}

// MARK:- Getters

template <typename data_t>
size_t LinkedList<data_t>::size() const{
    return this->size_;
}

template <typename data_t>
list_node<data_t>* LinkedList<data_t>::head() const{
    return this->head_;
}

template <typename data_t>
list_node<data_t>* LinkedList<data_t>::tail() const{
    return this->tail_;
}  

// MARK:- Linked List Manipulation

template <typename data_t>
void LinkedList<data_t>::reverse(){
    if(head_ == nullptr) return;
    list_node<data_t>* previous = nullptr, *next = nullptr;
    tail_ = head_;
    while(head_ != nullptr){
        next = head_->next;
        head_->next = previous;
        previous = head_;
        head_ = next;
    }
    head_ = previous;
}

template <typename data_t>
list_node<data_t>* LinkedList<data_t>::findMiddle(){
    if(head_ == nullptr) return nullptr;
    list_node<data_t> *middle,*end;
    middle = end = head_;
    bool isEven = false;
    while(end->next != nullptr){
        end = end->next;
        if(end->next != nullptr){
            end = end->next;
            middle = middle->next;
        }
        else{
            isEven = true;
        }
    }

    // If isEven is true the mid Nodes are middle and middle->next
    return middle;
}

template <typename data_t>
list_node<data_t>* LinkedList<data_t>::reverseKBlocks(int k){
    head_ = reverseKBlocksHelper(head_,k);
}

// MARK:- Join Linked Lists

template <typename data_t>
void LinkedList<data_t>::mergeSorted(const LinkedList<data_t> &list1, const LinkedList<data_t> &list2){

    head_ = tail_ = nullptr;
    size_ = 0;

    list_node<data_t>* head1 = list1.head_;
    list_node<data_t>* head2 = list2.head_;

    while(head1 != nullptr && head2 != nullptr){
        if(head1->data < head2->data){
            insertAtEnd(head1->data);
            head1 = head1->next;
        }
        else{
            insertAtEnd(head2->data);
            head2 = head2->next;
        }
    }
    while(head1 != nullptr){
        insertAtEnd(head1->data);
        head1 = head1->next;
    }
    while(head2 != nullptr){
        insertAtEnd(head2->data);
        head2 = head2->next;
    }
}


// MARK:- Traversal Methods
template <typename data_t>
void LinkedList<data_t>::print(){
    list_node<data_t>* current = head_;
    if(head_ == nullptr){
        std::cout << "Empty Linked List" << std::endl;
        return;
    }
    std::cout << current->data;
    current = current->next;
    while(current != nullptr){
        std::cout << " -> "<< current->data;
        current = current->next;
    }
    std::cout << std::endl;
}

template <typename data_t>
void LinkedList<data_t>::printReverse(){
    if(head_ == nullptr){
        std::cout << "Empty linked list" << std::endl;
        return;
    }
    std::cout << tail_->data;
    printReverseHelper(head_);
    std::cout << std::endl;
}

// Helper function for printReverse
template <typename data_t>
void LinkedList<data_t>::printReverseHelper(list_node<data_t>* current){
    if(current->next == nullptr) return;
    printReverseHelper(current->next);
    std::cout << " -> "<< current->data;
}

// Helper function for reverseKBlocks
template <typename data_t>
list_node<data_t>* LinkedList<data_t>::reverseKBlocksHelper(list_node<data_t> *current, int k){
    list_node<data_t> *nextHead, *previous = nullptr, *next, *start = current;

    if(current == nullptr) return nullptr;
    for(int i = 0; (i< k && current != nullptr); i++){
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    start->next = reverseKBlocksHelper(current,k);
    return previous;
}