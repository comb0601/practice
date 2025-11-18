/* Lesson 62: Custom Exceptions
 * Compile: cl Lesson62_CustomExceptions.cpp */
#include <iostream>
#include <exception>
#include <string>
using namespace std;

class InvalidAgeException : public exception {
private:
    string message;
public:
    InvalidAgeException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

void setAge(int age) {
    if (age < 0 || age > 150) {
        throw InvalidAgeException("Age must be between 0 and 150");
    }
    cout << "Age set to " << age << endl;
}

int main() {
    try {
        setAge(25);
        setAge(-5);
    }
    catch (const InvalidAgeException& e) {
        cout << "Exception: " << e.what() << endl;
    }
    return 0;
}
