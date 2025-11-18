// Map as dictionary/phonebook
#include <iostream>
#include <map>
#include <string>
using namespace std;
int main() {
    cout << "=== Map Dictionary ===" << endl;
    map<string, int> ages;
    ages["Alice"] = 25; ages["Bob"] = 30; ages["Charlie"] = 35;
    cout << "Ages:" << endl;
    for(const auto& [name, age] : ages) 
        cout << "  " << name << ": " << age << endl;
    cout << "Alice's age: " << ages["Alice"] << endl;
    if(ages.find("Diana") == ages.end())
        cout << "Diana not found" << endl;
    ages.erase("Bob");
    cout << "After removing Bob:" << endl;
    for(const auto& [name, age] : ages)
        cout << "  " << name << ": " << age << endl;
    return 0;
}
