
#ifndef ITER_H
#define ITER_H

#include <iostream>

template <typename T>
class SimpleIterator {
    T* ptr;  // raw pointer to element
public:
    explicit SimpleIterator(T* p = nullptr) : ptr(p) {}

    T& operator*() { return *ptr; }

    SimpleIterator& operator++() { 
        ++ptr; 
        return *this; 
    }

    SimpleIterator operator++(int) {
        SimpleIterator tmp = *this;
        ++ptr;
        return tmp;
    }

    // comparison
    bool operator==(const SimpleIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const SimpleIterator& other) const { return ptr != other.ptr; }
};


#endif // ITER_H