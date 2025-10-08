#ifndef STR_H
#define STR_H

#include <string.h>

class String {
public:
  String(const char *str = NULL) {
    if (str) {
      m_length = strlen(str);
      m_data = new char[m_length + 1];
      strcpy(m_data, str);
    }
  }

  ~String() {
    if (m_data)
      delete[] m_data;
  }

  // return size
  size_t size() const { return m_length; }
  const char *c_str() const { return m_data; }

  // get ref to a byte at an index (const / non const)
  char &operator[](size_t index) {
    if (!m_data || index >= m_length)
      throw std::out_of_range("out of range");
    return m_data[index];
  }

  // todo: cpp compiler can indeed catch const data violation :
  // - point to a const something,
  // - trash something using pointer access
  // - did we get any exception?

  const char &operator[](size_t index) const {
    if (!m_data || index >= m_length)
      throw std::out_of_range("out of range");

    return m_data[index];
  }

  // copy
  String &operator=(const String &other) {
    if (this != &other) {
      delete[] m_data; // free existing
      m_length = other.m_length;
      if (m_length > 0) {
        m_data = new char[m_length + 1];
        strcpy(m_data, other.m_data);
      } else {
        m_data = nullptr; // handle empty
      }
    }
    return *this;
  }

  // move
  String(String &&other) noexcept
      : m_data(other.m_data), m_length(other.m_length) {
    // pointers flipping only
    other.m_data = nullptr;
    other.m_length = 0;
  }
  String &operator=(String &&other) noexcept {
    // full move  (fre the old, point to the new, return the new.
    if (this != &other) {
      delete[] m_data;
      m_data = other.m_data;
      m_length = other.m_length;

      other.m_data = nullptr;
      other.m_length = 0;
    }

    return *this;
  }

  String operator+(const String &other) noexcept {

    size_t new_length = m_length + other.m_length;
    String new_str;

    if (new_length == 0) {
      // both empty or other is empty
      return new_str;
    }

    char *new_buffer = new char[new_length + 1];
    if (!new_buffer) {
      // allocation failed, keep old data
      return new_str;
    }

    new_buffer[new_length] = '\0';

    if (m_data) {
      memcpy(new_buffer, m_data, m_length);
    }

    if (other.m_data) {
      memcpy(new_buffer + m_length, other.m_data, other.m_length);
    }

    new_str.m_data = new_buffer;
    new_str.m_length = new_length;
    return new_str;
  }

  String substr(size_t pos, size_t len) const {
    if (pos >= m_length)
      return String();

    if (pos + len > m_length)
      len = m_length - pos; // adjust length

    char *buffer = new char[len + 1];
    strncpy(buffer, m_data + pos, len);
    buffer[len] = '\0';
    String result(buffer); // this will recalculate length for result
    delete[] buffer;
    return result;
  }

  bool operator==(const String &other) const { return are_equal(other); }
  bool operator!=(const String &other) const { return !(*this == other); }


private:
  char *m_data = NULL;
  size_t m_length = 0;

  bool are_equal(const String &other) const {
    if (m_length != other.m_length)
      return false;
    if (m_length == 0)
      return true; // local and other are empty

    return strcmp(m_data, other.m_data) == 0;
  }
};

// ?!

// Implementation of the friend function
inline std::ostream &operator<<(std::ostream &os, const String &str) {
  os << str.c_str(); 
  return os;
}

#endif // STR_H