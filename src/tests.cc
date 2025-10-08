
#define _CRT_SECURE_NO_WARNINGS

#if __cplusplus >= 201703L

#include "add_concept.h"
#include "aloc.h"
#include "container.h"
#include "func_reg.h"
#include "iter.h"
#include "shared_ptr.h"
#include "sngl_ton.h"
#include "str.h"
#include "trace.h"
#include "unique_ptr.h"
#include "vect.h"
#include <bits/stdc++.h>
#include <iostream>
#include <type_traits>
#include <vector>

///////////////////////////////////////////////////
//
//  @brief
//    Special and Partial Template Specialization
//
///////////////////////////////////////////////////

template <typename T> T max_value(T a, T b) { return (a > b) ? a : b; }

// Specialization for const char*
template <> const char *max_value(const char *a, const char *b) {
  return std::strcmp(a, b) > 0 ? a : b;
}

// partial
template <typename T> class Box {
  T value;

public:
  Box(T v) : value(v) {}
  T get() const { return value; }
};

// Partial specialization for pointer types
template <typename T> class Box<T *> {
  T *ptr;

public:
  Box(T *p) : ptr(p) {}
  T &get() const { return *ptr; }
};

TEST_FUNC(special_templates) {

  // std::cout << max_value(3, 7) << "\n";        // generic
  // std::cout << max_value("hi", "bye") << "\n"; // specialized

  // partial
  Box<int> b1(42);               // stores an int
  Box<int *> b2(new int(7));     // stores pointer, dereferences on get()
  std::cout << b1.get() << "\n"; // Outputs: 42
  std::cout << b2.get() << "\n"; // Outputs: 7

  return 0;
}

///////////////////////////////////////////////////
//
// @brief
//   Smart Pointers
//
///////////////////////////////////////////////////

typedef void (*DeleterFunc)(int *);

struct dummy_object {
  char arr[34]; // dummy : just to see what happens when x isa not the first
  int x;
};

// note: C style function as deleter
void my_delete(int *p) {
  std::cout << "my_delete called\n";
  delete p;
}

TEST_FUNC(shared_ptr) {

  try {

    SharedPtr<dummy_object> p1(new dummy_object{.x = 42});
    std::cout << "p1->x value: " << p1->x << "\n";

    SharedPtr<dummy_object> p2(std::move(p1)); // move p1 to p2
    std::cout << "p2->x value: " << p2->x << "\n";

    // modify p2
    ++p2->x;
    std::cout << "After p2 increment, p2->x = " << p2->x << "\n";

    SharedPtr<dummy_object> p3(new dummy_object{.x = 100});
    std::cout << "p3->x value: " << p3->x << "\n";

    p2 = p1 = p3;
    std::cout << "After reassignment:\n";
    std::cout << "p2->x = " << p2->x << "\n";
    std::cout << "p3->x = " << p3->x << "\n";

    return p2.get()->x;
  } catch (...) {
    std::cerr << "unknown exception\n";
    return -1;
  }
}

TEST_FUNC(unique_ptr) {
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
    return -1;
  }
}

///////////////////////////////////////////////////
//
// @brief
//  Iterators and Containers
//
///////////////////////////////////////////////////

TEST_FUNC(iter) {

  int arr[3] = {10, 20, 30};

  SimpleIterator<int> begin(arr);
  SimpleIterator<int> end(arr + 3);

  for (auto it = begin; it != end; ++it) {
    std::cout << *it << " ";
  }
  std::cout << "\n";
  return 0;
}

TEST_FUNC(container) {

  MiniContainer cont;
  cont.add({"a", 85});
  cont.add({"b", 92});
  cont.add({"c", 78});
  cont.add({"d", 95});
  cont.add({"s", 88});
  cont.print();

  std::sort(cont.begin(), cont.end());

  std::cout << "\nsorted:\n";
  cont.print();

  return 0;
}

///////////////////////////////////////////////////
//
// @brief
//  String class
//
///////////////////////////////////////////////////

TEST_FUNC(str) {

  String s1("abcd");
  String s_extra("efgh");
  String s_all = s1 + s_extra;

  std::cout << "s_all: " << s_all << "\n";
  
  assert(s1.size() == 4);
  String s2;
  assert(s2.size() == 0);
  s2 = s1;
  assert(s1 == s2);
  s1[0] = '1';
  assert(s1 != s2);
  assert(s1 == "1bcd");
  String sub = s1.substr(1, 2); // take 2 symbols beginning from position 1
  
  assert(sub == "bc");
  
  String s3 = std::move(s1);
  
  assert(s3 != s1);
  assert(s1 == "");

  return 0;
}

///////////////////////////////////////////////////
//
// @brief
//  Vector class
//
///////////////////////////////////////////////////

TEST_FUNC(vec) {

  Vector<int> v1;

  v1.reserve(16);
  assert(v1.size() == 0);
  assert(v1.capacity() == 16);

  v1.push_back(10);
  int x = v1.pop_back();

  v1.push_back(10);
  v1.push_back(20);
  v1.push_back(30);
  assert(v1.size() == 3);

  assert(v1[0] == 10 && v1[1] == 20 && v1[2] == 30);

  Vector<int> v2 = v1; // copy
  assert(v2 == v1);

  v2[0] = 99;
  assert(v2 != v1);

  Vector<int> v3 = std::move(v1); // move
  assert(v3.size() == 3);
  assert(v1.size() == 0); // moved-from vector is empty but valid

  v3.push_back(42);
  assert(v3.size() == 4);
  for (int *it = v3.begin(); it != v3.end(); ++it) {
    std::cout << *it << " ";
  }

  return 0;
}

///////////////////////////////////////////////////
//
// @brief
//  Custom Allocator
//
///////////////////////////////////////////////////

TEST_FUNC(allocator) {

  BoundedAllocator<int> allocator(100); // Allow up to 100 bytes of allocation
  std::vector<int, BoundedAllocator<int>> v({8, 4, 5, 9}, allocator);

  v.push_back(6);
  v.push_back(9);

  v[2] = -1;

  // Print out the vector
  for (int n : v)
    std::cout << n << ' ';
  std::cout << '\n';
  return 0;
}

///////////////////////////////////////////////////
//
// @brief
//  References and Rvalue References
//
///////////////////////////////////////////////////

TEST_FUNC(values_and_refs) {
  /*
  int a = 10;
  int *p = &a;
  int &a_ref = a;
  int *&ref_p = p;
  */

  using ref_type = int &;

  // colupsing to int&
  ref_type &a_ref = *(new int(10));
  ref_type &&b_ref = a_ref;

  int &&x = 5; // something like:  int _tmp = 5 ; int &x = _tmp;
  x = 9;

  return 0;
}

///////////////////////////////////////////////////
//
// @brief
//  Function Templates
//
///////////////////////////////////////////////////

template <typename T> T add_things(T a, T b) { return (a + b); }

TEST_FUNC(func_template) {

  auto x = add_things(3, 4);
  return x;
}

///////////////////////////////////////////////////
//
// @brief
//  Materialized prvalue
//
///////////////////////////////////////////////////

class ArbitraryClass {
public:
  ArbitraryClass(int x, int y) { m_val = x + y; }

  ~ArbitraryClass() = default;
  int get_val() const { return m_val; }

private:
  int m_val = 0;
};

ArbitraryClass make_class() // <- Warning: black magic ahead
{
  return ArbitraryClass(1, 1); // retrunning prvalue.
  // C++17, the returned object is "materialized" directly in the caller's stack
  // frame
}

TEST_FUNC(materialized_prval) {
  // temporary object is created in the caller's stack frame
  return make_class().get_val();
}

TEST_FUNC(single) {

  auto &a = Singleton::instance();
  a.hello();

  auto &b = get_global_singleton();
  b.hello();

  Singleton *p = Singleton::create_on_heap();
  p->hello();

  return 0;
}

TEST_FUNC(concepts) {

  auto x = add_test(3, 4);
  return 0;
}

enum class mss_index_t : uint32_t { _0, _1, _2, _3 };

inline mss_index_t &operator++(mss_index_t &val) {
  using int_type = std::underlying_type_t<mss_index_t>;
  return val = mss_index_t(int_type(val) + 1);
}


TEST_FUNC(enumerator) {

  mss_index_t idx = mss_index_t::_0;
  mss_index_t i =  mss_index_t::_0;

  i = static_cast<mss_index_t>(static_cast<int>(i) + 1); 

  ++idx;
  std::cout << static_cast<uint32_t>(idx) << "\n"; // prints 1

  return 0;

}


#endif