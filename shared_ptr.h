#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <iostream>
#include <memory>

// deleter template
template <typename T, typename DeleterType = std::default_delete<T>>

class SharedPtr {
public:
  explicit SharedPtr(T *p = nullptr, DeleterType d = DeleterType{})
      : m_ptr(p), m_deleter(d), ref_count(new uint32_t(1)) {}

  ~SharedPtr() { release(); }

  // copy constructor (when p1 and p2 share the same object)
  SharedPtr(const SharedPtr &other)
      : m_ptr(other.m_ptr), m_deleter(other.m_deleter),
        ref_count(other.ref_count) {
    if (ref_count) {
      ++(*ref_count);
    }
  }

  SharedPtr &operator=(const SharedPtr &other) {
    if (this != &other) {
      release(); // Release current resources
      m_ptr = other.m_ptr;
      m_deleter = other.m_deleter;
      ref_count = other.ref_count;
      if (ref_count) {
        ++(*ref_count); // Increment the shared reference count
      }
    }
    return *this;
  }

  // handle move semantics
  SharedPtr(SharedPtr &&other) noexcept
      : m_ptr(other.m_ptr), m_deleter(std::move(other.m_deleter)),
        ref_count(other.ref_count) {
    other.m_ptr = nullptr;
    other.ref_count = nullptr;
  }

  SharedPtr &operator=(SharedPtr &&other) noexcept {
    if (this != &other) {
      release();
      // move pointer and shared ref_count pointer
      m_ptr = other.m_ptr;
      m_deleter = std::move(other.m_deleter);
      ref_count = other.ref_count;
      // leave other in null state
      other.m_ptr = nullptr;
      other.ref_count = nullptr;
    }
    return *this;
  }

  T *get() const { return m_ptr; }

  // note: delete if ref_count is 0
  void release() {
    if (ref_count) {
      --(*ref_count);
      if (*ref_count == 0) {
        if (m_ptr) {
          m_deleter(m_ptr);
        }
        delete ref_count;
      }
    }
    m_ptr = nullptr;
    ref_count = nullptr;
  }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  T *m_ptr;
  DeleterType m_deleter;
  uint32_t *ref_count; // Pointer to shared reference count
};

#endif // SHARED_PTR_H