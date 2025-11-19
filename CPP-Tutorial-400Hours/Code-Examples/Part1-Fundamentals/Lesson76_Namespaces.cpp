/* Lesson 76: Namespaces
 * Compile: cl Lesson76_Namespaces.cpp */
#include <iostream>
using namespace std;

namespace Math {
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
}

namespace Graphics {
    void draw() { cout << "Drawing..." << endl; }
}

namespace Nested {
    namespace Inner {
        void func() { cout << "Nested function" << endl; }
    }
}

int main() {
    cout << "Math::add(5, 3) = " << Math::add(5, 3) << endl;
    Graphics::draw();
    Nested::Inner::func();

    using namespace Math;
    cout << "multiply(4, 5) = " << multiply(4, 5) << endl;

    return 0;
}
