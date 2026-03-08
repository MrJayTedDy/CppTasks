// Consider a program that consists of the single source-code file whose listing is given below.
// The code shown in this listing below will fail to compile, due to a type-deduction failure.
// (a) Identify which type cannot be deduced, and explain why this type cannot be deduced.
// (b) Explain how the code can be modified to resolve this problem, without changing the definition of func.
//
// template <class T, class U>
// void func(const U& u)
// {
//   T x;
//   // ...
// }
//
// int main()
// {
//   int x;
//   func(x); // ERROR: type-deduction failure
// }

template <class T, class U>
void func(const U& u) {
  T x;
  // ...
}

int main() {
  int x;
  //  Deduction can't be done because compiler can't resolve the value the T has to be. So we have to add explicit
  //  deduction
  func<int>(x);
}