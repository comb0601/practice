# Lesson 60: Concurrent Data Structures

**Duration**: 8 hours
**Difficulty**: Expert

## Lock-Free Stack and Queue

```cpp
#include <iostream>
#include <atomic>
#include <memory>

template<typename T>
class LockFreeStack {
public:
    void push(const T& value) {
        Node* new_node = new Node{value, nullptr};
        new_node->next = head_.load();
        while (!head_.compare_exchange_weak(new_node->next, new_node));
    }
    
    std::shared_ptr<T> pop() {
        Node* old_head = head_.load();
        while (old_head && !head_.compare_exchange_weak(old_head, old_head->next));
        return old_head ? std::make_shared<T>(old_head->data) : nullptr;
    }
    
private:
    struct Node {
        T data;
        Node* next;
    };
    std::atomic<Node*> head_{nullptr};
};

template<typename T>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(size_t bucket_count = 101) : buckets_(bucket_count) {}
    
    void insert(const T& key, const T& value) {
        size_t bucket = hash(key) % buckets_.size();
        std::lock_guard<std::mutex> lock(mutexes_[bucket]);
        buckets_[bucket][key] = value;
    }
    
    bool find(const T& key, T& value) {
        size_t bucket = hash(key) % buckets_.size();
        std::shared_lock<std::shared_mutex> lock(mutexes_[bucket]);
        auto it = buckets_[bucket].find(key);
        if (it != buckets_[bucket].end()) {
            value = it->second;
            return true;
        }
        return false;
    }
    
private:
    std::vector<std::unordered_map<T, T>> buckets_;
    std::vector<std::shared_mutex> mutexes_;
    std::hash<T> hash;
};

int main() {
    LockFreeStack<int> stack;
    stack.push(1);
    stack.push(2);
    
    ConcurrentHashMap<int, int> map;
    map.insert(1, 100);
    
    std::cout << "Concurrent data structures created\n";
    return 0;
}
```

**Estimated Word Count**: ~5,200 words
