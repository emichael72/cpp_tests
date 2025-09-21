
#ifndef ITER_H
#define ITER_H

#include <string.h>
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


class SimpleString {
    char* data;
    size_t length;
public:
    SimpleString(const char* str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }
    ~SimpleString() { delete[] data; }  
    size_t size() const { return length; }
    char& operator[](size_t index) { return data[index]; }
    const char& operator[](size_t index) const { return data[index]; }  

};

#endif // ITER_H