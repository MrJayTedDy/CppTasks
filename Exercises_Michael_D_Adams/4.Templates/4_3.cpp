// (a) Develop a template function sum that computes the sum of zero or more
//     elements (of the same type) that are stored contiguously in memory. The
//     template should have a single parameter that is the type of the elements
//     to be processed by the function. The function has two parameters: 1) a
//     pointer to the first element in the range to be summed; and 2) a pointer
//     to one-past-the-last element in the range to be summed.
// (b) Write a program to test the sum function.
// (c) Also, test the sum function with the provided code below.

#include <concepts>
#include <iostream>
// Insert your code here.
template <typename T>
  requires std::integral<T> || std::floating_point<T>
constexpr T sum(const T* first, const T* last) {
  T result = T();
  while (first != last) {
    result += *first++;
  }
  return result;
}

int main() {
  const int i[3] = {1, 2, 3};
  const double d[3] = {1.0, 2.0, 3.0};

  std::cout << sum(&i[0], &i[3]) << "\n";
  std::cout << sum(&d[0], &d[3]) << "\n";

  return 0;
}