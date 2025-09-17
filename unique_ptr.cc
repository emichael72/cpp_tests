// Homework: Implement a simple UniquePtr class template that manages a
// dynamically allocated object of type T. clang-format WebKit style

#include <iostream>
#include <memory>

typedef void (*DeleterFunc)(int *);

// deleter template
template <typename T, typename DeleterType = std::default_delete<T>>

class UniquePtr {
public:
  explicit UniquePtr(T *p = nullptr, DeleterType d = DeleterType{})
      : m_ptr(p), m_deleter(d) {}

  ~UniquePtr() { reset(); }

  UniquePtr(const UniquePtr &) = delete;
  UniquePtr &operator=(const UniquePtr &) = delete;

  UniquePtr(UniquePtr &&other) noexcept
      : m_ptr(other.m_ptr), m_deleter(std::move(other.m_deleter)) {
    other.m_ptr = nullptr;
  }

  UniquePtr &operator=(UniquePtr &&other) noexcept {
    if (this != &other) {
      reset();
      m_ptr = other.m_ptr;
      m_deleter = std::move(other.m_deleter);
      other.m_ptr = nullptr;
    }
    return *this;
  }

  T *get() const { return m_ptr; }

  // note: take ownership without auto deletion
  T *release() {
    T *tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  // note: delete current and take ownership of new ptr

  void reset(T *p = nullptr) {
    if (m_ptr) {
      m_deleter(m_ptr);
    }
    m_ptr = p;
  }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  // static default_delete is no longer needed; use DeleterType
  T *m_ptr;
  DeleterType m_deleter;
};

// note: C style function as deleter
void my_delete(int *p) {
  std::cout << "my_delete called\n";
  delete p;
}

int main() {
  try {
    // note: falling to default delete
    UniquePtr<int> p1(new int(42));
    int p2_val = 0;

    std::cout << "p1 value: " << *p1.get() << "\n";

    // note: move p1 to p2 (take ownership)
    UniquePtr<int> p2 = std::move(p1);

    // *p2 = (*p1)++; // run time error
    ++(*p2);

    if (p1.get() != nullptr) {
      throw std::runtime_error("p1 is not nullptr after move");
    }

    if (p2.get() != nullptr) {
      p2_val = *(p2.release());
    }

    std::cout << "p2 value: " << p2_val << "\n";

    // note: Using a C-style function as a custom deleter
    UniquePtr<int, DeleterFunc> p3(new int(++p2_val), my_delete);
    std::cout << "p3 value: " << *p3 << "\n";

    return p2_val;

  } catch (...) {
    std::cerr << "unknown exception\n";
  }
}
