//
// Created by Yogesh Kumar on 2019-03-07.
//

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <stdio.h>
using namespace std;

struct Node{
    int value;
    Node* left;
    Node* right;
};

class BinarySearchTree{
    Node *root;

public:

    // Constructor/Destructor
    BinarySearchTree(){
        root = nullptr;
    }
    BinarySearchTree(vector<int> A){
        root = arrayToBST(A,0,A.size());
    }
    ~BinarySearchTree(){
        deleteTree(root);
    }

    void deleteTree(Node *current){
        if(current->right) deleteTree(current->right);
        if(current->left) deleteTree(current->left);
        delete current;
    }

    Node *createNode(int value){
        Node *node = new Node;
        node->value = value;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

// Iterative Version
    void insertIteratively(int value){
        if(root == nullptr){
            root = createNode(value);
            return;
        }

        Node*current = root;

        while(true){
            if(current->value <= value){
                if(current->right != nullptr)current = current->right;
                else {
                    current->right = createNode(value);
                    break;
                }
            }
            else{
                if(current->left != nullptr) current = current->left;
                else{
                    current->left = createNode(value);
                    break;
                }
            }
        }
    }

    void searchIteratively(int value){
        Node *current = root;
        while(nullptr != current){
            if(current->value == value){
                cout << "Element Found" << endl;
                return;
            }
            else current = current->value < value ? current->right : current->left;
        }
        cout << "Not Found" << endl;
    }

    Node* maximumIteratively(){
        if(root == nullptr) {
            cout << "Empty Tree" << endl;
            return nullptr;
        }
        Node *current = root;
        while(current->right != nullptr){
            current = current->right;
        }
        cout << "Maximum : " << current->value << endl;
        return current;
    }

    void minimumIterative(){
        if(root == nullptr) {
            cout << "Empty Tree" << endl;
            return;
        }
        Node *current = root;
        while(current->left != nullptr){
                current = current->left;
        }
        cout << "Minimum : " << current->value << endl;
    }

        // Traversal
    void preOrderIterative(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
            return;
        }

        stack<Node*> s;
        s.push(root);
        Node* current;
        vector<int> result;
        while(!s.empty()){
            current = s.top();
            result.push_back(current->value);
            s.pop();
            if(current->right != nullptr){
                s.push(current->right);
            }
            if(current->left != nullptr){
                s.push(current->left);
            }
        }

        for(auto ele: result){
            cout << ele << " , ";
        }
        cout << "\n";
    }
    void inOrderIterative(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
            return;
        }

        stack<Node*> s;
        vector<int> result;
        Node* current = root;

        while(true){
            while(current != nullptr){
                s.push(current);
                current = current->left;
            }

            if(s.empty()) break;

            current = s.top();
            s.pop();
            result.push_back(current->value);
            current = current->right;
        }

        for(auto ele: result){
            cout << ele << " , ";
        }
        cout << "\n";
    }
    void postOrderIterative(){
        if(root == nullptr) {
            cout << "Empty Tree" << endl;
            return;
        }

        stack<Node*> s;
        vector<int> result;
        Node* current = root, *previous = nullptr;

        do{
            while(current != nullptr){
                s.push(current);
                current = current->left;
            }

            while(current == nullptr && !s.empty()){
                current = s.top();
                if(current->right == nullptr || current->right == previous){
                    result.push_back(current->value);
                    s.pop();
                    previous = current;
                    current = nullptr;
                }
                else{
                    current = current->right;
                }
            }
        }while(!s.empty());

        for(auto ele: result){
            cout << ele << " , ";
        }
        cout << "\n";

    }
    void levelOrderIterative(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
            return;
        }

        queue<Node*> q;
        vector<int> result;
        Node* current;
        q.push(root);

        while(!q.empty()){
            current = q.front();
            result.push_back(current->value);
            q.pop();
            if(current->left != nullptr){
                q.push(current->left);
            }
            if(current->right != nullptr){
                q.push(current->right);
            }
        }

        for(int ele: result){
            cout << ele << " , ";
        }
        cout << "\n";
    }

// Recursive Version
    void insertRecursive(int value){
        if(root == nullptr){
            root = createNode(value);
            return;
        }
        insertHelper(value,root);
    }

    void deleteRecursive(int value){
        deleteHelper(root,value);
    }

    void LCARecursive(int value1 , int value2){
        Node* lca;
        if(value1 <= value2) lca = LCAHelper(root,value1,value2);
        else lca = LCAHelper(root, value2, value1);
        cout << "Least Common Ancestor : " << lca->value << endl;
    }

        // Traversal
    void preOrderRecursive(){
        preOrderHelper(root);
        cout << "\n";
    }
    void inOrderRecursive(){
        inOrderHelper(root);
        cout << "\n";
    }
    void postOrderRecursive(){
        postOrderHelper(root);
        cout << "\n";
    }

    void NthSmallestRecursive(int n){
        if(n < 1){
            cout << "n is smaller than 1" << endl;
            return;
        }

        int count = 0;
        Node *small = NthSmallestHelper(root,n,&count);

        if(small == nullptr){
            cout << "n is too large" << endl;
        }
        else{
            cout << n << " th smallest value is : " <<  small->value << endl;
        }

    }

private:
    // Helper function for insert
    void insertHelper(int value, Node*current){
        if(value >= current->value){
            if(current->right != nullptr) insertHelper(value, current->right);
            else current->right = createNode(value);
        }
        else{
            if(current->left != nullptr) insertHelper(value, current->left);
            else current->left = createNode(value);
        }
    }

    // Helper function for preOrderRecursive
    void preOrderHelper(Node* current){
        if(current != nullptr){
            cout << current->value << " , ";
            preOrderHelper(current->left);
            preOrderHelper(current->right);
        }
    }

    // Helper function for inOrderRecursive
    void inOrderHelper(Node* current){
        if(current != nullptr){
            inOrderHelper(current->left);
            cout << current->value << " , ";
            inOrderHelper(current->right);
        }
    }

    // Helper function for postOrderRecursive
    void postOrderHelper(Node* current){
        if(current != nullptr){
            postOrderHelper(current->left);
            postOrderHelper(current->right);
            cout << current->value << " , ";
        }
    }

    // Helper function for maximumRecursively
    Node *maximumHelper(Node *current){
        if(current != nullptr){
            if(current->right) return maximumHelper(current->right);
            return current;
        }
        return nullptr;
    }

    // Helper function for deleteRecursive
    Node* deleteHelper(Node *current, int value){
        if(current == nullptr) return nullptr;
        if(current->value == value){
            // delete this
            if(current->right == nullptr && current->left == nullptr){
                // Leaf node
                delete current;
                return nullptr;
            }
            Node *temp;
            if(current->right == nullptr && current->left != nullptr){
                temp = current->left;
                delete current;
                return temp;
            }
            else if(current->left == nullptr && current->right != nullptr){
                temp = current->right;
                delete current;
                return temp;
            }
            else{
                temp = maximumHelper(current->left);
                current->value = temp->value;
                temp->value = value;
                current->left = deleteHelper(current->left,value);
                return current;
            }
        }
        else{
            if(current->value < value){
                current->right = deleteHelper(current->right,value);
            }
            else{
                current->left = deleteHelper(current->left,value);
            }
            return current;
        }
    }

    // Helper function for LCARecursive
    Node* LCAHelper(Node* current, int value1, int value2){
        if(current == nullptr) return nullptr;
        if(value1 < current->value && value2 > current->value){
            return current;
        }
        if(value1 > current->value) return LCAHelper(current->right, value1, value2);
        else return LCAHelper(current->left, value1, value2);
    }

    Node* arrayToBST(vector<int> A, int start, int end){
        if(start == end) return nullptr;
        int mid = (end+start)/2;
        Node *current = createNode(A[mid]);
        current->left = arrayToBST(A,start,mid);
        current->right = arrayToBST(A,mid+1,end);
        return current;
    }

    // Helper function for NthSmallest
    Node* NthSmallestHelper(Node *current, int n, int* count){
        if(current == nullptr) return nullptr;
        Node * left = NthSmallestHelper(current->left, n, count);
        if(left != nullptr){
            return left;
        }
        if(++(*count) == n){
            return current;
        }
        return NthSmallestHelper(current->right,n,count);
    }

};