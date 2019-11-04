#include "../Library/queue.h"
#include <iostream>
using namespace std;

int main(){
    queue<int> q;
    q.enQueue(1);
    q.enQueue(2);
    q.enQueue(3);
    q.enQueue(4);
    q.enQueue(5);

    q.print();

    cout << q.deQueue();
    cout << q.deQueue();
    cout << q.deQueue();
    cout << q.deQueue();
    cout << q.deQueue();

    cout << q.isEmpty();
    cout << q.deQueue();
    return 0;
}