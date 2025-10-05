#pragma once
#include <iostream>
class Singleton {
public:

  // a reference to the single instance.
  static Singleton &instance() {
    static Singleton inst;
    return inst;
  }

  // when weexpect a pointer. still returns the *same* object
  // every time, not a new one.
  static Singleton *create_on_heap() { return &instance(); }

  // compatibil with a "global singleton" , internally
  // just redirects to the same single instance.
  friend Singleton &get_global_singleton();

  void hello() { std::cout << "Singleton @" << this << "\n"; }

  static void destroy_instance() {
    // ?? when will this be processed on binary termination?
  }

private:
  Singleton() :m_temp(42) { std::cout << "Singleton constructed\n"; }

  ~Singleton() { std::cout << "Singleton destroyed\n"; }

  // prevent copy and move >> strictly one object.
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton &operator=(Singleton &&) = delete;

  const int m_temp;
};

// redirects to singleton instance.
Singleton &get_global_singleton() { return Singleton::instance(); }
