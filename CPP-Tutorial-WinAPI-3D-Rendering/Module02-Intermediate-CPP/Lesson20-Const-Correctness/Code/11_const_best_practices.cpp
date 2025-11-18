#include <iostream>
#include <vector>
using namespace std;
// 1. Use const for parameters that shouldn't change
void process(const vector<int>& data) {
    for(int val : data) {
        cout << val << " ";
    }
    cout << endl;
}
// 2. Mark member functions const if they don't modify state
class Calculator {
    int result;
public:
    Calculator() : result(0) {}
    int add(int a, int b) const {
        return a + b;  // Doesn't modify state
    }
    void setResult(int r) {
        result = r;  // Modifies state, not const
    }
};
// 3. Return const references when appropriate
class Container {
    vector<int> data;
public:
    const vector<int>& getData() const {
        return data;  // Don't allow modification
    }
};
int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    process(v);
    Calculator calc;
    cout << calc.add(5, 3) << endl;
    Container c;
    const vector<int>& data = c.getData();
    return 0;
}
