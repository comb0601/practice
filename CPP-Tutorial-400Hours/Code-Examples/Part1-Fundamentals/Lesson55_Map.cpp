/* Lesson 55: std::map
 * Compile: cl /std:c++17 Lesson55_Map.cpp */
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> ages;

    ages["Alice"] = 25;
    ages["Bob"] = 30;
    ages["Charlie"] = 35;

    cout << "Ages:" << endl;
    for (const auto& pair : ages) {
        cout << pair.first << ": " << pair.second << endl;
    }

    ages.erase("Bob");

    if (ages.count("Alice")) {
        cout << "Alice's age: " << ages["Alice"] << endl;
    }

    return 0;
}
