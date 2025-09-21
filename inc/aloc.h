#ifndef ALOC_H
#define ALOC_H

#include <iostream>
#include <stdexcept>
#include <cstddef> // for std::size_t
#include <new>     // for std::bad_alloc

template <typename T>
class BoundedAllocator {
    std::size_t total_size;       // Maximum allowed allocation size in bytes
    std::size_t allocated_size;   // Currently allocated size in bytes

public:
    using value_type = T;

    // Constructor
    explicit BoundedAllocator(std::size_t max_size)
        : total_size(max_size), allocated_size(0) {
        std::cout << "BoundedAllocator created with total_size = " << total_size << " bytes\n";
    }

    // Allocate memory
    T* allocate(std::size_t n) {
        std::size_t bytes_to_allocate = n * sizeof(T);
        if (allocated_size + bytes_to_allocate > total_size) {
            throw std::bad_alloc(); // Exceeds the allowed allocation size
        }
        allocated_size += bytes_to_allocate;
        std::cout << "Allocating " << bytes_to_allocate << " bytes. Total allocated: " << allocated_size << " bytes\n";
        return static_cast<T*>(::operator new(bytes_to_allocate));
    }

    // Deallocate memory
    void deallocate(T* p, std::size_t n) noexcept {
        std::size_t bytes_to_deallocate = n * sizeof(T);
        allocated_size -= bytes_to_deallocate;
        std::cout << "Deallocating " << bytes_to_deallocate << " bytes. Total allocated: " << allocated_size << " bytes\n";
        ::operator delete(p);
    }

    // Construct an object in-place
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new (p) U(std::forward<Args>(args)...);
    }

    // Destroy an object
    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

    // Equality comparison (required for allocators)
    bool operator==(const BoundedAllocator&) const noexcept {
        return true;
    }

    bool operator!=(const BoundedAllocator&) const noexcept {
        return false;
    }
};

#endif // ALOC_H