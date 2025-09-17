#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <memory>  // for std::default_delete
#include <utility> // for std::move

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

  T *release() {
    T *tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  void reset(T *p = nullptr) {
    if (m_ptr) {
      m_deleter(m_ptr);
    }
    m_ptr = p;
  }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  T *m_ptr;
  DeleterType m_deleter;
};

#endif // UNIQUE_PTR_H
