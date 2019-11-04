#include "HashTable.h"

// ===================================================================
//                       Hash Function Pool
// ===================================================================

// ------------------------ DJB2 -----------------------------
template <>
unsigned long int hashFn::djb2<std::string>(const std::string &key){
    unsigned long int hash = 5381;
    for(char c: key){
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

template <>
unsigned long int hashFn::djb2<char>(const char &key){
    return key;
}

// ------------------------ CRC ------------------------------
std::array<unsigned long int, 256> hashFn::generate_crc_lookup_table() noexcept{
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

    auto table = std::array<unsigned long int, 256>{};
    std::generate(table.begin(), table.end(), byte_checksum{});

    return table;
}

template <typename itr_t>
unsigned long int hashFn::crc(itr_t first, itr_t last){
    static auto const table = hashFn::generate_crc_lookup_table();
    return 0xFFFFFFFFuL &
        ~std::accumulate(first, last,~0uL & 0xFFFFFFFFuL, [](unsigned long int checksum, unsigned long int value){
            return table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8);
    });
}


// ===================================================================
//                       Probing Function Pool
// ===================================================================

template <unsigned long linear_factor>
unsigned long int probingFn::linear_probing(unsigned long int x){
    return linear_factor * x;
}

unsigned long int probingFn::quadratic_probing(unsigned long int x){
    return (x * x + x) >> 1;
}


// ===================================================================
//                          Hashing Module
// ===================================================================

template <typename key_t>
Hash_Module<key_t>::Hash_Module(){
    this->load_factor = 0.5;
    this->initial_size = 101;
}

template <typename key_t>
Hash_Module<key_t>::Hash_Module(double load_factor, unsigned long int initial_size){
    this->load_factor = load_factor;
    this->initial_size = initial_size;
}

template <typename key_t>
unsigned long int Hash_Module<key_t>::hash(key_t key){
    return hashFn::djb2(key);
}

template <typename key_t>
unsigned long int Hash_Module<key_t>::increase_size(unsigned long int old_size){
    return (old_size << 1);
}

template <typename key_t>
unsigned long int Hash_Module<key_t>::decrease_size(unsigned long int old_size){
    if((old_size >> 1) < this->initial_size) return this->initial_size;
    return (old_size >> 1);
}

template <typename key_t>
void Hash_Module<key_t>::setup_probing(key_t key){}

template <typename key_t>
unsigned long int Hash_Module<key_t>::probe(unsigned long int x){
    return x;
}

// ---------------------------- Linear Probing ----------------------------

template <typename key_t, unsigned long linear_constant>
LinearProbingModule<key_t, linear_constant>::LinearProbingModule():Hash_Module<key_t>(){}

template <typename key_t, unsigned long linear_constant>
LinearProbingModule<key_t, linear_constant>::LinearProbingModule(double load_factor, unsigned long int initial_size):Hash_Module<key_t>(load_factor, initial_size){}

template <typename key_t, unsigned long linear_constant>
unsigned long int LinearProbingModule<key_t, linear_constant>::gcd(unsigned long int x, unsigned long int y){
    if(y == 0) return x;
    return gcd(y, x%y);
}

template <typename key_t, unsigned long linear_constant>
unsigned long int LinearProbingModule<key_t, linear_constant>::probe(unsigned long int x){
    return probingFn::linear_probing<linear_constant>(x);
}

template <typename key_t, unsigned long linear_constant>
unsigned long int LinearProbingModule<key_t, linear_constant>::increase_size(unsigned long int old_size){
    unsigned long int new_size = (old_size << 1);
    while(gcd(new_size, linear_constant) != 1){
        new_size++;
    }
    return new_size;
}

template <typename key_t, unsigned long linear_constant>
unsigned long int LinearProbingModule<key_t, linear_constant>::decrease_size(unsigned long int old_size){
    unsigned long int new_size = (old_size >> 1);
    if(new_size < this->initial_size) return this->initial_size;
    while(gcd(new_size, linear_constant) != 1){
        new_size++;
    }
    return new_size;
}

// ---------------------------- Quadratic Probing ----------------------------

template <typename key_t>
QuadraticProbingModule<key_t>::QuadraticProbingModule():Hash_Module<key_t>(0.5, 128){}

template <typename key_t>
QuadraticProbingModule<key_t>::QuadraticProbingModule(double load_factor, unsigned long int initial_size):Hash_Module<key_t>(load_factor, initial_size){
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

template <typename key_t>
unsigned long int QuadraticProbingModule<key_t>::probe(unsigned long int x){
    return probingFn::quadratic_probing(x);
}

// ---------------------------- Double Hashing ----------------------------

template <typename key_t>
DoubleHashingModule<key_t>::DoubleHashingModule():Hash_Module<key_t>(){}

template <typename key_t>
DoubleHashingModule<key_t>::DoubleHashingModule(double load_factor, unsigned long int initial_size):Hash_Module<key_t>(load_factor, initial_size){}

template <typename key_t>
void DoubleHashingModule<key_t>::setup_probing(key_t key){
    hash = hashFn::djb2(key);
    if(hash == 0) hash = 1;
}

template <typename key_t>
unsigned long int DoubleHashingModule<key_t>::probe(unsigned long int x){
    return (x * hash);
}

// ===================================================================
//                          Base Hash Table
// ===================================================================

template <typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
Base_HashTable(Hash_Module<key_t> &hash_module_):hash_module(hash_module_){
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

template <typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
~Base_HashTable(){
    delete[] this->hash_table;
}

template <typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
void Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
insert(const key_t &key, const value_t &value){
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

template <typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
void Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
expand(){
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

template<typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
void Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
set_access_behaviour(Access_Behaviour access_behaviour){
    this->access_behaviour = access_behaviour;
}

template<typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
void Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
set_default_value(value_t default_value){
    this->default_value = default_value;
}

template<typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
value_t &Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
operator[](const key_t &key){
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

template<typename key_t, typename value_t, typename hash_node_t, unsigned long initial_size>
void Base_HashTable<key_t, value_t, hash_node_t, initial_size>::
insert(const key_t &key, const value_t &value,unsigned long int index){
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

// ===================================================================
//                             Hash Table
// ===================================================================

// ------------------------ Seperate Chaining ------------------------

template<typename key_t, typename value_t, unsigned long initial_size>
HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
HashTable(Hash_Module<key_t>& hash_module_):base_t(hash_module_){}

template<typename key_t, typename value_t, unsigned long initial_size>
HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
HashTable(Hash_Module<key_t>&& hash_module_):base_t(hash_module_){}


template<typename key_t, typename value_t, unsigned long initial_size>
HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
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

template<typename key_t, typename value_t, unsigned long initial_size>
void HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
copy_values_to_new_hash_table(hash_node_seperate_chaining<key_t, value_t> **new_hash_table, unsigned long int old_capacity){
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

template<typename key_t, typename value_t, unsigned long initial_size>
void HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
insert_to_hash_table(hash_node_seperate_chaining<key_t, value_t> *new_node, hash_node_seperate_chaining<key_t, value_t> **hash_table){
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

template<typename key_t, typename value_t, unsigned long initial_size>
void HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
insert_at_index(unsigned long int index, hash_node_seperate_chaining<key_t, value_t> *new_node){
    if(this->hash_table[index] != nullptr){
        new_node->next = this->hash_table[index];
    }
    else{
        new_node->next = nullptr;
    }
    this->hash_table[index] = new_node;
}

template<typename key_t, typename value_t, unsigned long initial_size>
bool HashTable<key_t, value_t, initial_size, Seperate_Chaining>::
search_key(const key_t &key, unsigned long int &saved_index){
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


// ------------------------ Open Addressing ------------------------

template<typename key_t, typename value_t, unsigned long initial_size>
HashTable<key_t, value_t, initial_size, Open_Addressing>::
HashTable(Hash_Module<key_t> &hash_module_):base_t(hash_module_){
    this->TOMBSTONE = new hash_node_t;
}

template<typename key_t, typename value_t, unsigned long initial_size>
HashTable<key_t, value_t, initial_size, Open_Addressing>::
HashTable(Hash_Module<key_t> &&hash_module_):base_t(hash_module_){
    this->TOMBSTONE = new hash_node_t;
}

template<typename key_t, typename value_t, unsigned long initial_size>
HashTable<key_t, value_t, initial_size, Open_Addressing>::
~HashTable() {
    int count = 0;
    hash_node_t* current, *next;
    for(int i = 0; i < this->capacity; i++){
        if(count == this->size) break;
        if(this->hash_table[i] != nullptr){
            delete this->hash_table[i];
        }
    }
}

template<typename key_t, typename value_t, unsigned long initial_size>
void HashTable<key_t, value_t, initial_size, Open_Addressing>::
copy_values_to_new_hash_table(hash_node_open_addressing<key_t, value_t> **new_hash_table, unsigned long int old_capacity) {
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

template<typename key_t, typename value_t, unsigned long initial_size>
void HashTable<key_t, value_t, initial_size, Open_Addressing>::
insert_to_hash_table(hash_node_open_addressing<key_t, value_t> *new_node, hash_node_open_addressing<key_t, value_t> **hash_table) {
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

template<typename key_t, typename value_t, unsigned long initial_size>
void HashTable<key_t, value_t, initial_size, Open_Addressing>::
insert_at_index(unsigned long int index, hash_node_open_addressing<key_t, value_t> *new_node) {
    this->hash_table[index] = new_node;
}

template<typename key_t, typename value_t, unsigned long initial_size>
bool HashTable<key_t, value_t, initial_size, Open_Addressing>::
search_key(const key_t &key, unsigned long int &saved_index) {
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