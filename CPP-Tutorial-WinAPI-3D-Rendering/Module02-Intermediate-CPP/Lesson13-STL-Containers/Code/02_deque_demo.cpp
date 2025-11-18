// Deque demonstration with comprehensive operations
#include <iostream>
#include <deque>
using namespace std;
int main() {
    cout << "=== Deque Demo ===" << endl;
    deque<int> dq;
    dq.push_back(3); dq.push_back(4); dq.push_front(2); dq.push_front(1);
    cout << "Deque: "; for(int v : dq) cout << v << " "; cout << endl;
    cout << "Front: " << dq.front() << ", Back: " << dq.back() << endl;
    dq.pop_front(); dq.pop_back();
    cout << "After pops: "; for(int v : dq) cout << v << " "; cout << endl;
    return 0;
}
