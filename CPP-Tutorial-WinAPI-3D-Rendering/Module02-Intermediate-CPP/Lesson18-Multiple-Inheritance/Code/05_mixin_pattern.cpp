#include <iostream>
using namespace std;
template<typename T>
class Timestamped : public T {
    time_t timestamp;
public:
    Timestamped() : timestamp(time(nullptr)) {}
    time_t getTimestamp() { return timestamp; }
};
class Data {
public:
    int value = 42;
};
int main() {
    Timestamped<Data> td;
    cout << "Value: " << td.value << endl;
    cout << "Timestamp: " << td.getTimestamp() << endl;
    return 0;
}
