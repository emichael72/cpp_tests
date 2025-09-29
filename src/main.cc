#include "func_reg.h"
#include <iostream>

///////////////////////////////////////////////////
//
// @brief
//  C++ Play ground.
//
///////////////////////////////////////////////////

// Test runner
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

void print_help(const char *progname) {
  std::cout << "Usage: " << progname << " [OPTIONS]\n"
            << "Options:\n"
            << "  -a, --all            Run all registered tests\n"
            << "  -t, --test <name>    Run a specific test by name\n"
            << "  -h, --help           Show this help message\n"
            << "\nAvailable tests:\n";
  for (auto &[name, _] : FunctionRegistry::map()) {
    std::cout << "  " << name << "\n";
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_help(argv[0]);
    return 0;
  }

  std::string arg1 = argv[1];
  if (arg1 == "-a" || arg1 == "--all") {
    run_all_tests();
  } else if (arg1 == "-t" || arg1 == "--test") {
    if (argc < 3) {
      std::cerr << "Error: missing test name\n";
      return 1;
    }
    run_single_test(argv[2]);
  } else if (arg1 == "-h" || arg1 == "--help") {
    print_help(argv[0]);
  } else {
    std::cerr << "Unknown option: " << arg1 << "\n\n";
    print_help(argv[0]);
    return 1;
  }

  return 0;
}