// Queue FIFO operations
#include <iostream>
#include <queue>
using namespace std;
int main() {
    cout << "=== Queue Demo ===" << endl;
    queue<int> q;
    q.push(10); q.push(20); q.push(30);
    cout << "Enqueueing: 10, 20, 30" << endl;
    while(!q.empty()) {
        cout << "Dequeue: " << q.front() << endl;
        q.pop();
    }
    return 0;
}
