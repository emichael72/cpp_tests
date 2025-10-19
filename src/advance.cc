
#ifdef BUILD_ADVANCE_TESTS

#include <iostream>


// primary template
template <typename Tuple, typename... Ts>
struct append;


// partial specialization for std::tuple<...>
template <typename... Us, typename... Ts>
struct append<std::tuple<Us...>, Ts...> {
    using type = std::tuple<Us..., Ts...>;
};


// aliasing to append_t
template <typename Tuple, typename... Ts>
using append_t = typename append<Tuple, Ts...>::type;


int main() {
  
   using L1 = std::tuple<int>;
   using L2 = append_t<L1, double, char>; 

  return 0;
}

#endif // BUILD_ADVANCE_TESTS