// clang-format WebKit style
// implementation of a unique pointer class with move semantics.

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class UniquePtr {
    T* ptr;

public:
    explicit UniquePtr(T* p = nullptr)
        : ptr(p)
    {
    } // ToDo: explicit? why?

    ~UniquePtr()
    {
        cout << "deleting " << name << endl;
        delete ptr;
        ptr = nullptr;
    }

    // kill copy constructor and the '=' operator
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    string name = "unnamed";

    // handles move constructor (aka the && operator)
    // TODO how does the compiler know to map '&&' to std::move() ?
    UniquePtr(UniquePtr&& other) noexcept
        : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    // assigne operator for move , exampel p2 = std::move(p1);
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // access
    T* get() const { return ptr; }

    // manual release so I could get row pointer from unique ptr int* raw = p.release();
    T* release()
    {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }
};

int main()
{
    UniquePtr<int> p1(new int(42));
    try {
        p1.name = "p1";
    

        // UniquePtr<int> p2 = p1; // ok - getting compile time error

        UniquePtr<int> p2 = std::move(p1); // ok

        p2.name = "p2";

        if (p1.get() == nullptr) {
            cout << p1.name << " is now nullptr after move.\n";
        }

        int* x = p2.release(); // returns raw pointer and sets p2 to nullptr

        if (p2.get() == nullptr) {
            throw std::runtime_error("p2 is nullptr after move!");
        }

        // doing just 'return p2.get()' gives runtime error, however I would expect this to pass given the exception handling above
        return p2.get() != nullptr ? 0 : 1;

    } catch (const std::runtime_error& e) {
        cout << "caught exception:" << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // TODO: why p1 gets deleted at at_exit()?
}
