
#if 0
#include "free_style.h"
#include <iostream>

///////////////////////////////////////////////////
//
// @brief
//  C++ Play ground.
//
///////////////////////////////////////////////////

// Test runner
#if __cplusplus >= 201703L
#include "func_reg.h"

void run_all_tests() {

  for (auto &[name, func] : FunctionRegistry::map()) {
    std::cout << "\033[1;36mRunning test '" << name << "()' ...\033[0m\n";
    int result = -1;
    try {
      result = func();
      if (result >= 0) {
        std::cout << "  \033[1;32mTest '" << name
                  << "()' completed successfully, return = " << result
                  << "\033[0m\n\n";
      } else {
        std::cout << "  \033[1;31mTest '" << name
                  << "()' failed, return = " << result << "\033[0m\n\n";
      }
    } catch (const std::exception &e) {
      std::cout << "  \033[1;31mTest '" << name
                << "()' threw exception: " << e.what() << "\033[0m\n\n";
    } catch (...) {
      std::cout << "  \033[1;31mTest '" << name
                << "()' threw unknown exception\033[0m\n\n";
    }
  }

  std::cout << "\033[1;36mAll Tests Completed.\033[0m\n";
}

void run_single_test(const std::string &name) {
  auto it = FunctionRegistry::map().find(name);
  if (it == FunctionRegistry::map().end()) {
    std::cerr << "\033[1;31mTest '" << name << "' not found\033[0m\n";
    return;
  }

  std::cout << "\033[1;36mRunning test '" << name << "' ...\033[0m\n";
  try {
    int result = it->second();
    if (result >= 0) {
      std::cout << "  \033[1;32mTest '" << name
                << "' completed successfully, return = " << result
                << "\033[0m\n";
    } else {
      std::cout << "  \033[1;31mTest '" << name
                << "' failed, return = " << result << "\033[0m\n";
    }
  } catch (const std::exception &e) {
    std::cout << "  \033[1;31mTest '" << name
              << "' threw exception: " << e.what() << "\033[0m\n";
  } catch (...) {
    std::cout << "  \033[1;31mTest '" << name
              << "' threw unknown exception\033[0m\n";
  }
}
#endif

void print_help(const char *progname) {
  std::cout << "Usage: " << progname << " [OPTIONS]\n"
            << "Options:\n"
#if __cplusplus >= 201703L
            << "  -a, --all            Run all registered tests\n"
            << "  -t, --test <name>    Run a specific test by name\n"
#endif
            << "  -f, --free_style     Run 'free style' training code\n"
            << "  -b, --buff           Run buff management tester\n"
            << "  -h, --help           Show this help message\n"
            << "\nAvailable tests:\n";
#if __cplusplus >= 201703L
  for (auto &[name, _] : FunctionRegistry::map()) {
    std::cout << "  " << name << "\n";
  }
#endif
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_help(argv[0]);
    return 0;
  }

  std::string arg1 = argv[1];

#if __cplusplus >= 201703L
  if (arg1 == "-a" || arg1 == "--all") {
    run_all_tests();
  } else if (arg1 == "-t" || arg1 == "--test") {
    if (argc < 3) {
      std::cerr << "Error: missing test name\n";
      return 1;
    }
    run_single_test(argv[2]);

  } else if (arg1 == "-f" || arg1 == "--free_style") {
    return class_tester();
#else
  if (arg1 == "-f" || arg1 == "--free_style") {
    return class_tester();
#endif
  } else if (arg1 == "-b" || arg1 == "--buff") {
    return buff_tester();
  } else if (arg1 == "-h" || arg1 == "--help") {
    print_help(argv[0]);
  } else {
    std::cerr << "Unknown option: " << arg1 << "\n\n";
    print_help(argv[0]);
    return 1;
  }

  return 0;
}

#else

/*
// #define add_pointer_c(T, NAME) typedef T* NAME
// add_pointer_c(int, p_1); // same as int* p_1;


template <class T>
struct add_pointer
{
    // produce apointer type to T
    using type = T*; // define type as T*

};

// template that takes a template with single argument and a parameter
template<template<class> class UnaryMeta, class Param>
struct apply_meta
{
    using type = typename UnaryMeta<Param>::type;
};

int main() {
    
    int i = 5;    
    add_pointer<int>::type p = &i;  // same as int* p = &i;
    apply_meta<add_pointer, int>::type p1 = &i; // same as int* p1 = &i; 
    
    return 0;
}
*/



#include <type_traits>
#include <iostream>
#include <string>

template <class T>
struct add_const {
    using type = const T;
};

template <class T>
struct add_pointer {
    using type = T*;
};

template <class T>
struct make_const_pointer {
    using type = typename add_pointer<typename add_const<T>::type>::type;
};

template<template<class> class UnaryMeta, class Param>
struct apply_meta {
    using type = typename UnaryMeta<Param>::type;
};

/////////////////////////////////////////////////////////////////
//
// remove_const qualifier
//
/////////////////////////////////////////////////////////////////

template <class T>
struct remove_const {
    using type = T;   // default: unchanged
};

// specialization for const-qualified types
template <class T>
struct remove_const<const T> {
    using type = T;
};

template <class T>
struct remove_const_eager {
    using type = typename T::type;  // tries to look into T now
};

// ------------------ helper types ------------------
struct NoInner {};   // has no inner_type
struct HasInner { using inner_type = const int; };

//- can treat types uniformly,
// regardless of whether they were declared const.

template <typename T>
void process(T& val) {
    using base_type = typename remove_const<T>::type;
    base_type temp = val;   // copies val even if T was const
    temp = 123;           
}

struct Wrapper {
    using type = const int;  // satisfies the requirement
};

int main() {


    remove_const<NoInner> ok;   // lazy : does not look inside NoInner for type
    
    using int_lazy = typename remove_const<int>::type; // ok
    using int_eager_a = typename remove_const_eager<Wrapper>::type; // ok 
    // using int_eager_b = typename remove_const_eager<int>::type; // bad 
    
    int_lazy x = 200;


    const int dummy = 150;
    process(dummy); 

    return 0;

}

#endif