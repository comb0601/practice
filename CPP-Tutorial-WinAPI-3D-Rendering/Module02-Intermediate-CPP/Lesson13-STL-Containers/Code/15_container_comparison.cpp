// Container performance comparison
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <chrono>
using namespace std;
using namespace chrono;
int main() {
    cout << "=== Container Comparison ===" << endl;
    const int N = 10000;
    
    // Vector push_back
    auto start = high_resolution_clock::now();
    vector<int> vec;
    for(int i = 0; i < N; i++) vec.push_back(i);
    auto end = high_resolution_clock::now();
    cout << "Vector push_back: " << duration_cast<microseconds>(end - start).count() << " us" << endl;
    
    // List push_back
    start = high_resolution_clock::now();
    list<int> lst;
    for(int i = 0; i < N; i++) lst.push_back(i);
    end = high_resolution_clock::now();
    cout << "List push_back: " << duration_cast<microseconds>(end - start).count() << " us" << endl;
    
    // Deque push_back
    start = high_resolution_clock::now();
    deque<int> dq;
    for(int i = 0; i < N; i++) dq.push_back(i);
    end = high_resolution_clock::now();
    cout << "Deque push_back: " << duration_cast<microseconds>(end - start).count() << " us" << endl;
    
    // Set insert
    start = high_resolution_clock::now();
    set<int> s;
    for(int i = 0; i < N; i++) s.insert(i);
    end = high_resolution_clock::now();
    cout << "Set insert: " << duration_cast<microseconds>(end - start).count() << " us" << endl;
    
    return 0;
}
