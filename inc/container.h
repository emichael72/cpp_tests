
#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <iostream>
#include <algorithm> // for sort

struct DataItem {
    std::string name;
    int dummy_int;

    //overload < operator for sorting
    bool operator<(const DataItem& other) const {
        return dummy_int < other.dummy_int;
    }
};


class MiniContainer {
    DataItem data[5];
    size_t length = 0;

public:
    void add(const DataItem& s) {
        if (length < 5) data[length++] = s;
    }

    DataItem* begin() { return data; }
    DataItem* end()   { return data + length; }

    void print() const {
        for (size_t i = 0; i < length; ++i) {
            std::cout << data[i].name << " (" << data[i].dummy_int << ")\n";
        }
    }
};


#endif // CONTAINER_H