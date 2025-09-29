#pragma once
#include <iostream>
class Singleton {
public:

    // === Primary accessor (Meyers' singleton) ===
    // Returns a reference to the single instance.
    // Constructed once on first use, thread-safe since C++11.
    static Singleton& instance() {
        static Singleton inst;
        return inst;
    }

    // === Heap-style accessor (returns pointer) ===
    // For APIs that expect a pointer. Still returns the *same* object
    // every time, not a new one.
    static Singleton* create_on_heap() {
        return &instance();
    }

    // === Global-style accessor ===
    // Provides compatibility with a "global singleton" style, but internally
    // just redirects to the same single instance.
    friend Singleton& get_global_singleton();

    void hello() {
        std::cout << "Hello from Singleton @" << this << "\n";
    }

    // === Explicit destroy (rarely needed) ===
    // Typically avoided, but provided for embedded environments where
    // you might want to free resources deterministically.
    static void destroy_instance() {
        // no-op in this design: instance() lives until program exit
    }

private:
    Singleton() {
        std::cout << "Singleton constructed\n";
    }

    ~Singleton() {
        std::cout << "Singleton destroyed\n";
    }

    // Prevent copy and move — strictly one object.
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

// === Global accessor function ===
// For compatibility with "static global" usage.
// Redirects to the single Meyers' singleton instance.
Singleton& get_global_singleton() {
    return Singleton::instance();
}
 
