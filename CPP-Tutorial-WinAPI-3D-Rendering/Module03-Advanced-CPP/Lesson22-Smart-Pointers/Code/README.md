# Lesson 22: Smart Pointers - Code Examples

## Files Overview

1. **01_unique_ptr_basics.cpp** - unique_ptr fundamentals
2. **02_shared_ptr_basics.cpp** - shared_ptr and reference counting
3. **03_weak_ptr_basics.cpp** - weak_ptr for observation
4. **04_circular_reference.cpp** - Breaking cycles with weak_ptr
5. **05_custom_deleters.cpp** - Custom deletion for resources

## Compilation

```bash
g++ -std=c++17 -Wall filename.cpp -o filename
```

## Key Concepts

- **unique_ptr**: Exclusive ownership, move-only
- **shared_ptr**: Shared ownership, reference counted
- **weak_ptr**: Non-owning observer, breaks cycles
- **Custom deleters**: For non-memory resources

## Best Practices

1. Prefer `make_unique` and `make_shared`
2. Use `unique_ptr` by default
3. Use `weak_ptr` to break circular references
4. Pass smart pointers appropriately
5. Avoid raw `new`/`delete`
