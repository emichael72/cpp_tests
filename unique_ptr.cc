// Homework: Implement a simple UniquePtr class template that manages a
// dynamically allocated object of type T. clang-format WebKit style

#include <iostream>

// note: custom deleter
template <typename T> class UniquePtr {
public:
  // note: callable signature void(func(void))

  using DeleterType = void (*)(T *);

  explicit UniquePtr(T *p = nullptr, DeleterType d = nullptr)
      : m_ptr(p), m_deleter(nullptr) {
    m_deleter = d ? d : &UniquePtr::default_delete;
  }

  ~UniquePtr() { reset(); }

  UniquePtr(const UniquePtr &) = delete;
  UniquePtr &operator=(const UniquePtr &) = delete;

  UniquePtr(UniquePtr &&other) noexcept
      : m_ptr(other.m_ptr), m_deleter(other.m_deleter) {
    other.m_ptr = nullptr;
    other.m_deleter = nullptr;
  }

  UniquePtr &operator=(UniquePtr &&other) noexcept {
    if (this != &other) {
      reset();
      m_ptr = other.m_ptr;
      m_deleter = other.m_deleter;
      other.m_ptr = nullptr;
      other.m_deleter = nullptr;
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
    if (m_ptr && m_deleter) {
      m_deleter(m_ptr);
    }
    m_ptr = p;
  }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  static void default_delete(T *p) { delete p; }
  T *m_ptr = nullptr;
  DeleterType m_deleter = nullptr;
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

    UniquePtr<int> p2 = std::move(p1);

    if (p1.get() != nullptr) {
      throw std::runtime_error("p1 is not nullptr after move");
    }

    if (p2.get() != nullptr) {
      p2_val = *(p2.release());
    }

    std::cout << "p2 value: " << p2_val << "\n";

    // note: Using a C-style function as a custom deleter
    UniquePtr<int> p3(new int(++p2_val), my_delete);
    std::cout << "p3 value: " << *p3 << "\n";

    return p2_val;

  } catch (...) {
    std::cerr << "unknown exception\n";
  }
}
