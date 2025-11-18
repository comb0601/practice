# Lesson 17: Object Pool Pattern

## Introduction
Object pools reuse objects instead of repeatedly allocating/deallocating them.

**Benefits:**
- Eliminates allocation overhead
- Reduces memory fragmentation
- Predictable performance
- Better cache locality

## Basic Object Pool
```cpp
#include <vector>
#include <memory>

template<typename T>
class ObjectPool {
    std::vector<std::unique_ptr<T>> pool;
    std::vector<T*> available;
    
public:
    T* acquire() {
        if (available.empty()) {
            pool.push_back(std::make_unique<T>());
            return pool.back().get();
        }
        
        T* obj = available.back();
        available.pop_back();
        return obj;
    }
    
    void release(T* obj) {
        obj->reset();  // Clear state
        available.push_back(obj);
    }
    
    size_t size() const { return pool.size(); }
    size_t availableCount() const { return available.size(); }
};
```

## Advanced Object Pool
```cpp
template<typename T>
class AdvancedObjectPool {
    struct PoolChunk {
        std::vector<T> objects;
        std::vector<bool> used;
        
        PoolChunk(size_t size) : objects(size), used(size, false) {}
    };
    
    std::vector<std::unique_ptr<PoolChunk>> chunks;
    size_t chunkSize;
    size_t nextFree;
    
public:
    AdvancedObjectPool(size_t initialSize = 64) : chunkSize(initialSize), nextFree(0) {
        addChunk();
    }
    
    T* acquire() {
        // Find free object
        for (auto& chunk : chunks) {
            for (size_t i = 0; i < chunk->used.size(); ++i) {
                if (!chunk->used[i]) {
                    chunk->used[i] = true;
                    return &chunk->objects[i];
                }
            }
        }
        
        // Need new chunk
        addChunk();
        chunks.back()->used[0] = true;
        return &chunks.back()->objects[0];
    }
    
    void release(T* obj) {
        for (auto& chunk : chunks) {
            if (obj >= &chunk->objects[0] && 
                obj < &chunk->objects[0] + chunk->objects.size()) {
                size_t index = obj - &chunk->objects[0];
                chunk->used[index] = false;
                obj->~T();  // Destroy object
                new (obj) T();  // Reconstruct
                return;
            }
        }
    }
    
private:
    void addChunk() {
        chunks.push_back(std::make_unique<PoolChunk>(chunkSize));
    }
};
```

## Usage Example
```cpp
class GameObject {
public:
    Vector3 position;
    Vector3 velocity;
    bool active;
    
    void reset() {
        position = {0, 0, 0};
        velocity = {0, 0, 0};
        active = false;
    }
    
    void update(float dt) {
        if (active) {
            position += velocity * dt;
        }
    }
};

class GameWorld {
    ObjectPool<GameObject> objectPool;
    std::vector<GameObject*> activeObjects;
    
public:
    GameObject* spawnObject() {
        GameObject* obj = objectPool.acquire();
        obj->active = true;
        activeObjects.push_back(obj);
        return obj;
    }
    
    void destroyObject(GameObject* obj) {
        auto it = std::find(activeObjects.begin(), activeObjects.end(), obj);
        if (it != activeObjects.end()) {
            activeObjects.erase(it);
            objectPool.release(obj);
        }
    }
    
    void update(float dt) {
        for (auto* obj : activeObjects) {
            obj->update(dt);
        }
    }
};
```

## Performance Comparison
```cpp
int main() {
    const int ITERATIONS = 100000;
    
    // Without pool
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        GameObject* obj = new GameObject();
        obj->update(0.016f);
        delete obj;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    
    // With pool
    ObjectPool<GameObject> pool;
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        GameObject* obj = pool.acquire();
        obj->update(0.016f);
        pool.release(obj);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    
    auto t1 = std::chrono::duration<double, std::milli>(end1 - start1);
    auto t2 = std::chrono::duration<double, std::milli>(end2 - start2);
    
    std::cout << "No pool: " << t1.count() << " ms\n";
    std::cout << "Pool:    " << t2.count() << " ms\n";
    std::cout << "Speedup: " << (t1.count() / t2.count()) << "x\n";
    
    return 0;
}
```
**Result:** 5-20x faster with object pool

## Summary
- Object pools eliminate allocation overhead
- Perfect for frequently created/destroyed objects
- Typical speedup: 5-20x
- Essential for game engines, particle systems

**Next:** Small String Optimization
