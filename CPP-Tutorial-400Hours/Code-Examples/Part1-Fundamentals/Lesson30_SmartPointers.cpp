/*
 * Lesson 30: Smart Pointers - Automatic Memory Management
 * Compile: cl /std:c++17 Lesson30_SmartPointers.cpp
 */

#include <iostream>
#include <memory>
using namespace std;

class Resource {
public:
    Resource(int id) : id(id) {
        cout << "Resource " << id << " created" << endl;
    }
    ~Resource() {
        cout << "Resource " << id << " destroyed" << endl;
    }
    void display() {
        cout << "Resource ID: " << id << endl;
    }
private:
    int id;
};

int main() {
    cout << "=== Smart Pointers Demo ===" << endl << endl;

    // UNIQUE_PTR
    cout << "=== UNIQUE_PTR ===" << endl;
    {
        unique_ptr<int> ptr1 = make_unique<int>(42);
        cout << "Value: " << *ptr1 << endl;

        unique_ptr<Resource> res1 = make_unique<Resource>(1);
        res1->display();

        // unique_ptr<int> ptr2 = ptr1; // Error: cannot copy
        unique_ptr<int> ptr2 = move(ptr1);  // OK: can move
        if (ptr1 == nullptr) {
            cout << "ptr1 is now null after move" << endl;
        }
        cout << "ptr2: " << *ptr2 << endl;

    } // Automatically deleted here
    cout << "Scope ended - resources freed" << endl << endl;

    // SHARED_PTR
    cout << "=== SHARED_PTR ===" << endl;
    {
        shared_ptr<Resource> sptr1 = make_shared<Resource>(2);
        cout << "Reference count: " << sptr1.use_count() << endl;

        {
            shared_ptr<Resource> sptr2 = sptr1;  // Share ownership
            cout << "Reference count after copy: " << sptr1.use_count() << endl;
            sptr2->display();
        }
        cout << "Reference count after inner scope: " << sptr1.use_count() << endl;

    } // Deleted when last shared_ptr goes out of scope
    cout << "Shared resource freed" << endl << endl;

    // WEAK_PTR
    cout << "=== WEAK_PTR ===" << endl;
    {
        shared_ptr<Resource> sptr = make_shared<Resource>(3);
        weak_ptr<Resource> wptr = sptr;  // Doesn't increase ref count

        cout << "Shared ref count: " << sptr.use_count() << endl;
        cout << "Weak ptr expired? " << (wptr.expired() ? "Yes" : "No") << endl;

        if (auto locked = wptr.lock()) {
            locked->display();
        }
    }
    cout << endl;

    // ARRAY WITH UNIQUE_PTR
    cout << "=== UNIQUE_PTR WITH ARRAY ===" << endl;
    {
        unique_ptr<int[]> arr = make_unique<int[]>(5);
        for (int i = 0; i < 5; i++) {
            arr[i] = (i + 1) * 10;
        }

        cout << "Array: ";
        for (int i = 0; i < 5; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    } // Array automatically deleted
    cout << "Array freed automatically" << endl;

    return 0;
}
