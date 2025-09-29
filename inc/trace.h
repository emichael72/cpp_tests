
#ifndef TRACE_H
#define TRACE_H

#include <iostream>

struct Trace {
  const char *name;

  Trace(const char *n) : name(n) {
    std::cout << "\033[1;32m" // bright green
              << "Entering: "
              << "\033[0m" << name << "()" << std::endl;
  }

  ~Trace() {
    std::cout << std::endl
              << "\033[1;31m" // bright red
              << "Exiting: "
              << "\033[0m" << name << "()" << std::endl
              << std::endl;
  }
};

#endif // TRACE_H