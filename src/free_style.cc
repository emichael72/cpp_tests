

#include "buff_mngr.h"
#include <iostream>
#include <utility>

class TestClass {

public:
  TestClass(char val) : m_val(val){};
  ~TestClass(){};

  int show() {
    std::cout << m_val << std::endl;
    return 0;
  }

  // copy constructor
  TestClass(const TestClass &other) : m_val(other.m_val) {
    std::cout << "In copy constructor..\n";
  }

  // copy assignment
  TestClass &operator=(const TestClass &other) {
    std::cout << "In copy assignment..\n";
    if (this != &other)
      m_val = other.m_val;
    return *this;
  }

  // handle move
  TestClass(TestClass &&other) : m_val(other.m_val) {
    other.m_val = 'I'; // invalid marker
    std::cout << "In move constructor..\n";
  }

  TestClass &operator=(TestClass &&other) {
    std::cout << "In move assignment..\n";
    if (this != &other) {
      m_val = other.m_val;
      other.m_val = 'I'; // invalid marker
    }
    return *this;
  }

  // functor
  int operator()() { return show(); }

private:
  char m_val;
};

int buff_tester() {
  int ret_val = 1;

  Buffer<int> *bufP = new Buffer<int>(10);
  Buffer<int> &bufR = *bufP;

  // Access through reference
  for (size_t i = 0; i < bufR.size(); ++i)
    bufR[i] = static_cast<int>(i * 10);

  Buffer<int> bufA = bufR; // Copy constructor 
  Buffer<int> bufB =
      std::move(bufR); // Move constructor (P & R state becomes undefined)

  Buffer<char> bufC(3);
  bufC[0] = 'A';
  bufC[1] = 'B';
  bufC[2] = 'C';
  Buffer<char> bufD = bufC; // Copy constructor (for chars)

  delete bufP; // clean up heap allocation
  return ret_val;
}

int class_tester() {

  int ret_val = 1;

  TestClass tst_class_a('a'); // direct
  TestClass tst_class_b = TestClass(
      'b'); // copy constructor not processed due to elision optimization

  tst_class_a = tst_class_b; // copy assignment
  ret_val = tst_class_b();   // invocation through () overload
  TestClass tst_class_c('c');

  tst_class_c = std::move(tst_class_b); // move assignment

  tst_class_b();

  return ret_val;
}
