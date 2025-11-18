// Stack LIFO operations
#include <iostream>
#include <stack>
using namespace std;
int main() {
    cout << "=== Stack Demo ===" << endl;
    stack<int> stk;
    stk.push(10); stk.push(20); stk.push(30);
    cout << "Pushing: 10, 20, 30" << endl;
    while(!stk.empty()) {
        cout << "Pop: " << stk.top() << endl;
        stk.pop();
    }
    return 0;
}
