
#include <iostream>

struct Trace {
    const char* name;
    Trace(const char* n) : name(n) {
        std::cout << ">>> Enter " << name << "()" << std::endl;
    }
    ~Trace() {
        std::cout << "<<< Exit  " << name << "()\n" << std::endl;
    }
};