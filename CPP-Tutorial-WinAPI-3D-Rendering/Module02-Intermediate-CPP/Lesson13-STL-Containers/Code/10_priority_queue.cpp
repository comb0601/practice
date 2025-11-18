// Priority queue with heap
#include <iostream>
#include <queue>
using namespace std;
int main() {
    cout << "=== Priority Queue ===" << endl;
    priority_queue<int> pq;
    pq.push(30); pq.push(10); pq.push(50); pq.push(20);
    cout << "Elements: 30, 10, 50, 20" << endl;
    cout << "Popping (max-heap):" << endl;
    while(!pq.empty()) {
        cout << "  " << pq.top() << endl;
        pq.pop();
    }
    return 0;
}
