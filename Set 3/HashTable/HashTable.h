#ifndef DS__Hash_Table
#define DS__Hash_Table

#include <iostream>
#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

// ===================================================================
//                       Characterisation
// ===================================================================

enum Collision_Handling_Technique{
    Seperate_Chaining,
    Open_Addressing
};

enum Access_Behaviour{
    Error_If_Not_Found,
    Create_If_Not_Found
};

// ===================================================================
//                           Data Nodes
// ===================================================================

template <typename key_t, typename value_t, typename hash_node_t, unsigned long int initial_size>
class Base_HashTable;

template <typename key_t, typename value_t>
struct hash_node_seperate_chaining{
    key_t key;
    value_t value;  
    hash_node_seperate_chaining<key_t, value_t>* next;
};

template <typename key_t, typename value_t>
struct hash_node_open_addressing{
    key_t key;
    value_t value;
};

// ===================================================================
//                       Hash Function Pool
// ===================================================================

namespace hashFn{
    // ------------------------ DJB2 -----------------------------
    template <typename key_t>
    unsigned long int djb2(const key_t& key);

    template <>
    unsigned long int djb2<std::string>(const std::string& key);

    template <>
    unsigned long int djb2<char>(const char& key);

    // ------------------------ CRC ------------------------------
    std::array<unsigned long int, 256> generate_crc_lookup_table() noexcept;

    template <typename itr_t>
    unsigned long int crc(itr_t first, itr_t last);
}

// ===================================================================
//                       Probing Function Pool
// ===================================================================

namespace probingFn{
    template <unsigned long int linear_factor>
    inline unsigned long int linear_probing(unsigned long int x);

    inline unsigned long int quadratic_probing(unsigned long int x);
}


// ===================================================================
//                          Hashing Module
// ===================================================================

template <typename key_t>
class Hash_Module{
  public:
    unsigned long int initial_size;
    double load_factor;

    Hash_Module();

    Hash_Module(double load_factor, unsigned long int initial_size);

    virtual unsigned long int hash(key_t key);

    virtual unsigned long int increase_size(unsigned long int old_size);

    virtual unsigned long int decrease_size(unsigned long int old_size);

    virtual void setup_probing(key_t key);

    virtual unsigned long int probe(unsigned long int x);

};

// ---------------------------- Linear Probing ----------------------------

// NOTE:- Choose linear_constant as prime to avoid cycle
template <typename key_t, unsigned long int linear_constant>
class LinearProbingModule: public Hash_Module<key_t>{
  public:

    LinearProbingModule();
    LinearProbingModule(double load_factor, unsigned long int initial_size);

    static unsigned long int gcd(unsigned long int x, unsigned long int y);

    unsigned long int probe(unsigned long int x) override;

    unsigned long int increase_size(unsigned long int old_size) override;

    unsigned long int decrease_size(unsigned long int old_size) override;
};

// ---------------------------- Quadratic Probing ----------------------------

template <typename key_t>
class QuadraticProbingModule: public Hash_Module<key_t>{
  public:

    QuadraticProbingModule();
    QuadraticProbingModule(double load_factor, unsigned long int initial_size);

    unsigned long int probe(unsigned long int x) override;
};

// ---------------------------- Double Hashing ----------------------------

template <typename key_t>
class DoubleHashingModule: public Hash_Module<key_t>{
    unsigned long int hash;
  public:

    DoubleHashingModule();
    DoubleHashingModule(double load_factor, unsigned long int initial_size);

    void setup_probing(key_t key) override;

    unsigned long int probe(unsigned long int x) override;
};

// ===================================================================
//                          Base Hash Table
// ===================================================================

template <typename key_t, typename value_t, typename hash_node_t, unsigned long int initial_size>
class Base_HashTable{
  protected:
    unsigned long int capacity;             // Size Of Array
    unsigned long int size;                 // Number of nodes stored
    unsigned long int max_size;             // Maximum nodes current array can store
    Hash_Module<key_t>& hash_module;        // Contains information characterisng hash table

    hash_node_t* cached_node;
    bool cache_available;
    hash_node_t** hash_table;

    Access_Behaviour access_behaviour;
    value_t default_value;

  public:

    Base_HashTable(Hash_Module<key_t>& hash_module_);

    ~Base_HashTable();

    void insert(const key_t& key, const value_t& value);

    void expand();

    void set_access_behaviour(Access_Behaviour access_behaviour);

    void set_default_value(value_t default_value);

    value_t& operator[](const key_t& key);

  protected:

    void insert(const key_t& key, const value_t& value, unsigned long int index);

  public:
    // Override These Functions in Derived Class
    virtual void copy_values_to_new_hash_table(hash_node_t** new_hash_table, unsigned long int old_capacity){}
    virtual void insert_to_hash_table(hash_node_t* new_node, hash_node_t** hash_table){}
    virtual void insert_at_index(unsigned long int index, hash_node_t* new_node){}
    virtual bool search_key(const key_t& key, unsigned long int& save_index){return false;}
};

// ===================================================================
//                             Hash Table
// ===================================================================

template <typename key_t, typename value_t, unsigned long int initial_size = 0, Collision_Handling_Technique handler_t = Seperate_Chaining>
class HashTable;

// ------------------------ Seperate Chaining ------------------------

template <typename key_t, typename value_t, unsigned long int initial_size>
class HashTable<key_t, value_t, initial_size, Seperate_Chaining>
: public Base_HashTable<key_t, value_t, hash_node_seperate_chaining<key_t, value_t>, initial_size>{

    typedef hash_node_seperate_chaining<key_t, value_t> hash_node_t;
    typedef Base_HashTable<key_t, value_t, hash_node_t, initial_size> base_t;

  public:

    HashTable(Hash_Module<key_t>& hash_module_);
    HashTable(Hash_Module<key_t>&& hash_module_ = Hash_Module<key_t>());

    ~HashTable();

  private:
    void copy_values_to_new_hash_table(hash_node_t** new_hash_table, unsigned long int old_capacity) override;

    void insert_to_hash_table(hash_node_t* new_node, hash_node_t** hash_table) override;

    void insert_at_index(unsigned long int index, hash_node_t* new_node) override;

    bool search_key(const key_t& key, unsigned long int& saved_index) override;

  public:
    hash_node_t remove(const key_t& key);
};

// ------------------------ Open Addressing ------------------------

template <typename key_t, typename value_t, unsigned long int initial_size>
class HashTable<key_t, value_t, initial_size, Open_Addressing>
: public Base_HashTable<key_t, value_t, hash_node_open_addressing<key_t, value_t>, initial_size>{
    typedef hash_node_open_addressing<key_t, value_t> hash_node_t;
    typedef Base_HashTable<key_t, value_t, hash_node_t, initial_size> base_t;

    hash_node_t* TOMBSTONE;

  public:

    HashTable(Hash_Module<key_t>& hash_module_);
    HashTable(Hash_Module<key_t>&& hash_module_ = Hash_Module<key_t>());

    ~HashTable();

  private:
    void copy_values_to_new_hash_table(hash_node_t** new_hash_table, unsigned long int old_capacity) override;

    void insert_to_hash_table(hash_node_t* new_node, hash_node_t** hash_table) override;

    void insert_at_index(unsigned long int index, hash_node_t* new_node) override;

    bool search_key(const key_t& key, unsigned long int& saved_index) override;

  public:
    hash_node_t remove(const key_t& key);
};

#include "HashTable.cpp"
#endif