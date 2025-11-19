/*
 * Lesson 20: std::string - Modern C++ Strings
 * Compile: cl Lesson20_StdString.cpp
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "=== std::string Demo ===" << endl << endl;

    // STRING CREATION
    string s1 = "Hello";
    string s2("World");
    string s3(5, 'A');  // "AAAAA"

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl << endl;

    // STRING CONCATENATION
    string full = s1 + " " + s2;
    cout << "Concatenated: " << full << endl << endl;

    // STRING LENGTH
    cout << "Length: " << full.length() << endl;
    cout << "Size: " << full.size() << endl;
    cout << "Empty? " << (full.empty() ? "Yes" : "No") << endl << endl;

    // ACCESSING CHARACTERS
    cout << "First char: " << full[0] << endl;
    cout << "Last char: " << full[full.length()-1] << endl;
    cout << "At index 6: " << full.at(6) << endl << endl;

    // SUBSTRING
    string sub = full.substr(0, 5);
    cout << "Substring (0,5): " << sub << endl << endl;

    // FIND
    size_t pos = full.find("World");
    if (pos != string::npos) {
        cout << "'World' found at position: " << pos << endl;
    }
    cout << endl;

    // REPLACE
    string text = "I like Java";
    text.replace(7, 4, "C++");
    cout << "After replace: " << text << endl << endl;

    // INSERT
    string msg = "Hello";
    msg.insert(5, " World");
    cout << "After insert: " << msg << endl << endl;

    // ERASE
    msg.erase(5, 6);
    cout << "After erase: " << msg << endl << endl;

    // APPEND
    msg.append("!");
    cout << "After append: " << msg << endl << endl;

    // COMPARISON
    string a = "apple", b = "banana";
    cout << "Comparing '" << a << "' and '" << b << "': ";
    if (a < b) cout << a << " comes first" << endl;
    cout << endl;

    // CONVERSIONS
    int num = 42;
    string numStr = to_string(num);
    cout << "Int to string: " << numStr << endl;

    string strNum = "123";
    int value = stoi(strNum);
    cout << "String to int: " << value << endl << endl;

    // STRING MANIPULATION
    string lower = "hello";
    string upper = lower;
    for (char &c : upper) c = toupper(c);
    cout << "Lowercase: " << lower << endl;
    cout << "Uppercase: " << upper << endl;

    return 0;
}
