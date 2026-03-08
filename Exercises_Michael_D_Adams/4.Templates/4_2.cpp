// Develop a template function min3 that takes three arguments of the same type and returns the least of these
// arguments. For example, min3(1, 0, 2) would return 0 and min3(1.5, 0.5, 3.0) would return 0.5.
#include <concepts>
#include <iostream>
template <typename T>
  requires std::integral<T> || std::floating_point<T>
constexpr T min3(const T first, const T second, const T third) {
  if ((first < second) && (first < third)) {
    return first;
  } else {
    return second < third ? second : third;
  }
}

int main() {
  std::cout << min3(1, 0, 2) << std::endl;
  std::cout << min3(1.5, 0.5, 3.0) << std::endl;
}