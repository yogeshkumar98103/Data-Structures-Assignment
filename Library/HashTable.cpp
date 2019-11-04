#include <iostream>
#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

using namespace std;

enum Collision_Handling_Technique{
    Seperate_Chaining,
    Open_Addressing
};

enum Access_Behaviour{
    Error_If_Not_Found,
    Create_If_Not_Found
};

namespace hashFn{

    // --------------------------- DJB2 --------------------------------
    template <typename key_t>
    unsigned long int djb2(const key_t& key);

    template <>
    unsigned long int djb2(const string& key){
        unsigned long int hash = 5381;
        for(char c: key){
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }

    unsigned long int djb2(const char& key){
        return key;
    }

    // --------------------------- CRC ---------------------------------
    array<unsigned long int, 256> generate_crc_lookup_table() noexcept{
        auto const reversed_polynomial = 0xEDB88320uL;

        struct byte_checksum{
            unsigned long int operator()() noexcept{
                auto checksum = static_cast<unsigned long int>(n++);
                for (auto i = 0; i < 8; ++i)
                    checksum = (checksum >> 1) ^ ((checksum & 0x1u) ? reversed_polynomial : 0);
                return checksum;
            }
            unsigned n = 0;
        };

        auto table = array<unsigned long int, 256>{};
        generate(table.begin(), table.end(), byte_checksum{});

        return table;
    }

    template <typename itr_t>
    unsigned long int crc(itr_t first, itr_t last){
        static auto const table = generate_crc_lookup_table();
        return 0xFFFFFFFFuL &
               ~accumulate(first, last,
                           ~0uL & 0xFFFFFFFFuL,
                           [](unsigned long int checksum, unsigned long int value){
                               return table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8);
                           });
    }
}

namespace probingFn{
    template <unsigned long int linear_factor>
    inline unsigned long int linear_probing(unsigned long int x){
        return linear_factor * x;
    }

    inline unsigned long int quadratic_probing(unsigned long int x){
        return (x * x + x) >> 1;
    }
}

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

template <typename key_t>
class Hash_Module{
  public:
    unsigned long int initial_size;
    double load_factor;

    Hash_Module(){
        this->load_factor = 0.5;
        this->initial_size = 101;
    }

    Hash_Module(double load_factor, unsigned long int initial_size){
        this->load_factor = load_factor;
        this->initial_size = initial_size;
    }

    virtual unsigned long int hash(key_t key){
        return hashFn::djb2(key);
    }

    virtual unsigned long int increase_size(unsigned long int old_size){
        return (old_size << 1);
    }

    virtual unsigned long int decrease_size(unsigned long int old_size){
        if((old_size >> 1) < this->initial_size) return this->initial_size;
        return (old_size >> 1);
    }

    virtual void setup_probing(key_t key){}

    virtual unsigned long int probe(unsigned long int x){
        return x;
    }

};

// NOTE:- Choose linear_constant as prime to avoid cycle
template <typename key_t, unsigned long int linear_constant>
class LinearProbingModule: public Hash_Module<key_t>{
  public:

    LinearProbingModule():Hash_Module<key_t>(){}
    LinearProbingModule(double load_factor, unsigned long int initial_size):Hash_Module<key_t>(load_factor, initial_size){}

    static unsigned long int gcd(unsigned long int x, unsigned long int y){
        if(y == 0) return x;
        return gcd(y, x%y);
    }

    unsigned long int probe(unsigned long int x) override{
        return probingFn::linear_probing<linear_constant>(x);
    }

    unsigned long int increase_size(unsigned long int old_size) override{
        unsigned long int new_size = (old_size << 1);
        while(gcd(new_size, linear_constant) != 1){
            new_size++;
        }
        return new_size;
    }

    unsigned long int decrease_size(unsigned long int old_size) override{
        unsigned long int new_size = (old_size >> 1);
        if(new_size < this->initial_size) return this->initial_size;
        while(gcd(new_size, linear_constant) != 1){
            new_size++;
        }
        return new_size;
    }
};

// NOTE:- Choose initial capacity to be power of two
template <typename key_t>
class QuadraticProbingModule: public Hash_Module<key_t>{
  public:

    QuadraticProbingModule():Hash_Module<key_t>(0.5, 128){}
    QuadraticProbingModule(double load_factor, unsigned long int initial_size):Hash_Module<key_t>(load_factor, initial_size){
        // Adjust initial_size to be power of 2
        unsigned long int size = initial_size;
        unsigned long int x = 1;
        while(x < 4){
            size |= size >> (x);
            x <<= 1;
        }
        ++size;
        this->initial_size = size;
    }

    unsigned long int probe(unsigned long int x) override{
        return probingFn::quadratic_probing(x);
    }
};

template <typename key_t, unsigned long int linear_constant>
class DoubleHashingModule: public Hash_Module<key_t>{
    unsigned long int hash;
  public:

    DoubleHashingModule():Hash_Module<key_t>(){}
    DoubleHashingModule(double load_factor, unsigned long int initial_size):Hash_Module<key_t>(load_factor, initial_size){}

    void setup_probing(key_t key) override{
        hash = hashFn::djb2(key);
        if(hash == 0) hash = 1;
    }

    unsigned long int probe(unsigned long int x) override{
        return (x * hash);
    }
};

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

    Base_HashTable(Hash_Module<key_t>& hash_module_):hash_module(hash_module_){
        if(initial_size > 0){
            this->capacity = initial_size;
        }
        else{
            this->capacity = hash_module.initial_size;
        }

        this->size = 0;
        this->max_size = (unsigned long int)(this->capacity * this->hash_module.load_factor);
        this->hash_table = new hash_node_t*[this->capacity];
        this->cached_node = nullptr;
        this->cache_available = false;
        this->access_behaviour = Create_If_Not_Found;

        for(int i = 0; i < this->capacity; i++){
            this->hash_table[i] = nullptr;
        }
    }

    ~Base_HashTable(){
        delete[] this->hash_table;
    }

    void insert(const key_t& key, const value_t& value){
        if(this->size >= this->max_size){
            expand();
        }

        hash_node_t* new_node = new hash_node_t;
        new_node->key = key;
        new_node->value = value;
        this->insert_to_hash_table(new_node, this->hash_table);
        this->size++;
        this->cache_available = true;
        this->cached_node = new_node;
    }

    void expand(){
        unsigned long old_capacity = this->capacity;
        this->capacity = this->hash_module.increase_size(this->capacity);
        if(old_capacity == this->capacity){
            throw "Cannot Expand Array as increased size is same as old size";
        }
        hash_node_t** new_hash_table = new hash_node_t*[this->capacity];

        for(int i = 0; i < this->capacity; i++){
            new_hash_table[i] = nullptr;
        }

        this->copy_values_to_new_hash_table(new_hash_table, old_capacity);

        delete[] this->hash_table;
        this->cached_node = nullptr;
        this->cache_available = false;
        this->hash_table = new_hash_table;
        this->max_size = (unsigned long int)(hash_module.load_factor * this->capacity);
    }

    void set_access_behaviour(Access_Behaviour access_behaviour){
        this->access_behaviour = access_behaviour;
    }

    void set_default_value(value_t default_value){
        this->default_value = default_value;
    }

    value_t& operator[](const key_t& key){
        unsigned long int index = -1;
        if( (cache_available && cached_node->key == key) || search_key(key, index)){
            return this->cached_node->value;
        }

        switch(this->access_behaviour){
            case Error_If_Not_Found:
                throw "Key Not Found";
                break;

            case Create_If_Not_Found:
                insert(key, default_value, index);
                return this->cached_node->value;
                break;
        }
    }

  protected:
    void insert(const key_t& key, const value_t& value, unsigned long int index){
        hash_node_t* new_node = new hash_node_t;
        new_node->key = key;
        new_node->value = value;

        if(this->size >= this->max_size){
            expand();
            this->insert_to_hash_table(new_node, this->hash_table);
        }
        else{
            this->insert_at_index(index, new_node);
        }
        
        this->size++;
        this->cache_available = true;
        this->cached_node = new_node;
    }

  public:
    // Override These Functions in Derived Class
    virtual void copy_values_to_new_hash_table(hash_node_t** new_hash_table, unsigned long int old_capacity){}
    virtual void insert_to_hash_table(hash_node_t* new_node, hash_node_t** hash_table){}
    virtual void insert_at_index(unsigned long int index, hash_node_t* new_node){}
    virtual bool search_key(const key_t& key, unsigned long int& save_index){return false;}
};

template <typename key_t, typename value_t, unsigned long int initial_size = 0, Collision_Handling_Technique handler_t = Seperate_Chaining>
class HashTable;

template <typename key_t, typename value_t, unsigned long int initial_size>
class HashTable<key_t, value_t, initial_size, Seperate_Chaining>
        : public Base_HashTable<key_t, value_t, hash_node_seperate_chaining<key_t, value_t>, initial_size>{

    typedef hash_node_seperate_chaining<key_t, value_t> hash_node_t;
    typedef Base_HashTable<key_t, value_t, hash_node_t, initial_size> base_t;

  public:

    HashTable(Hash_Module<key_t>& hash_module_):base_t(hash_module_){}
    HashTable(Hash_Module<key_t>&& hash_module_ = Hash_Module<key_t>()):base_t(hash_module_){}

    ~HashTable(){
        int count = 0;
        hash_node_t* current, *next;
        for(int i = 0; i < this->capacity; i++){
            if(count == this->size) break;
            if(this->hash_table[i] != nullptr){
                current = this->hash_table[i];
                do{
                    next = current->next;
                    delete current;
                    current = next;
                    count++;
                }while(current != nullptr);
            }
        }
    }

    void copy_values_to_new_hash_table(hash_node_t** new_hash_table, unsigned long int old_capacity) override{
        int count = 0;
        hash_node_t* current, *next;
        for(int i = 0; i < old_capacity; i++){
            if(count == this->size) break;
            if(this->hash_table[i] != nullptr){
                current = this->hash_table[i];
                do{
                    next = current->next;
                    current->next = nullptr;
                    insert_to_hash_table(current, new_hash_table);
                    count++;
                    current = next;
                }while(current != nullptr);
            }
        }
    }

    void insert_to_hash_table(hash_node_t* new_node, hash_node_t** hash_table) override {
        unsigned long int hash = this->hash_module.hash(new_node->key);
        hash %= this->capacity;

        if(hash_table[hash] != nullptr){
            new_node->next = hash_table[hash];
        }
        else{
            new_node->next = nullptr;
        }
        hash_table[hash] = new_node;
    }

    void insert_at_index(unsigned long int index, hash_node_t* new_node) override{
        if(this->hash_table[index] != nullptr){
            new_node->next = this->hash_table[index];
        }
        else{
            new_node->next = nullptr;
        }
        this->hash_table[index] = new_node;
    }

    bool search_key(const key_t& key, unsigned long int& saved_index) override{
        unsigned long int hash = this->hash_module.hash(key);
        hash %= this->capacity;

        hash_node_t* current = this->hash_table[hash];
        while(current != nullptr){
            if(current->key == key){
                this->cache_available = true;
                this->cached_node = current;
                return true;
            }
            current = current->next;
        }

        saved_index = hash;
        return false;
    }
};

template <typename key_t, typename value_t, unsigned long int initial_size>
class HashTable<key_t, value_t, initial_size, Open_Addressing>
        : public Base_HashTable<key_t, value_t, hash_node_open_addressing<key_t, value_t>, initial_size>{
    typedef hash_node_open_addressing<key_t, value_t> hash_node_t;
    typedef Base_HashTable<key_t, value_t, hash_node_t, initial_size> base_t;

    hash_node_t* TOMBSTONE;

  public:

    HashTable(Hash_Module<key_t>& hash_module_):base_t(hash_module_){
        this->TOMBSTONE = new hash_node_t;
    }
    HashTable(Hash_Module<key_t>&& hash_module_ = Hash_Module<key_t>()):base_t(hash_module_){
        this->TOMBSTONE = new hash_node_t;
    }

    ~HashTable(){
        int count = 0;
        hash_node_t* current, *next;
        for(int i = 0; i < this->capacity; i++){
            if(count == this->size) break;
            if(this->hash_table[i] != nullptr){
                delete this->hash_table[i];
            }
        }
    }

    void copy_values_to_new_hash_table(hash_node_t** new_hash_table, unsigned long int old_capacity) override{
        int count = 0;
        hash_node_t* current;
        for(int i = 0; i < old_capacity; i++){
            if(count == this->size) break;
            current = this->hash_table[i];
            if(current != nullptr && current != this->TOMBSTONE){
                this->insert_to_hash_table(current, new_hash_table);
                count++;
            }
        }
    }

    void insert_to_hash_table(hash_node_t* new_node, hash_node_t** hash_table) override{
        unsigned long int hash = this->hash_module.hash(new_node->key);
        hash %= this->capacity;
        this->hash_module.setup_probing(new_node->key);

        unsigned long index = hash;
        unsigned long x = 1;
        hash_node_t* current = hash_table[index];
        while(!( current == nullptr || current == this->TOMBSTONE)){
            index = (hash + this->hash_module.probe(x++)) % this->capacity;
            current = hash_table[index];
        }

        hash_table[index] = new_node;
    }

    void insert_at_index(unsigned long int index, hash_node_t* new_node) override{
        this->hash_table[index] = new_node;
    }

    bool search_key(const key_t& key, unsigned long int& saved_index) override{
        unsigned long int hash = this->hash_module.hash(key);
        hash %= this->capacity;

        unsigned long index = hash;
        unsigned long x = 1;
        hash_node_t* current = this->hash_table[index];

        while(current != nullptr){
            if(current != this->TOMBSTONE && current->key == key){
                this->cache_available = true;
                this->cached_node = current;
                return true;
            }
            index = (hash + this->hash_module.probe(x++))%this->capacity;
            current = this->hash_table[index];
        }

        saved_index = index;
        return false;
    }
};

int main(){
    // auto module = LinearProbingModule<char, 17>();
    // HashTable<char, int, 4, Open_Addressing> count(module);

    HashTable<char, int> count;

    count.set_default_value(0);

    string s = "aghfbkanrtfakfagtfaugffavzfklrgmptgrhuqiekrqnvmfgjkhrfqr";
    for(char c: s){
        count[c]++;
    }
    cout << endl;

    for(int i = 97; i <= 122; i++){
        cout << (char)i << " : " << count[i] << endl;
    }
    cout << s.size();

    return 0;
}

/*

Expected Output

a : 6
b : 1
c : 0
d : 0
e : 1
f : 9
g : 6
h : 3
i : 1
j : 1
k : 5
l : 1
m : 2
n : 2
o : 0
p : 1
q : 3
r : 6
s : 0
t : 3
u : 2
v : 2
w : 0
x : 0
y : 0
z : 1

 */