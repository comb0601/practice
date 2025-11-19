/*
 * Lesson 22: Smart Pointers - Comprehensive Guide
 *
 * This example demonstrates unique_ptr, shared_ptr, and weak_ptr
 * with practical examples and best practices.
 *
 * Compilation: g++ -std=c++17 01_smart_pointers_comprehensive.cpp -o smart_ptrs
 * Execution: ./smart_ptrs
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Example class for demonstrations
class Resource {
private:
    std::string name;
    int id;
    static int nextId;

public:
    Resource(const std::string& n) : name(n), id(nextId++) {
        std::cout << "  [+] Resource '" << name << "' (ID: " << id << ") created" << std::endl;
    }

    ~Resource() {
        std::cout << "  [-] Resource '" << name << "' (ID: " << id << ") destroyed" << std::endl;
    }

    void use() const {
        std::cout << "  [*] Using Resource '" << name << "' (ID: " << id << ")" << std::endl;
    }

    const std::string& getName() const { return name; }
    int getId() const { return id; }
};

int Resource::nextId = 1;

// Class that owns a resource
class Owner {
private:
    std::string ownerName;
    std::shared_ptr<Resource> resource;

public:
    Owner(const std::string& name, std::shared_ptr<Resource> res)
        : ownerName(name), resource(res) {
        std::cout << "  Owner '" << ownerName << "' created" << std::endl;
    }

    ~Owner() {
        std::cout << "  Owner '" << ownerName << "' destroyed" << std::endl;
    }

    void useResource() {
        if (resource) {
            std::cout << "  " << ownerName << " is using: ";
            resource->use();
        }
    }

    std::shared_ptr<Resource> getResource() const {
        return resource;
    }
};

// Observer pattern with weak_ptr
class Observer {
private:
    std::string name;
    std::weak_ptr<Resource> watchedResource;

public:
    Observer(const std::string& n, std::shared_ptr<Resource> res)
        : name(n), watchedResource(res) {
        std::cout << "  Observer '" << name << "' watching resource" << std::endl;
    }

    void check() {
        std::cout << "  Observer '" << name << "' checking: ";
        auto resource = watchedResource.lock();  // Try to get shared_ptr
        if (resource) {
            std::cout << "Resource still alive - ";
            resource->use();
        } else {
            std::cout << "Resource has been destroyed!" << std::endl;
        }
    }
};

// Factory function returning unique_ptr
std::unique_ptr<Resource> createResource(const std::string& name) {
    return std::make_unique<Resource>(name);
}

// Function taking unique_ptr by value (transfer ownership)
void consumeResource(std::unique_ptr<Resource> res) {
    std::cout << "  consumeResource() received ownership" << std::endl;
    res->use();
    // res is destroyed when function ends
}

// Function taking shared_ptr by value (share ownership)
void shareResource(std::shared_ptr<Resource> res) {
    std::cout << "  shareResource() sharing ownership (count: "
              << res.use_count() << ")" << std::endl;
    res->use();
}

// Custom deleter example
void customDeleter(Resource* ptr) {
    std::cout << "  [CUSTOM DELETER] Deleting resource" << std::endl;
    delete ptr;
}

int main() {
    std::cout << "=== Comprehensive Smart Pointers Tutorial ===" << std::endl;

    // ===================================================================
    // 1. UNIQUE_PTR - Exclusive ownership
    // ===================================================================
    std::cout << "\n1. UNIQUE_PTR - Exclusive Ownership" << std::endl;
    std::cout << "====================================" << std::endl;

    {
        std::cout << "\nCreating unique_ptr:" << std::endl;
        std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>("UniqueRes1");
        ptr1->use();

        std::cout << "\nMoving unique_ptr:" << std::endl;
        std::unique_ptr<Resource> ptr2 = std::move(ptr1);  // Transfer ownership
        
        std::cout << "ptr1 is now: " << (ptr1 ? "valid" : "null") << std::endl;
        std::cout << "ptr2 is now: " << (ptr2 ? "valid" : "null") << std::endl;
        
        if (ptr2) {
            ptr2->use();
        }

        std::cout << "\nUsing factory function:" << std::endl;
        auto ptr3 = createResource("FactoryRes");
        ptr3->use();

        std::cout << "\nTransferring to function:" << std::endl;
        auto ptr4 = std::make_unique<Resource>("TransferRes");
        consumeResource(std::move(ptr4));
        std::cout << "ptr4 is now: " << (ptr4 ? "valid" : "null") << std::endl;

        std::cout << "\nLeaving scope - automatic cleanup:" << std::endl;
    }

    // ===================================================================
    // 2. SHARED_PTR - Shared ownership with reference counting
    // ===================================================================
    std::cout << "\n2. SHARED_PTR - Shared Ownership" << std::endl;
    std::cout << "=================================" << std::endl;

    {
        std::cout << "\nCreating shared_ptr:" << std::endl;
        std::shared_ptr<Resource> sptr1 = std::make_shared<Resource>("SharedRes1");
        std::cout << "Reference count: " << sptr1.use_count() << std::endl;

        {
            std::cout << "\nCreating second reference:" << std::endl;
            std::shared_ptr<Resource> sptr2 = sptr1;  // Copy - increases ref count
            std::cout << "Reference count: " << sptr1.use_count() << std::endl;

            {
                std::cout << "\nCreating third reference:" << std::endl;
                std::shared_ptr<Resource> sptr3 = sptr2;
                std::cout << "Reference count: " << sptr1.use_count() << std::endl;

                std::cout << "\nLeaving inner scope (sptr3 destroyed):" << std::endl;
            }
            std::cout << "Reference count after sptr3 destroyed: " << sptr1.use_count() << std::endl;

            std::cout << "\nLeaving middle scope (sptr2 destroyed):" << std::endl;
        }
        std::cout << "Reference count after sptr2 destroyed: " << sptr1.use_count() << std::endl;

        std::cout << "\nLeaving outer scope (sptr1 destroyed):" << std::endl;
    }

    // ===================================================================
    // 3. WEAK_PTR - Non-owning reference
    // ===================================================================
    std::cout << "\n3. WEAK_PTR - Non-Owning Reference" << std::endl;
    std::cout << "===================================" << std::endl;

    {
        std::cout << "\nCreating shared_ptr and weak_ptr:" << std::endl;
        std::shared_ptr<Resource> shared = std::make_shared<Resource>("WeakRes");
        std::weak_ptr<Resource> weak = shared;

        std::cout << "Shared ref count: " << shared.use_count() << std::endl;
        std::cout << "Weak expired: " << (weak.expired() ? "yes" : "no") << std::endl;

        std::cout << "\nAccessing through weak_ptr:" << std::endl;
        if (auto locked = weak.lock()) {
            locked->use();
            std::cout << "Shared ref count during lock: " << shared.use_count() << std::endl;
        }

        std::cout << "\nResetting shared_ptr:" << std::endl;
        shared.reset();
        std::cout << "Weak expired: " << (weak.expired() ? "yes" : "no") << std::endl;

        std::cout << "\nTrying to access after expiration:" << std::endl;
        if (auto locked = weak.lock()) {
            locked->use();
        } else {
            std::cout << "  Cannot lock - resource is gone!" << std::endl;
        }
    }

    // ===================================================================
    // 4. PRACTICAL EXAMPLE - Observer Pattern
    // ===================================================================
    std::cout << "\n4. Observer Pattern with weak_ptr" << std::endl;
    std::cout << "==================================" << std::endl;

    {
        std::cout << "\nCreating resource and observers:" << std::endl;
        auto resource = std::make_shared<Resource>("ObservedRes");
        Observer obs1("Observer1", resource);
        Observer obs2("Observer2", resource);

        std::cout << "\nObservers checking (resource alive):" << std::endl;
        obs1.check();
        obs2.check();

        std::cout << "\nDestroying resource:" << std::endl;
        resource.reset();

        std::cout << "\nObservers checking (resource destroyed):" << std::endl;
        obs1.check();
        obs2.check();
    }

    // ===================================================================
    // 5. SHARED_PTR with Multiple Owners
    // ===================================================================
    std::cout << "\n5. Multiple Owners with shared_ptr" << std::endl;
    std::cout << "===================================" << std::endl;

    {
        std::cout << "\nCreating shared resource:" << std::endl;
        auto sharedRes = std::make_shared<Resource>("MultiOwnerRes");

        std::cout << "\nCreating multiple owners:" << std::endl;
        Owner owner1("Owner1", sharedRes);
        Owner owner2("Owner2", sharedRes);

        std::cout << "\nReference count: " << sharedRes.use_count() << std::endl;

        std::cout << "\nOwners using resource:" << std::endl;
        owner1.useResource();
        owner2.useResource();

        std::cout << "\nLeaving scope - owners destroyed first, then resource:" << std::endl;
    }

    // ===================================================================
    // 6. Custom Deleters
    // ===================================================================
    std::cout << "\n6. Custom Deleters" << std::endl;
    std::cout << "==================" << std::endl;

    {
        std::cout << "\nUnique_ptr with custom deleter:" << std::endl;
        std::unique_ptr<Resource, decltype(&customDeleter)>
            customPtr(new Resource("CustomDelRes"), customDeleter);
        customPtr->use();

        std::cout << "\nShared_ptr with custom deleter:" << std::endl;
        std::shared_ptr<Resource> customShared(new Resource("CustomDelShared"), customDeleter);
        customShared->use();

        std::cout << "\nLeaving scope:" << std::endl;
    }

    // ===================================================================
    // 7. Arrays with Smart Pointers
    // ===================================================================
    std::cout << "\n7. Arrays with Smart Pointers" << std::endl;
    std::cout << "==============================" << std::endl;

    {
        std::cout << "\nUnique_ptr with array:" << std::endl;
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }

        std::cout << "Array contents: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    // ===================================================================
    // 8. Best Practices Summary
    // ===================================================================
    std::cout << "\n8. Best Practices" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "1. Prefer make_unique and make_shared" << std::endl;
    std::cout << "2. Use unique_ptr by default (exclusive ownership)" << std::endl;
    std::cout << "3. Use shared_ptr only when you need shared ownership" << std::endl;
    std::cout << "4. Use weak_ptr to break circular references" << std::endl;
    std::cout << "5. Never mix smart pointers with raw pointers" << std::endl;
    std::cout << "6. Pass unique_ptr by move, shared_ptr by const reference" << std::endl;

    std::cout << "\nProgram ending - all resources cleaned up automatically!" << std::endl;
    return 0;
}
