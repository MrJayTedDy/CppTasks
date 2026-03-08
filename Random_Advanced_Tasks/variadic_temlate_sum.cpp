// (a) Develop a variadic template function named sum_all that accepts zero or
//     more arguments. These arguments may be of different types (e.g., a mix of
//     int, double, and float simultaneously).
// (b) The function should return the total sum of all provided arguments. Use
//     Fold Expressions (introduced in C++17) to implement the summation logic.
// (c) Implement a Concept to constrain the template parameters. The concept
//     must ensure that each argument type T satisfies the condition
//     std::numeric_limits<T>::is_specialized. For variadic templates, this
//     should be checked using a fold-like syntax, such as (std::integral<Args>
//     && ...) or equivalent logic for your specific requirements.
// (d) Write a test program to demonstrate the function. For example, a call to
//     sum_all(1, 2.5, 3, 0.5) should return 7.0.

#include <concepts>
#include <iostream>

// Insert your code here.
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric... Args>
auto sum_all(Args... args) {
  return (args + ... + 0);
}

int main() {
  // Test with mixed types
  std::cout << sum_all(1, 2.5, 3, 0.5) << "\n";  // Expected: 7.0
  std::cout << sum_all(10, 20, 30) << "\n";      // Expected: 60
  std::cout << sum_all(1.5, 2.5, 3.0) << "\n";   // Expected: 7.0
  std::cout << sum_all(42) << "\n";              // Expected: 42
  std::cout << sum_all() << "\n";                // Expected: 0

  return 0;
}