#include <iostream>
#include "LinkedList.h"

list_node<int>* add(list_node<int>* h1, list_node<int>* h2, int s1, int s2){

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

LinkedList<int> addDigitWise(const LinkedList<int> &list1, const LinkedList<int> &list2){
    int s1 = list1.size();
    int s2 = list2.size();

    LinkedList<int> list3;
    list_node<int>* head = add(list1.head(), list2.head(), s1, s2);
    size_t size = (s1 > s2) ? s1 : s2;

    int remainder = head->data/10;

    if(remainder != 0){
        auto newNode = new list_node<int>;
        newNode->data = remainder;
        newNode->next = head;
        head = newNode;
        size++;
    }

    (head->next->data) %= 10;

    return LinkedList<int>(head, size);
}

int main(){
    LinkedList<int> num1;
    LinkedList<int> num2;

    num1.insertAtEnd(9);
    num1.insertAtEnd(8);
    num1.insertAtEnd(7);

    num2.insertAtEnd(4);
    num2.insertAtEnd(2);
    num2.insertAtEnd(7);

    auto sum = addDigitWise(num1, num2);
    sum.print();
    return 0;
}