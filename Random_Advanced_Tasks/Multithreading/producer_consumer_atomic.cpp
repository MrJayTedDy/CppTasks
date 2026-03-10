#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

class SpinLockMutex {
 public:
  SpinLockMutex() { flag_.clear(); }
  void lock() {
    while (flag_.test_and_set()) {
      std::this_thread::yield();
    }
  }
  void unlock() { flag_.clear(); }

 private:
  std::atomic_flag flag_;
};

SpinLockMutex mtx;

void producer(std::queue<int>& q) {
  for (int i = 0; i < 10000; ++i) {
    int val = rand() % 100;
    std::lock_guard lock(mtx);
    q.push(val);
    std::cout << "thread = " << std::this_thread::get_id()
              << "\t Provided: " << val << std::endl;
  }
}

void consumer(std::queue<int>& q) {
  int consumed = 0;
  while (consumed < 10000) {
    std::lock_guard lock(mtx);
    if (!q.empty()) {
      int val = q.front();
      q.pop();
      consumed++;
      std::cout << "thread = " << std::this_thread::get_id()
                << "\tIteration: " << consumed << "\t Consumed: " << val
                << std::endl;
    }
  }
}

int main() {
  std::queue<int> dataQueue;
  std::thread producer_thread(producer, std::ref(dataQueue));
  std::thread consumer_thread(consumer, std::ref(dataQueue));
  producer_thread.join();
  consumer_thread.join();
}