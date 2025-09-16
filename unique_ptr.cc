// clang-format WebKit style
// Implementation of a unique pointer class with move semantics.


#include <iostream>


typedef void (*p_free)(void* ptr);
typedef void *(*p_alloc)(size_t size);


// UniquePtr<char> uniq_p1(malloc(10), free);
// UniquePtr<void> uniq_p2(fd, close);

template <typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* p = nullptr)
        : m_ptr(p)
    {
    }

    explicit UniquePtr(size_t size, p_free freeFunc = nullptr, p_alloc allocFunc = nullptr)
        : m_size(size), m_deleter(freeFunc), m_allocator(allocFunc)
    {
        if (m_size == 0 || freeFunc == nullptr || allocFunc == nullptr) {
            throw std::invalid_argument("UniquePtr: size must be > 0 and allocator/deleter must be provided");
        }

        this->allocator();
    }

    ~UniquePtr()
    {
       this->deleter();
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Handles move constructor
    UniquePtr(UniquePtr&& other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other) {
            
            this->deleter();
            
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    // Access
    T* get() const { return m_ptr; }


    // Manual release
    T* release()
    {
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

private:

    T* m_ptr = nullptr;
    size_t m_size = 0;
    p_free m_deleter = nullptr;
    p_alloc m_allocator = nullptr;
    
    
    void deleter() {
        if(m_ptr != nullptr) {
              if(m_deleter != nullptr) {
                m_deleter(m_ptr);
            } 
            else {
                delete m_ptr;
            }
       }
    }

    void allocator() {
        if(m_ptr == nullptr) {
            if (m_allocator != nullptr) {
                m_ptr = static_cast<T*>(m_allocator(m_size * sizeof(T)));

            }
            else {
                m_ptr = new T;
            }
        }
    }
};

int main()
{
    try {
        
        UniquePtr<int> p1(sizeof(int),free, malloc);
        // UniquePtr<int> p1(new int(42));

        int p_val = 0;
        int dummy = 42;


        *p1.get() = 42;
        UniquePtr<int> p2 = std::move(p1); 

        if (p1.get() != nullptr) {
            throw std::runtime_error("p1 is not nullptr after move");
        }
        
        if (p2.get() != nullptr) {
            p_val = *(p2.release());
        }

        p->doSomething();

        std::cout << "Value: " << p_val << "\n";
        return p_val;

    }
    catch (...) {
        std::cerr << "unknown exception\n";
    }

}
