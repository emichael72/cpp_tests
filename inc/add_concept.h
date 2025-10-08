#pragma once
#include <concepts>

/*
 * ---------------------------------------------------------------------------
 *  C++20 Concepts — Overview
 * ---------------------------------------------------------------------------
 *
 *  Concepts are compile-time predicates (boolean conditions) that constrain
 *  template parameters. They let you describe *what kind of types* a template
 *  can accept, improving both code clarity and compiler diagnostics.
 *
 *  Before C++20, invalid template instantiations would often produce long,
 *  cryptic compiler errors. Concepts allow you to express constraints directly,
 *  so that a type that does not meet the requirements is rejected with a
 *  readable, early diagnostic.
 *
 *  Basic syntax forms:
 *
 *      template <typename T>
 *      concept ConceptName = // compile-time rule ;
 *
 *      template <ConceptName T>
 *      void function(T value);     // short form
 *
 *      template <typename T>
 *      requires ConceptName<T>
 *      void function(T value);     // long form (same effect)
 *
 *  Concepts can use the 'requires' expression to specify what operations
 *  must be valid for a type. If any of those expressions fail to compile,
 *  the concept is not satisfied.
 *
 * ---------------------------------------------------------------------------
 *  Example: Addable Concept
 * ---------------------------------------------------------------------------
 *
 *  The following concept checks that a type T supports the '+' operator
 *  between two values of type T. Any type for which `a + b` is a valid
 *  expression will satisfy this concept (int, double, std::string, etc.).
 *
 *  If a type does not provide '+', the compiler will produce a clear error:
 *
 *      error: type 'Foo' does not satisfy 'Addable'
 *
 * ---------------------------------------------------------------------------
 */

// Define a concept named 'Addable'
// It requires that 'a + b' is a valid expression for two values of type T.
template <typename T>
concept Addable = requires(T a, T b) {
    a + b; // expression must compile successfully
};


// Define a function template constrained by the Addable concept.
// Only types that satisfy Addable<T> can be used here.
template <typename T>
requires Addable<T>
T add_test(T a, T b) {
    return a + b;
}

/*
 * Example usage:
 *
 *   std::cout << add_test(1, 2);                 // OK (int)
 *   std::cout << add_test(1.5, 2.7);             // OK (double)
 *   std::cout << add_test(std::string("a"), "b"); // OK (string)
 *
 *   struct Foo {};
 *   Foo f1, f2;
 *   add_test(f1, f2);  // Compile-time error: 'Foo' does not satisfy Addable
 *
 * ---------------------------------------------------------------------------
 */
