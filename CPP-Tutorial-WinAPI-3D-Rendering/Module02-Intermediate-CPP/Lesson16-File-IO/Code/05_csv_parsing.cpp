#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
int main() {
    ofstream out("data.csv");
    out << "Name,Age,Score\n";
    out << "Alice,25,95\n";
    out << "Bob,30,87\n";
    out.close();
    ifstream in("data.csv");
    string line;
    getline(in, line);  // Skip header
    while(getline(in, line)) {
        stringstream ss(line);
        string name, age, score;
        getline(ss, name, ',');
        getline(ss, age, ',');
        getline(ss, score, ',');
        cout << name << " is " << age << " years old with score " << score << endl;
    }
    in.close();
    return 0;
}
