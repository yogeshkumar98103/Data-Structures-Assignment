#include "Treap.h"

// ===================================================================
//                             Base Treap
// ===================================================================

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t, value_t, heap_key_t>* Base_Treap<tree_key_t, value_t, heap_key_t>::end_node = new treap_node<tree_key_t, value_t, heap_key_t>;

template<typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &Base_Treap<tree_key_t, value_t, heap_key_t>::
search(const tree_key_t &tree_key){
    if( (this->cache_available && this->cached_node->tree_key == tree_key) || this->search_key(tree_key, nullptr)){
        return save = this->cached_node;
    }
    return save = this->end_node;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
bool Base_Treap<tree_key_t, value_t, heap_key_t>::
search_key(const tree_key_t &tree_key, Base_Treap::treap_node_t **saved_ptr){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t, value_t, heap_key_t>* Base_Treap<tree_key_t, value_t, heap_key_t>::
create_node(const tree_key_t &tree_key, const heap_key_t &heap_key, const value_t &value){
    treap_node_t* new_node = new treap_node_t;
    new_node->tree_key = tree_key;
    new_node->heap_key = heap_key;
    new_node->value = value;
    new_node->parent = nullptr;
    new_node->left_child = nullptr;
    new_node->right_child = nullptr;
    return new_node;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
insert_key(const tree_key_t &tree_key, const heap_key_t &heap_key, const value_t &value){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
insert_key_at_parent(const tree_key_t &tree_key, const heap_key_t &heap_key, const value_t &value, Base_Treap::treap_node_t *current){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
rotate_treap_to_flow_up(Base_Treap::treap_node_t *current){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
rotate_treap_to_flow_down(Base_Treap::treap_node_t *current){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t, value_t, heap_key_t> *Base_Treap<tree_key_t, value_t, heap_key_t>::
right_rotate(Base_Treap::treap_node_t *node){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t, value_t, heap_key_t> *Base_Treap<tree_key_t, value_t, heap_key_t>::
left_rotate(Base_Treap::treap_node_t *node){
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

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t, value_t, heap_key_t> *Base_Treap<tree_key_t, value_t, heap_key_t>::
delete_key(){}

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
delete_tree(Base_Treap::treap_node_t *root){
    if(root == nullptr) return;
    delete_tree(root->left_child);
    delete_tree(root->right_child);
    delete root;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
Base_Treap<tree_key_t, value_t, heap_key_t>::
Base_Treap(){
    this->root = nullptr;
    this->node_count = 0;
    this->cache_available = false;
    this->cached_node = nullptr;
    this->end_itr = end_node;
    this->begin_itr = end_node;
    this->access_behaviour = Create_If_Not_Found;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
Base_Treap<tree_key_t, value_t, heap_key_t>::
~Base_Treap(){
    delete_tree(root);
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
set_access_behaviour(Access_Behaviour access_behaviour){
    this->access_behaviour = access_behaviour;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
void Base_Treap<tree_key_t, value_t, heap_key_t>::
set_default_value(value_t default_value){
    this->default_value = default_value;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
size_t Base_Treap<tree_key_t, value_t, heap_key_t>::
size() const {
    return this->node_count;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &Base_Treap<tree_key_t, value_t, heap_key_t>::
end(){
    return this->end_itr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &Base_Treap<tree_key_t, value_t, heap_key_t>::
begin(){
    return this->begin_itr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &Base_Treap<tree_key_t, value_t, heap_key_t>::
end() const {
    return this->end_itr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator& Base_Treap<tree_key_t, value_t, heap_key_t>::
begin() const {
    return this->begin_itr;
}

// ===================================================================
//                                Iterator
// ===================================================================


template <typename tree_key_t, typename value_t, typename heap_key_t>
Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
iterator(){
    this->ptr = nullptr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
iterator(treap_node_t *ptr){
    this->ptr = ptr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &
Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
operator++(){
    // Inorder Successor
    if(this->ptr->right_child != nullptr){
        this->ptr = this->ptr->right_child;
        while(this->ptr->left_child != nullptr){
            this->ptr = this->ptr->left_child;
        }
    }
    else{
        treap_node_t *parent = this->ptr->parent;
        while(parent != nullptr && this->ptr == parent->right_child){
            this->ptr = parent;
            parent = parent->parent;
        }
        this->ptr = parent;
        if(this->ptr == nullptr){
            this->ptr = end_node;
        }
    }
    return (*this);
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator
Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
operator++(int x){
    this->operator++();
    return (*this);
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t,value_t, heap_key_t>&  Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
operator*(){
    return *ptr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
treap_node<tree_key_t,value_t, heap_key_t>*  Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
operator->(){
    return ptr;
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
bool Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
operator==(const typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &itr2){
    return (this->ptr == itr2.ptr);
}

template <typename tree_key_t, typename value_t, typename heap_key_t>
bool Base_Treap<tree_key_t, value_t, heap_key_t>::iterator::
operator!=(const typename Base_Treap<tree_key_t, value_t, heap_key_t>::iterator &itr2){
    return (this->ptr != itr2.ptr);
}

// ===================================================================
//                                Treap
// ===================================================================

// --------------------------  Manual Mode ----------------------------

template<typename tree_key_t, typename value_t, typename heap_key_t>
Treap<tree_key_t, value_t, heap_key_t>::
Treap():base_treap_t(){}

template<typename tree_key_t, typename value_t, typename heap_key_t>
void Treap<tree_key_t, value_t, heap_key_t>::
insert(const tree_key_t &tree_key, const heap_key_t &heap_key, const value_t &value){
    this->insert_key(tree_key, heap_key, value);
}

template<typename tree_key_t, typename value_t, typename heap_key_t>
typename Treap<tree_key_t, value_t, heap_key_t>::iterator &Treap<tree_key_t, value_t, heap_key_t>::
operator[](const tree_key_t &tree_key){
    return this->search(tree_key);
}

template<typename tree_key_t, typename value_t, typename heap_key_t>
value_t &Treap<tree_key_t, value_t, heap_key_t>::
operator[](const std::pair<const tree_key_t, const heap_key_t> &key){
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

// ------------------------- Randomised Mode --------------------------

template<typename tree_key_t, typename value_t>
Treap<tree_key_t, value_t, void>::
Treap():base_treap_t(){
    // Setup Random Number Generator
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    RNGenerator.seed(ss);
    uniform = std::uniform_real_distribution<double>(0, 1);
}

template<typename tree_key_t, typename value_t>
void Treap<tree_key_t, value_t, void>::
insert(const tree_key_t &tree_key, const value_t &value){
    double heap_key = uniform(RNGenerator);
    this->insert_key(tree_key, heap_key, value);
}

template<typename tree_key_t, typename value_t>
value_t &Treap<tree_key_t, value_t, void>::
operator[](const tree_key_t &tree_key){
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