// Develop a class Counter that represents a simple integer counter.
// The class should satisfy the following require- ments:
//   (a) A constructor should be provided that takes a single int argument that
//   is used to initialize the counter value.
//       The argument should default to zero.
//   (b) The prefix increment and postfix increment operators should be
//   overloaded in order to provide a means by which to
//       increment the counter value.
//   (c) A member function getValue should be provided that returns the current
//   counter value.

// In addition, the class must track how many Counter objects are currently in
// existence. A means for querying this count should be provided. The code must
// not use any global variables. (Hint: Use static members.)

#include <iostream>

class Counter {
 public:
  Counter(int input_num = 0) : inner_counter_(input_num) { objects_counter++; }

  Counter(const Counter& other) : inner_counter_(other.inner_counter_) {
    ++objects_counter;
  }

  ~Counter() { --objects_counter; }

  Counter& operator=(const Counter& other) {
    inner_counter_ = other.inner_counter_;

    return *this;
  }

  Counter& operator++() {
    inner_counter_ += 1;
    return *this;
  }

  Counter operator++(int add_val) {
    static_cast<void>(add_val);
    Counter old = *this;
    ++(*this);
    return old;
  }

  int getValue() const { return inner_counter_; }
  static unsigned int getCurrentAmmountOfCounter() { return objects_counter; }

 private:
  int inner_counter_;
  static unsigned int objects_counter;
};

unsigned int Counter::objects_counter = 0;

void testCounter() {
  std::cout << "Start: " << Counter::getCurrentAmmountOfCounter()
            << std::endl;  // Should be 0
  {
    Counter c1(10);
    Counter c2 = c1;  // Copy constructor test
    std::cout << "Inside block: " << c1.getCurrentAmmountOfCounter()
              << std::endl;  // Should be 2

    Counter c3;
    c3 = c2;  // Copy assignment operator test
    std::cout << "After assignment: " << c3.getCurrentAmmountOfCounter()
              << std::endl;  // Should be 3
  }
  // All counter should be destryed after {} block
  std::cout << "End: " << Counter::getCurrentAmmountOfCounter()
            << std::endl;  // Should be 0
}

int main() {
  testCounter();
  return 0;
}