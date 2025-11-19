/*
 * Production-Grade Custom Memory Allocators
 * Demonstrates various allocation strategies for performance
 */

#ifndef CUSTOM_ALLOCATORS_H
#define CUSTOM_ALLOCATORS_H

#include <cstddef>
#include <cstdlib>
#include <new>
#include <memory>
#include <cassert>
#include <vector>
#include <iostream>

// ========== Stack Allocator (Linear/Arena Allocator) ==========
template<size_t Size>
class StackAllocator {
private:
    alignas(16) char buffer_[Size];
    char* current_;

public:
    StackAllocator() : current_(buffer_) {}

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        // Align current pointer
        size_t space = Size - (current_ - buffer_);
        void* ptr = current_;

        if (!std::align(alignment, size, ptr, space)) {
            throw std::bad_alloc();
        }

        current_ = static_cast<char*>(ptr) + size;
        return ptr;
    }

    void deallocate(void* /*ptr*/) {
        // Stack allocator doesn't support individual deallocations
        // Use reset() to free all memory
    }

    void reset() {
        current_ = buffer_;
    }

    size_t bytes_used() const {
        return current_ - buffer_;
    }

    size_t bytes_available() const {
        return Size - bytes_used();
    }
};

// ========== Pool Allocator (Fixed-Size Blocks) ==========
template<typename T, size_t BlockSize = 4096>
class PoolAllocator {
private:
    union Node {
        Node* next;
        alignas(T) char data[sizeof(T)];
    };

    struct Block {
        alignas(Node) char memory[BlockSize];
        Block* next;
    };

    Node* free_list_;
    Block* blocks_;
    size_t allocations_;
    size_t deallocations_;

    void add_block() {
        Block* new_block = static_cast<Block*>(std::malloc(sizeof(Block)));
        if (!new_block) throw std::bad_alloc();

        new_block->next = blocks_;
        blocks_ = new_block;

        // Add all nodes from this block to free list
        const size_t nodes_per_block = BlockSize / sizeof(Node);

        for (size_t i = 0; i < nodes_per_block; ++i) {
            Node* node = reinterpret_cast<Node*>(new_block->memory + i * sizeof(Node));
            node->next = free_list_;
            free_list_ = node;
        }
    }

public:
    PoolAllocator() : free_list_(nullptr), blocks_(nullptr),
                      allocations_(0), deallocations_(0) {
        add_block();
    }

    ~PoolAllocator() {
        while (blocks_) {
            Block* next = blocks_->next;
            std::free(blocks_);
            blocks_ = next;
        }
    }

    T* allocate() {
        if (!free_list_) {
            add_block();
        }

        Node* node = free_list_;
        free_list_ = node->next;
        ++allocations_;

        return reinterpret_cast<T*>(node);
    }

    void deallocate(T* ptr) {
        Node* node = reinterpret_cast<Node*>(ptr);
        node->next = free_list_;
        free_list_ = node;
        ++deallocations_;
    }

    size_t allocation_count() const { return allocations_; }
    size_t deallocation_count() const { return deallocations_; }
};

// ========== Monotonic Allocator (Bump Allocator) ==========
class MonotonicAllocator {
private:
    char* buffer_;
    size_t capacity_;
    size_t offset_;

public:
    MonotonicAllocator(size_t capacity)
        : capacity_(capacity), offset_(0) {
        buffer_ = static_cast<char*>(std::malloc(capacity));
        if (!buffer_) throw std::bad_alloc();
    }

    ~MonotonicAllocator() {
        std::free(buffer_);
    }

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        // Align offset
        size_t padding = 0;
        size_t aligned_offset = offset_;

        if (alignment > 0 && (aligned_offset % alignment) != 0) {
            padding = alignment - (aligned_offset % alignment);
            aligned_offset += padding;
        }

        if (aligned_offset + size > capacity_) {
            throw std::bad_alloc();
        }

        void* ptr = buffer_ + aligned_offset;
        offset_ = aligned_offset + size;

        return ptr;
    }

    void reset() {
        offset_ = 0;
    }

    size_t bytes_used() const { return offset_; }
    size_t bytes_available() const { return capacity_ - offset_; }
};

// ========== Free List Allocator ==========
class FreeListAllocator {
private:
    struct Header {
        size_t size;
        size_t padding;
    };

    struct FreeBlock {
        size_t size;
        FreeBlock* next;
    };

    void* buffer_;
    size_t capacity_;
    FreeBlock* free_list_;
    size_t used_memory_;

public:
    FreeListAllocator(size_t capacity)
        : capacity_(capacity), used_memory_(0) {

        buffer_ = std::malloc(capacity);
        if (!buffer_) throw std::bad_alloc();

        free_list_ = static_cast<FreeBlock*>(buffer_);
        free_list_->size = capacity;
        free_list_->next = nullptr;
    }

    ~FreeListAllocator() {
        std::free(buffer_);
    }

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        FreeBlock* prev = nullptr;
        FreeBlock* current = free_list_;

        while (current) {
            // Calculate padding
            size_t padding = 0;
            uintptr_t address = reinterpret_cast<uintptr_t>(current) + sizeof(Header);

            if (alignment > 0 && (address % alignment) != 0) {
                padding = alignment - (address % alignment);
            }

            size_t required_size = sizeof(Header) + padding + size;

            if (current->size >= required_size) {
                // Found a suitable block
                if (current->size - required_size > sizeof(FreeBlock)) {
                    // Split the block
                    FreeBlock* new_block = reinterpret_cast<FreeBlock*>(
                        reinterpret_cast<char*>(current) + required_size
                    );
                    new_block->size = current->size - required_size;
                    new_block->next = current->next;

                    if (prev) {
                        prev->next = new_block;
                    } else {
                        free_list_ = new_block;
                    }
                } else {
                    // Use entire block
                    if (prev) {
                        prev->next = current->next;
                    } else {
                        free_list_ = current->next;
                    }
                }

                // Set header
                Header* header = reinterpret_cast<Header*>(current);
                header->size = required_size;
                header->padding = padding;

                used_memory_ += required_size;

                return reinterpret_cast<void*>(address + padding);
            }

            prev = current;
            current = current->next;
        }

        throw std::bad_alloc();
    }

    void deallocate(void* ptr) {
        if (!ptr) return;

        // Get header
        Header* header = reinterpret_cast<Header*>(
            static_cast<char*>(ptr) - sizeof(Header)
        );

        FreeBlock* block = reinterpret_cast<FreeBlock*>(header);
        block->size = header->size;
        block->next = free_list_;
        free_list_ = block;

        used_memory_ -= header->size;
    }

    size_t bytes_used() const { return used_memory_; }
    size_t bytes_available() const { return capacity_ - used_memory_; }
};

// ========== STL-Compatible Allocator Wrapper ==========
template<typename T, typename Allocator>
class STLAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = STLAllocator<U, Allocator>;
    };

private:
    Allocator* allocator_;

public:
    explicit STLAllocator(Allocator* alloc) : allocator_(alloc) {}

    template<typename U>
    STLAllocator(const STLAllocator<U, Allocator>& other)
        : allocator_(other.allocator_) {}

    pointer allocate(size_type n) {
        return static_cast<pointer>(
            allocator_->allocate(n * sizeof(T), alignof(T))
        );
    }

    void deallocate(pointer p, size_type /*n*/) {
        allocator_->deallocate(p);
    }

    template<typename U>
    bool operator==(const STLAllocator<U, Allocator>& other) const {
        return allocator_ == other.allocator_;
    }

    template<typename U>
    bool operator!=(const STLAllocator<U, Allocator>& other) const {
        return !(*this == other);
    }

    template<typename U, typename A>
    friend class STLAllocator;
};

#endif // CUSTOM_ALLOCATORS_H
