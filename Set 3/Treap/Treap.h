#ifndef DS__Treap
#define DS__Treap

#include <random>
#include <chrono>

// ===================================================================
//                          Characterisation
// ===================================================================

enum Access_Behaviour{
    Error_If_Not_Found,
    Create_If_Not_Found
};

// ===================================================================
//                          Data Node
// ===================================================================

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

// ===================================================================
//                             Base Treap
// ===================================================================

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

    iterator& search(const tree_key_t& tree_key);

  protected:
    iterator begin_itr;                     // Stores the iterator to node with smallest tree key
    iterator end_itr;                       // Stores the iterator to end_node
    iterator save;                          // Used to return reference in search

    // -------------------------------- Search --------------------------------

    bool search_key(const tree_key_t& tree_key, treap_node_t** saved_ptr);


    // -------------------------------- Insert --------------------------------

    treap_node_t* create_node(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value);

    void insert_key(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value);

    void insert_key_at_parent(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value, treap_node_t* current);

    // -------------------------------- Rotation --------------------------------

    void rotate_treap_to_flow_up(treap_node_t* current);

    void rotate_treap_to_flow_down(treap_node_t* current);

    treap_node_t* right_rotate(treap_node_t* node);

    treap_node_t* left_rotate(treap_node_t* node );

    // -------------------------------- DELETION --------------------------------

    treap_node_t* delete_key();

    void delete_tree(treap_node_t* root);

  public:

    // ---------------------- Constructors And Destructors ----------------------

    Base_Treap();

    ~Base_Treap();

    // -------------------------- Getters And Setters ---------------------------

    void set_access_behaviour(Access_Behaviour access_behaviour);

    void set_default_value(value_t default_value);

    size_t size() const;

    iterator& end();

    iterator& begin();

    iterator& end() const;

    iterator& begin() const;
};

// -------------------------- Iterator ----------------------------

template <typename tree_key_t, typename value_t, typename heap_key_t>
class Base_Treap<tree_key_t, value_t, heap_key_t>::iterator{
    treap_node_t* ptr;

  public:
    iterator();
    iterator(treap_node_t* ptr);

    iterator& operator++();
    iterator operator++(int x);

    treap_node_t& operator*();
    treap_node_t* operator->();

    bool operator==(const iterator& itr2);
    bool operator!=(const iterator& itr2);
};

// ===================================================================
//                                Treap
// ===================================================================

// --------------------------- Manual Mode ----------------------------

template <typename tree_key_t, typename value_t, typename heap_key_t = void>
class Treap: public Base_Treap<tree_key_t, value_t, heap_key_t>{
    typedef Base_Treap<tree_key_t, value_t, heap_key_t> base_treap_t;
    typedef typename base_treap_t::iterator iterator;
    using typename base_treap_t::treap_node_t;
  
  public:
    Treap();

    void insert(const tree_key_t& tree_key, const heap_key_t& heap_key, const value_t& value);

    iterator& operator [](const tree_key_t& tree_key);
    value_t& operator [](const std::pair<const tree_key_t, const heap_key_t>& key);
};

// ------------------------- Randomised Mode --------------------------

template <typename tree_key_t, typename value_t>
class Treap<tree_key_t, value_t, void>: public Base_Treap<tree_key_t, value_t, double>{
    typedef Base_Treap<tree_key_t, value_t, double> base_treap_t;
    typedef typename base_treap_t::iterator iterator;
    using typename base_treap_t::treap_node_t;

    std::mt19937_64 RNGenerator;
    std::uniform_real_distribution<double> uniform;

  public:
    Treap();

    void insert(const tree_key_t& tree_key, const value_t& value);

    value_t& operator [](const tree_key_t& tree_key);
};

#include "Treap.cpp"
#endif