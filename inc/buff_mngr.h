/*

cases where a class should implement all three:

copy constructor
copy assignment operator
move constructor / move assignment operator

You implement all of them when your class manages a resource (e.g., dynamic
memory, file handle, buffer, socket, etc.) and you want:

To allow safe copying (duplicating the resource),
and To allow efficient moving (transferring ownership without copying).

*/

#include <iostream>
#include <cxxabi.h>
#include <cstring>   // for std::memcpy
#include <utility>   // for std::move

std::string demangle(const char* name) { // c++ mangled text to human readable string 
    int status;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string result = (status == 0) ? demangled : name;
    free(demangled);
    return result;
}

template <typename T>
void print_type() {
    std::cout << demangle(typeid(T).name()) << '\n';
}

template<typename T>
class Buffer {
public:
    explicit Buffer(size_t size)
        : m_size(size), m_data(new T[size]) {
        std::cout << "Constructed (" << m_size << " elements of type " << demangle(typeid(T).name()) << ")\n";
    }

    ~Buffer() {
        delete[] m_data;
        std::cout << "Destroyed\n";
    }

    // Copy constructor (deep copy)
    Buffer(const Buffer &other)
        : m_size(other.m_size), m_data(new T[other.m_size]) {
        std::memcpy(m_data, other.m_data, m_size * sizeof(T));
        std::cout << "Copied\n";
    }

    // Copy assignment (deep copy)
    Buffer &operator=(const Buffer &other) {
        std::cout << "Copy assigned\n";
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_data = new T[m_size];
            std::memcpy(m_data, other.m_data, m_size * sizeof(T));
        }
        return *this;
    }

    // Move constructor (steal resource)
    Buffer(Buffer &&other) noexcept
        : m_size(other.m_size), m_data(other.m_data) {
        other.m_size = 0;
        other.m_data = nullptr;
        std::cout << "Moved\n";
    }

    // Move assignment (steal resource, cleanup first)
    Buffer &operator=(Buffer &&other) noexcept {
        std::cout << "Move assigned\n";
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_data = other.m_data;
            other.m_size = 0;
            other.m_data = nullptr;
        }
        return *this;
    }

    // Access element
    T &operator[](size_t index) { return m_data[index]; }
    const T &operator[](size_t index) const { return m_data[index]; }

    // Size query
    size_t size() const { return m_size; }

private:
    size_t m_size{0};
    T *m_data{};
};
