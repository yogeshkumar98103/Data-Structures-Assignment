#include "../Library/stack.h"
#include <iostream>
using namespace std;

int main(){
    stack<int> s;
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);
    s.print();
    cout << s.pop();
    cout << s.pop();
    cout << s.pop();
    cout << s.pop();
    cout << s.isEmpty();
    return 0;
}