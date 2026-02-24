// (a) Define a class Exponent to hold a real exponent for an exponentiation
//     operation. Through clever use of operator overloading, find a way to have
//     the expression x ** y call std::pow(x, y), where the types of x and y are
//     double and Exponent, respectively. In other words, the program should
//     output the value 16 (i.e., 2^4 = 16).
//
// (b) Is the preceding use of operator overloading wise? Explain.

#include <cmath>
#include <iostream>

// Insert your code here.

int main() {
  const double x = 2.0;
  const Exponent y = 4.0;

  std::cout << x * *y << "\n";

  return 0;
}