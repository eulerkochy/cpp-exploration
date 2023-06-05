// https://www.slamecka.cz/posts/2021-03-17-cpp-metaprogramming-exercises-1/

#include <iostream>
#include <type_traits>

template <int ... T>
struct Vector {};


template <int Head, int ... Tail> 
constexpr void printImpl(const Vector<Head, Tail...>& v) {
  std::cout << Head << " ";
  if constexpr (sizeof...(Tail) != 0) {
    printImpl(Vector<Tail...>{});
  } 
}

template <int ... T>
constexpr void print(const Vector<T...>& v) {
  if constexpr (sizeof...(T) != 0) {
    printImpl(Vector<T...>{});
  }
  std::cout << std::endl;
}

template <int Elem, typename Type>
struct Prepend {
};

template <int Elem, int ... T>
struct Prepend<Elem, Vector<T...>> {
  using type = Vector<Elem, T...>;
};

template <int Elem, typename T>
using PrependT = typename Prepend<Elem, T>::type;

template <int Elem, typename Type>
struct Append {
};

template <int Elem, int ... T>
struct Append<Elem, Vector<T...>> {
  using type = Vector<T..., Elem>;
};

template <typename Type>
struct PopBack {
};

template <int ... X, int ... Y>
constexpr auto combine(Vector<X...> v1, Vector<Y...> v2) { return Vector<X..., Y...>{}; }

template <int H>
constexpr auto popBackImpl(Vector<H> v) { return Vector<>{}; }

template <int H, int ... T>
constexpr auto popBackImpl(Vector<H, T...> v) { 
  return combine(Vector<H>{}, popBackImpl(Vector<T...>{})); 
}

template <int H, int ... T>
struct PopBack<Vector<H, T...>> {
  using type = decltype(popBackImpl(Vector<H, T...>{}));
};

template <int Elem, typename Container>
struct RemoveFirst {
};

int main() {
  static_assert(std::is_same_v<Vector<1,2>, Vector<1,2>>);
  print(Vector<1>{});
  print(Vector<1, 2, 3, 4, 5>{});
  // print(Vector<>{});
  static_assert(std::is_same_v<Prepend<1, Vector<2,3>>::type, Vector<1,2,3>>);
  static_assert(std::is_same_v<PrependT<1, Vector<2,3>>, Vector<1,2,3>>);
  
  static_assert(std::is_same_v< Append<4, Vector<1,2,3>>::type , Vector<1,2,3,4> >);

  static_assert(std::is_same_v< PopBack<Vector<1,2,3,4>>::type , Vector<1,2,3> >);
  return 0;
}