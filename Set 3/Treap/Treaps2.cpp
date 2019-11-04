#include <iostream>
#include <random>
#include <chrono>
using namespace std;

enum Access_Behaviour{
    Error_If_Not_Found,
    Create_If_Not_Found
};

template <typename tree_key_t, typename value_t, typename heap_key_t>
class Base_Treap;

template <typename tree_key_t, typename value_t, typename heap_key_t>
struct treap_node{
    tree_key_t tree_key;
    heap_key_t heap_key;
    value_t value;

    friend class Base_Treap<tree_key_t, value_t, heap_key_t>;

  private:
    treap_node* left_child;
    treap_node* right_child;
    treap_node* parent;
};

template <typename tree_key_t, typename value_t, typename heap_key_t>
class Base_Treap{
protected:
    typedef treap_node<tree_key_t, value_t, heap_key_t> treap_node_t;

    treap_node_t* root;                     // Root Of Treap
    size_t node_count;                      // Number Of Nodes In treap

    bool cache_available;                   // Whether there is a cached node
    treap_node_t* cached_node;              // Stores currently visited node

    static treap_node_t* end_node;          // Sentinal denoting the end of treap

    Access_Behaviour access_behaviour;      // Determines behavious when searched key is not found
    value_t default_value;                  // Default value stored when no value is provided

  public:
    class iterator;

    iterator& search(const tree_key_t& tree_key){
        if( (this->cache_available && this->cached_node->tree_key == tree_key) || this->search_key(tree_key, nullptr)){
            return save = this->cached_node;
        }
        return save = this->end_node;
    }

  protected:
    iterator begin_itr;                     // Stores the iterator to node with smallest tree key
    iterator end_itr;                       // Stores the iterator to end_node
    iterator save;                          // Used to return reference in search

    // -------------------------------- Search --------------------------------

    bool search_key(const tree_key_t& tree_key, treap_node_t** saved_ptr){
        treap_node_t* current = this->root;
        while(current != nullptr){
            if(saved_ptr != nullptr) *saved_ptr = current;
            if(current->tree_key == tree_key){
                this->cache_available = true;
                this->cached_node = current;
                return true;
            }
            else if(current->tree_key < tree_key){
                current = current->right_child;
            }
            else{
                current = current->left_child;
            }
        }

        return false;
    }


    // -------------------------------- Insert --------------------------------

    treap_node_t* create_node(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value){
        treap_node_t* new_node = new treap_node_t;
        new_node->tree_key = tree_key;
        new_node->heap_key = heap_key;
        new_node->value = value;
        new_node->parent = nullptr;
        new_node->left_child = nullptr;
        new_node->right_child = nullptr;
        return new_node;
    }

    void insert_key(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value){
        treap_node_t* current;
        if( (this->cache_available && this->cached_node->tree_key == tree_key) || this->search_key(tree_key, &current)){
            this->cached_node->value = value;
            if(this->cached_node->heap_key < heap_key){
                this->cached_node->heap_key = heap_key;
                this->rotate_treap_to_flow_up(this->cached_node->parent);
            }
            else if(this->cached_node->heap_key > heap_key){
                this->cached_node->heap_key = heap_key;
                this->rotate_treap_to_flow_down(this->cached_node);
            }
            return;
        }

        insert_key_at_parent(tree_key, heap_key, value, current);
    }

    void insert_key_at_parent(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value, treap_node_t* current){
        treap_node_t* new_node = create_node(tree_key, heap_key, value);
        ++this->node_count;
        if(this->root == nullptr){
            this->root = new_node;
            this->cache_available = true;
            this->cached_node = root;
            this->begin_itr = root;
            return;
        }

        new_node->parent = current;
        if(tree_key < current->tree_key){
            current->left_child = new_node;
        }
        else{
            current->right_child = new_node;
        }
        if(new_node->tree_key < this->begin_itr->tree_key){
            this->begin_itr = new_node;
        }

        this->rotate_treap_to_flow_up(current);
        this->cache_available = true;
        this->cached_node = new_node;
    }

    // -------------------------------- Rotation --------------------------------

    void rotate_treap_to_flow_up(treap_node_t* current){
        if(current == nullptr) return;
        treap_node_t* parent, *new_child;
        bool is_right_child;
        while(current != this->root){
            parent = current->parent;
            is_right_child = (parent->right_child == current);
            if(current->left_child != nullptr && current->heap_key < current->left_child->heap_key)
                new_child = this->right_rotate(current);
            else if(current->right_child != nullptr && current->heap_key < current->right_child->heap_key)
                new_child = this->left_rotate(current);
            else break;

            if(is_right_child)
                parent->right_child = new_child;
            else
                parent->left_child = new_child;
            current = parent;
        }

        if(current == this->root){
            if(current->left_child != nullptr && current->heap_key < current->left_child->heap_key)
                this->root = this->right_rotate(current);
            else if(current->right_child != nullptr && current->heap_key < current->right_child->heap_key)
                this->root = this->left_rotate(current);
        }
    }

    void rotate_treap_to_flow_down(treap_node_t* current){
        if(current == nullptr) return;
        treap_node_t* parent, *new_child;
        bool is_right_child;
        treap_node_t* selected_child;

        if(current == this->root){
            selected_child = current;

            if(current->left_child != nullptr && current->heap_key < current->left_child->heap_key)
                selected_child = current->left_child;
            else if(current->right_child != nullptr && selected_child->heap_key < current->right_child->heap_key)
                selected_child = current->left_child;
            else if(selected_child == current) return;

            if(selected_child == current->right_child)
                this->root = this->left_rotate(current);
            else
                this->root = this->right_rotate(current);
        }

        while(true){
            parent = current->parent;
            selected_child = current;
            is_right_child = (parent->right_child == current);

            if(current->left_child != nullptr && selected_child->heap_key < current->left_child->heap_key)
                selected_child = current->left_child;
            if(current->right_child != nullptr && selected_child->heap_key < current->right_child->heap_key)
                selected_child = current->right_child;
            if(selected_child == current) break;

            if(selected_child == current->right_child)
                new_child = this->left_rotate(current);
            else
                new_child = this->right_rotate(current);

            if(is_right_child)
                parent->right_child = new_child;
            else
                parent->left_child = new_child;
        }
    }

    treap_node_t* right_rotate(treap_node_t* node){
        treap_node_t* save = node->left_child;
        if(node->left_child->right_child != nullptr) {
            node->left_child->right_child->parent = node;
        }
        node->left_child = node->left_child->right_child;
        save->right_child = node;
        save->parent = node->parent;
        node->parent = save;
        return save;
    }

    treap_node_t* left_rotate(treap_node_t* node ){
        treap_node_t* save = node->right_child;
        if(node->right_child->left_child != nullptr){
            node->right_child->left_child->parent = node;
        }
        node->right_child = node->right_child->left_child;
        save->left_child = node;
        save->parent = node->parent;
        node->parent = save;
        return save;
    }

    // -------------------------------- DELETION --------------------------------

    treap_node_t* delete_key(){}

    void delete_tree(treap_node_t* root){
        if(root == nullptr) return;
        delete_tree(root->left_child);
        delete_tree(root->right_child);
        delete root;
    }

  public:
     
    // ---------------------- Constructors And Destructors ----------------------

    Base_Treap(){
        this->root = nullptr;
        this->node_count = 0;
        this->cache_available = false;
        this->cached_node = nullptr;
        this->end_itr = end_node;
        this->begin_itr = end_node;
        this->access_behaviour = Create_If_Not_Found;
    }

    ~Base_Treap(){
        delete_tree(root);
    }

    // -------------------------- Getters And Setters ---------------------------

    void set_access_behaviour(Access_Behaviour access_behaviour){
        this->access_behaviour = access_behaviour;
    }

    void set_default_value(value_t default_value){
        this->default_value = default_value;
    }

    size_t size() const{
        return this->node_count;
    }

    iterator& end(){
        return this->end_itr;
    }

    iterator& begin(){
        return this->begin_itr;
    }

    iterator& end() const{
        return this->end_itr;
    }

    iterator& begin() const{
        return this->begin_itr;
    }
};

template <typename tree_key_t, typename value_t, typename heap_key_t>
class Base_Treap<tree_key_t, value_t, heap_key_t>::iterator{
    treap_node_t* ptr;
    public:

    iterator(){
        ptr = nullptr;
    }

    iterator(treap_node_t* ptr){
        this->ptr = ptr;
    }

    iterator& operator++(){
        // Inorder Successor
        if(ptr->right_child != nullptr){
            ptr = ptr->right_child;
            while(ptr->left_child != nullptr){
                ptr = ptr->left_child;
            }
        }
        else{
            treap_node_t *parent = ptr->parent;
            while(parent != nullptr && ptr == parent->right_child){
                ptr = parent;
                parent = parent->parent;
            }
            ptr = parent;
            if(ptr == nullptr){
                ptr = end_node;
            }
        }
        return (*this);
    }

    iterator operator++(int x){
        this->operator++();
        return (*this);
    }

    treap_node_t& operator*(){
        return *ptr;
    }

    treap_node_t* operator->(){
        return ptr;
    }

    bool operator==(const iterator& itr2){
        return (this->ptr == itr2.ptr);
    }

    bool operator!=(const iterator& itr2){
        return (this->ptr != itr2.ptr);
    }
};


// Initialise Sentinal End Node
template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t, value_t, heap_key_t>* Base_Treap<tree_key_t, value_t, heap_key_t>::end_node = new treap_node<tree_key_t, value_t, heap_key_t>;

// Manual Version
template <typename tree_key_t, typename value_t, typename heap_key_t = void>
class Treap: public Base_Treap<tree_key_t, value_t, heap_key_t>{
    typedef Base_Treap<tree_key_t, value_t, heap_key_t> base_treap_t;
    typedef typename base_treap_t::iterator iterator;
    using typename base_treap_t::treap_node_t;
public:
    Treap():base_treap_t(){}

    void insert(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value){
        this->insert_key(tree_key, heap_key, value);
    }

    iterator& operator [](const tree_key_t& tree_key){
        return this->search(tree_key);
    }

    value_t& operator [](const pair<const tree_key_t, const heap_key_t>& key){
        treap_node_t* saved_parent;
        if( (this->cache_available && this->cached_node->tree_key == key.first) || this->search_key(key.first, &saved_parent)){
            if(this->cached_node->heap_key == key.second)
                return this->cached_node->value;
        }

        switch(this->access_behaviour){
            case Error_If_Not_Found:
                throw "Key Not Found";
                break;

            case Create_If_Not_Found:
                this->insert_key_at_parent(key.first, key.second, this->default_value, saved_parent);
                return this->cached_node->value;
                break;
        }
    }
};

// Randomised Version
template <typename tree_key_t, typename value_t>
class Treap<tree_key_t, value_t, void>: public Base_Treap<tree_key_t, value_t, double>{
    typedef Base_Treap<tree_key_t, value_t, double> base_treap_t;
    typedef typename base_treap_t::iterator iterator;
    using typename base_treap_t::treap_node_t;

    std::mt19937_64 RNGenerator;
    std::uniform_real_distribution<double> uniform;

public:
    Treap():base_treap_t(){
        // Setup Random Number Generator
        uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
        RNGenerator.seed(ss);
        uniform = std::uniform_real_distribution<double>(0, 1);
    }

    void insert(const tree_key_t& tree_key, const value_t& value){
        double heap_key = uniform(RNGenerator);
        this->insert_key(tree_key, heap_key, value);
    }

    value_t& operator [](const tree_key_t& tree_key){
        treap_node_t* saved_parent;
        if( (this->cache_available && this->cached_node->tree_key == tree_key) || this->search_key(tree_key, &saved_parent)){
            return this->cached_node->value;
        }

        switch(this->access_behaviour){
            case Error_If_Not_Found:
                throw "Key Not Found";
                break;

            case Create_If_Not_Found:
                double heap_key = uniform(RNGenerator);
                this->insert_key_at_parent(tree_key, heap_key, this->default_value, saved_parent);
                return this->cached_node->value;
                break;
        }
    }


};

int main(){
    Treap<int, int, int> treap1;  // Manual Mode
    Treap<int, int> treap2;       // Randomised Mode
    treap2.set_default_value(0);

    treap1.insert(5,9,1);
    treap1.insert(67,7,2);
    treap1.insert(23,5,3);
    treap1.insert(12,87,4);
    treap1.insert(4,45,5);
    treap1.insert(2,4,6);
    treap1.insert(3,2,7);
    treap1.insert(50,8,8);
    treap1.insert(6,2,9);
    treap1.insert(24,10,10);

    treap1.insert(50,88,10);
    treap1.insert(12,1,4);
    treap1.insert(2,80,10);

    treap1[make_pair(10,4)] = 8;

    treap2.insert(5,1);
    treap2.insert(67,2);
    treap2.insert(23,3);
    treap2.insert(12,4);
    treap2.insert(4,5);
    treap2.insert(2,6);
    treap2.insert(3,7);
    treap2.insert(50,8);
    treap2.insert(6,9);
    treap2.insert(24,10);



    auto save1 = treap2[10];
    auto save2 = treap2[2];

    auto temp = save1;
    save1 = save2;
    save2 = temp;
    cout << "SAVE 1 " << save1 << endl;
    cout << "SAVE 2 " << save2 << endl;

    for(auto x = treap2.begin(); x != treap2.end(); x++){
        cout << x->tree_key << " : " << x->heap_key << " : " << x->value << endl;
    }


    return 0;
}