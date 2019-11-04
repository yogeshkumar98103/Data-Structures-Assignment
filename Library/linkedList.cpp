//
// Created by Yogesh Kumar on 2019-04-17.
//

#include "linkedList.h"

// MARK:- Constructor/Destructor
template <typename data_t>
LinkedList<data_t>::LinkedList(){
    head = tail = nullptr;
    size = 0;
}

template <typename data_t>
LinkedList<data_t>::~LinkedList(){
    list_node_t* current = head;
    list_node_t* next = head;
    while(current != nullptr){
        next = current->next;
        delete current;
        current = next;
    }
}

// MARK:- Insert Methods
template <typename data_t>
list_node<data_t>* LinkedList<data_t>::createNode(data_t data){
    list_node_t newNode = new list_node_t;
    newNode->data = data;
    newNode->next = nullptr;
    ++size;
    return newNode;
}

template <typename data_t>
void LinkedList<data_t>::insertAtBeginning(data_t data){
    list_node_t* newNode = createNode(data);
    if(head == nullptr){
        head = tail = newNode;
        return;
    }

    newNode->next = head;
    head = newNode;
}

template <typename data_t>
void LinkedList<data_t>::insertAtEnd(data_t data){
    list_node_t* newNode = createNode(data);
    if(head == nullptr){
        head = tail = newNode;
        return;
    }

    //Insert at End
    tail->next = newNode;
    tail = newNode;
}

template <typename data_t>
void LinkedList<data_t>::insertAtPosition(data_t data, std::size_t position){
    if(position > size+1 || position < 1) return;
    if(position == 1) insertAtBeginning(data);
    else if(position == size+1) insertAtEnd(data);

    else{
        list_node_t* newNode = createNode(data);
        list_node_t* current = head;

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
    if(head == nullptr) {
        head = tail = createNode(data);
        return;
    }
    else if(data < head->data) insertAtBeginning(data);
    else if(data > tail->data) insertAtEnd(data);
    else{
        list_node_t* newNode = createNode(data);
        list_node_t* previous = head, *current = head->next;
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
    if(head == nullptr)return nullptr;
    if(size == 1){
        delete head;
        head = tail = nullptr;
        return;
    }

    data_t deletedValue = head->data;
    list_node_t* start = head;
    head = head->next;
    delete start;
    --size;
    return deletedValue;
}

template <typename data_t>
data_t LinkedList<data_t>::removeFromEnd(){
    if(head == nullptr) return nullptr;

    data_t deletedValue;

    if(size == 1){
        deletedValue = head->data;
        delete head;
        head = tail = nullptr;
        return deletedValue;
    }

    list_node_t* current = head;

    for(int i = 2; i< size; i++){
        current = current->next;
    }

    deletedValue = tail->data;
    current->next = nullptr;
    delete tail;
    tail = current;
    --size;
    return deletedValue;
}

template <typename data_t>
data_t LinkedList<data_t>::removeAtPosition(std::size_t position){
    if(head == nullptr || position < 0 || position > size) return;
    if(position == 0) return removeFromBeginning();
    if(position == size) return removeFromEnd();

    list_node<data_t>* current = head;
    for(int i = 2; i<position; i++){
        current = current->next;
    }
    list_node<data_t>* deletedNode = current->next;
    data_t deletedValue = deletedNode->data;
    current->next = deletedNode->next;
    delete deletedNode;
    return deletedValue;
}

// MARK:- Linked List Manipulation

template <typename data_t>
void LinkedList<data_t>::reverse(){
    if(head == nullptr) return;
    list_node<data_t>* previous = nullptr, *next = nullptr;
    tail = head;
    while(head != nullptr){
        next = head->next;
        head->next = previous;
        previous = head;
        head = next;
    }
    head = previous;
}

template <typename data_t>
list_node<data_t>* LinkedList<data_t>::findMiddle(){
    if(head == nullptr) return nullptr;
    list_node<data_t> *middle,*end;
    middle = end = head;
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
    head = reverseKBlocksHelper(head,k);
}

template <typename data_t>
void LinkedList<data_t>::reverseSecondHalf(){
    // Finding middle
    list_node<data_t>* middle = findMiddle();
    list_node<data_t>* temp = middle;

    // Reverse the second half
    if(middle == nullptr) return;
    list_node<data_t>* previous = nullptr, *next = nullptr;
    middle = middle->next;
    tail = middle;
    while(middle != nullptr){
        next = middle->next;
        middle->next = previous;
        previous = middle;
        middle = next;
    }
    temp->next = nullptr;

    // Make new links
    list_node<data_t>* second = previous;
    list_node<data_t>* first = head;
    list_node<data_t>* temp2;
    while(second != nullptr){
        temp = first->next;
        temp2 = second->next;
        first->next = second;
        second->next = temp;
        first = temp;
        second = temp2;
    }
}

// MARK:- Join Linked Lists
// Linked Lists must have int type
template <typename data_t>
LinkedList<int> LinkedList<data_t>::addDigitWise(const LinkedList<int> &list1, const LinkedList<int> &list2){
    int s1 = list1.size;
    int s2 = list2.size;

    LinkedList<int> list3;
    list3.head = add(list1.head,list2.head,s1,s2);
    list3.size = (s1 > s2) ? s1 : s2;

    int remainder = list3.head->data/10;

    if(remainder != 0){
        auto newNode = new list_node<data_t>;
        newNode->data = remainder;
        newNode->next = list3.head;
        list3.head = newNode;
        list3.size++;
    }

    (list3.head->next->data)%=10;

    //Setting tail
    list_node<int>* current = list3.head;
    for(int i = 1; i<list3.size; i++){
        current = current->next;
    }
    list3.tail = current;

    return list3;
}

template <typename data_t>
void LinkedList<data_t>::mergeSorted(const LinkedList<data_t> &list1, const LinkedList<data_t> &list2){

    head = tail = nullptr;
    size = 0;

    list_node<data_t>* head1 = list1.head;
    list_node<data_t>* head2 = list2.head;

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
    list_node<data_t>* current = head;
    if(head == nullptr){
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
    if(head == nullptr){
        std::cout << "Empty linked list" << std::endl;
        return;
    }
    std::cout << tail->data;
    printReverseHelper(head);
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

// Helper function for addDigitWise
template <typename data_t>
list_node<int>* LinkedList<data_t>::add(list_node<int>* h1, list_node<int>* h2, int s1, int s2){

    if(h1 == nullptr && h2 == nullptr) return nullptr;

    int sum = 0;
    auto current = new list_node<int>;

    if(s1 == s2){
        current->next = add(h1->next,h2->next,s1-1,s2-1);
        sum = h1->data + h2->data;
        if(current->next != nullptr){
            sum += (current->next->data)/10;
            current->next->data %= 10;
        }
    }
    else if(s1>s2){
        current->next = add(h1->next,h2,s1-1,s2);
        sum = h1->data;
        if(current->next != nullptr){
            sum += (current->next->data)/10;
            current->next->data %= 10;
        }
    }
    else{
        current->next = add(h1,h2->next,s1,s2-1);
        sum = h2->data;
        if(current->next != nullptr){
            sum += (current->next->data)/10;
            current->next->data %= 10;
        }
    }


    current->data = sum;

    return current;
}
