#include <iostream>
#include "HashTable.h"

int main(){
    // auto module = LinearProbingModule<char, 17>();
    // HashTable<char, int, 101, Open_Addressing> count(module);

    HashTable<char, int> count;
    count.set_default_value(0);

    std::string s = "aghfbkanrtfakfagtfaugffavzfklrgmptgrhuqiekrqnvmfgjkhrfqr";
    for(char c: s){
        count[c]++;
    }
    std::cout << std::endl;

    for(int i = 97; i <= 122; i++){
        std::cout << (char)i << " : " << count[i] << std::endl;
    }

    count.delete('a');

    for(int i = 97; i <= 122; i++){
        std::cout << (char)i << " : " << count[i] << std::endl;
    }
    return 0;
}