#include <iostream>
#include <vector>
using namespace std;
class SafeVector {
    vector<int> data;
public:
    void push(int val) {
        data.push_back(val);  // Strong exception guarantee
    }
    int at(size_t idx) {
        return data.at(idx);  // Throws if out of range
    }
};
int main() {
    SafeVector sv;
    sv.push(1); sv.push(2); sv.push(3);
    try {
        cout << sv.at(10) << endl;
    } catch(const out_of_range& e) {
        cout << "Caught: " << e.what() << endl;
    }
    return 0;
}
