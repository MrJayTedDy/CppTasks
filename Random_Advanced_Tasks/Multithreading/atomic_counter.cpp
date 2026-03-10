#include <atomic>
#include <iostream>
#include <thread>

class SpinLockMutex {
 public:
  SpinLockMutex() : flag_{ATOMIC_FLAG_INIT} {}
  void lock() {
    while (flag_.test_and_set()) {
    }
  }

  void unlock() { flag_.clear(); }

  ~SpinLockMutex() { flag_.clear(); }

 private:
  std::atomic_flag flag_;
};

int counter = 0;
SpinLockMutex mtx;

void counter_increment() {
  for (int i = 0; i < 10000; ++i) {
    std::lock_guard lock(mtx);
    counter++;
  }
}

int main() {
  std::thread t1(counter_increment);
  std::thread t2(counter_increment);

  t1.join();
  t2.join();

  std::cout << "counter = " << counter << std::endl;
}