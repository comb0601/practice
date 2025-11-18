/* Lesson 67: String Streams
 * Compile: cl Lesson67_StringStreams.cpp */
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    // WRITING TO STRING STREAM
    ostringstream oss;
    oss << "Value: " << 42 << ", Pi: " << 3.14;
    string result = oss.str();
    cout << result << endl;

    // READING FROM STRING STREAM
    istringstream iss("10 20 30");
    int a, b, c;
    iss >> a >> b >> c;
    cout << "Read: " << a << ", " << b << ", " << c << endl;

    // STRING TO NUMBER
    string numStr = "123";
    istringstream converter(numStr);
    int num;
    converter >> num;
    cout << "Converted: " << num << endl;

    return 0;
}
