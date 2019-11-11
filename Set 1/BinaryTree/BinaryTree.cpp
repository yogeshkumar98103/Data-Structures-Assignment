//
// Created by Yogesh Kumar on 2019-03-04.
//

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

struct Node{
    int value;
    Node *left;
    Node *right;
};

class BinaryTree{
    Node *root;

public:

    BinaryTree(){
        this->root = nullptr;
        // root = newNode(1);
        // root->left = newNode(2);
        // root->right = newNode(3);
        // root->left->left = newNode(4);
        // root->left->right = newNode(5);
        // root->right->left = newNode(6);
        // root->right->right = newNode(7);
    }

    Node* newNode(int value){
        Node* node = new Node;
        node->value = value;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }


// Iterative Versions
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
            s.top();
            s.pop();
            result.push_back(current->value);
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
                    s.top();
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
            q.pop();
            result.push_back(current->value);
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

    void findMaxIterative(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
            return;
        }

        // Level order traversal
        queue<Node*> q;
        int max = INT_MIN;
        Node* current;
        q.push(root);


        while(!q.empty()){
            current = q.front();
            if(current->value > max) max = current->value;
            q.pop();
            if(current->left != nullptr){
                q.push(current->left);
            }
            if(current->right != nullptr){
                q.push(current->right);
            }
        }

        cout << "Maximum : " << max << endl;
    }

    void insert(int value){
        if(root == nullptr){
            root = newNode(value);
            return;
        }
        queue<Node*> q;
        Node* current;
        q.push(root);

        while(!q.empty()){
            current = q.front();
            q.pop();
            if(current->left != nullptr){
                q.push(current->left);
            }
            else{
                current->left = newNode(value);
                break;
            }
            if(current->right != nullptr){
                q.push(current->right);
            }
            else{
                current->right = newNode(value);
                break;
            }
        }

    }

    void levelOrderReverse(){
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
            q.pop();
            result.push_back(current->value);
            if(current->right != nullptr){     // Just change the order here
                q.push(current->right);
            }
            if(current->left != nullptr){
                q.push(current->left);
            }

        }

        for(int i = result.size()-1; i>=0; i--){
            cout << result[i] << " , ";
        }
        cout << "\n";

    }

    void findHeightIterative(){

        int level = 0;
        if(root != nullptr){

            queue<Node*> q;
            Node* current;
            q.push(root);
            q.push(nullptr);

            while(!q.empty()){
                current = q.front();
                q.pop();
                if(current == nullptr){
                    level++;
                    if(!q.empty()){
                        q.push(nullptr);
                    }
                }
                else {
                    if(current->left != nullptr){
                        q.push(current->left);
                    }
                    if(current->right != nullptr) {
                        q.push(current->right);
                    }
                }
            }
        }

        cout << "Height : " << level << endl;
    }

    void deepestNodeIterative(){
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
        cout << "Deppest Node : " << current->value << endl;
    }

    void findLeavesIterative(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
            return;
        }

        queue<Node*> q;
        int leaves = 0;
        Node* current;
        q.push(root);

        while(!q.empty()){
            current = q.front();
            q.pop();
            if(current->left == nullptr && current->right == nullptr){
                leaves++;
            }
            if(current->left != nullptr){
                q.push(current->left);
            }
            if(current->right != nullptr){
                q.push(current->right);
            }
        }
        cout << "Leaves : " << leaves << endl;
    }

    void zigzagTraversal(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
            return;
        }
        stack<Node*> currentLevel;
        stack<Node*> nextLevel;
        vector<int> result;
        currentLevel.push(root);
        Node* current;
        bool leftToRight = true;

        while(!currentLevel.empty()){
            current = currentLevel.top();
            currentLevel.pop();
            result.push_back(current->value);
            if(leftToRight){
                if(current->left) nextLevel.push(current->left);
                if(current->right) nextLevel.push(current->right);
            }
            else{
                if(current->right) nextLevel.push(current->right);
                if(current->left) nextLevel.push(current->left);
            }
            if(currentLevel.empty()){
                leftToRight = !leftToRight;
                // Swap stacks
                currentLevel = nextLevel;
                while(!nextLevel.empty()){
                    nextLevel.pop();
                }
            }
        }

        for(auto ele: result){
            cout << ele << " , ";
        }
        cout << "\n";
    }

// Recursive Version


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

    void findMaxRecursive(){
        cout << "Maximum : " << findMaxHelper(root) << endl;
    }

    void findSizeRecursive(){
        cout << "Size : " << findSizeHelper(root) << endl;
    }

    void deleteTreeRecursive(){
        deleteTreeHelper(root);
    }

    void findHeightRecursive(){
        cout << "Height : " << findHeightHelper(root) << endl;
    }

    void findDiameterRecursive(){
        int diameter = 0;
        findDiameterHelper(root,&diameter);
        cout << "Diameter : " << diameter << endl;
    }

    void allPathsToLeaves(){
        vector<int> path;
        allPathsHelper(root,path);
    }

    void checkSum(int sum){
        bool check = checkSumHelper(root,sum);
        if(check){
            cout << "Such sum exists" << endl;
        }
        else{
            cout << "Nope it doesn't" << endl;
        }
    }

    void mirrorTreeRecursive(){
        if(root == nullptr) return;
        mirrorTreeHelper(root);
    }

    void leastCommonAncestor(){
        Node *node1 = root->right->left->left;
        Node *node2 = root->left->right;
        if(root == nullptr) return;
        Node* LCA = LCAHelper(root,node1,node2);
        cout << "Least Common ancestor is : " << LCA->value << endl;
    }

    void buildTree(vector<int> preOrder, vector<int> inOrder){
        deleteTreeRecursive();
        root = buildTreeHelper(preOrder,0,preOrder.size(),inOrder,0,inOrder.size());
    }

    void allAncestors(){
        Node* node = root->right->right;
        stack<Node*> ancestors;
        allAncestorsHelper(root,node,ancestors);
        while(!ancestors.empty()){
            cout << ancestors.top()->value << " , ";
            ancestors.pop();
        }
        cout << "\n";
    }

private:
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

    // Helper function for findMax
    int findMaxHelper(Node* current){
        if(current != nullptr){
            int left = findMaxHelper(current->left);
            int right = findMaxHelper(current->right);
            if(current->value > left && current->value > right){
                return current->value;
            }
            if(left > current->value && left > right){
                return  left;
            }
            return right;
        }
        return INT_MIN;
    }

    // Helper function for findSize
    int findSizeHelper(Node* current){
        if(current != nullptr){
            return 1 + findSizeHelper(current->left) + findSizeHelper(current->right);
        }
        return 0;
    }

    // Helper function for deleteTreeRecursive
    void deleteTreeHelper(Node *current){
        if(current != nullptr){
            deleteTreeHelper(current->left);
            deleteTreeHelper(current->right);
            delete current;
        }
    }

    // Helper function for findHeight
    int findHeightHelper(Node *current){
        if(current != nullptr){
           int left = findHeightHelper(current->left);
           int right = findHeightHelper(current->right);
           if(left > right){
               return left + 1;
           }
           return right + 1;
        }
        return 0;
    }

    // Helper function for findDiameter
    int findDiameterHelper(Node *current,int *dia){
        if(current == nullptr) return 0;
        int left = findDiameterHelper(current->left,dia);
        int right = findDiameterHelper(current->right,dia);
        if(left + right > *dia){
            *dia = left + right;
        }
        return max(left,right) + 1;
    }

    // Helper function for allPathsToLeaves
    void allPathsHelper(Node *current, vector<int> path){
        if(current->left == nullptr && current->right == nullptr){
            path.push_back(current->value);
            // Print path
            cout << path[0];
            for(int i = 1; i<path.size(); i++){
                cout << " -> " << path[i];
            }
            cout << "\n";
            path.pop_back();
            return;
        }
        path.push_back(current->value);
        if(current->left != nullptr){
            allPathsHelper(current->left,path);
        }
        if(current->right != nullptr){
            allPathsHelper(current->right,path);
        }
    }

    // Helper function for checkSum
    bool checkSumHelper(Node *current, int sum){
        if(current == nullptr){
            return (sum == 0);
        }
        sum -= current->value;
            bool left = checkSumHelper(current->left, sum);
            bool right = checkSumHelper(current->right, sum);
            return (left || right);
    };

    // Helper function for mirrorTree
    void mirrorTreeHelper(Node *current){
        Node *temp = current->left;
        current->left = current->right;
        current->right = temp;

        if(current->left != nullptr){
            mirrorTreeHelper(current->left);
        }
        if(current->right != nullptr){
            mirrorTreeHelper(current->right);
        }
    }

    // Helper function for leastCommonAncestor
    Node* LCAHelper(Node *current, Node* node1, Node* node2){
        if(current != nullptr){
            if(current == node1){
                return node1;
            }
            if(current == node2){
                return node2;
            }
            Node* left = LCAHelper(current->left,node1,node2);
            Node* right = LCAHelper(current->right,node1,node2);
            if((left == node1 && right == node2) || (left == node2 && right == node1)){
                // This is LCA
                return current;
            }
            if(left != nullptr){
                return left;
            }
            if(right != nullptr){
                return right;
            }
            return nullptr;
        }
        return nullptr;
    }

    // Helper function for buildTree
    Node* buildTreeHelper(vector<int> preOrder,int s1,int e1, vector<int> inOrder, int s2,int e2){
        if(s1 == e1){
            return nullptr;
        }
        Node *current = new Node;
        current->value = preOrder[s1];
        for(int i = s2; i<e2; i++){
            if(inOrder[i] == preOrder[s1]){
                current->left = buildTreeHelper(preOrder,s1+1,s1+i+1-s2,inOrder,s2,i);
                current->right = buildTreeHelper(preOrder,s1+1+i-s2,e1,inOrder,i+1,e2);
                return current;
            }
        }
        return nullptr;
    }

    // Helper function for allAncestors
    bool allAncestorsHelper(Node *current, Node *node, stack<Node*> & ancestors){
        if(current != nullptr){
            if(current == node){
                ancestors.push(current);
                return true;
            }
            bool left = allAncestorsHelper(current->left,node,ancestors);
            bool right = allAncestorsHelper(current->right,node,ancestors);
            if(left || right){
                ancestors.push(current);
                return true;
            }
        }
        return false;
    }
};