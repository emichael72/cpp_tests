
#ifdef BUILD_TEMPLATES_TESTS

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

#endif // BUILD_TEMPLATES_TESTS