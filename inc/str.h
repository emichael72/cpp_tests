#ifndef STR_H
#define STR_H

#include <string.h>

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

#endif // STR_H