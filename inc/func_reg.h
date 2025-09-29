
#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

struct FunctionRegistry {
  using FuncType = int (*)();
  using MapType = std::unordered_map<std::string, FuncType>;

  static MapType &map() {
    static MapType m;
    return m;
  }

  static void register_func(const char *name, FuncType f) { map()[name] = f; }

  static int call(const std::string &name) {
    if (auto it = map().find(name); it != map().end())
      return it->second(); // call function through pointer
    std::cerr << "Function " << name << " not found!\n";
    return -1;
  }

  // Print all registered functions
  static void list() {
    std::cout << "Registered functions:\n";
    for (const auto &[name, _] : map()) {
      std::cout << "  " << name << "\n";
    }
  }

  // Iterators (so you can use range-based for, etc.)
  static auto begin() { return map().begin(); }
  static auto end() { return map().end(); }
  static auto cbegin() { return map().cbegin(); }
  static auto cend() { return map().cend(); }
};

// Define and register an int(void) function
#define TEST_FUNC(name)                                                        \
  int name();                                                                  \
  namespace {                                                                  \
  struct FnRegister_##name {                                                   \
    FnRegister_##name() { FunctionRegistry::register_func(#name, name); }      \
  } FnRegister_##name##_instance;                                              \
  }                                                                            \
  int name()
