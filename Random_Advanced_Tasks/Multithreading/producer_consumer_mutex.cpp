#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

std::mutex mtx;

void producer(std::queue<int>& q) {
  for (size_t i{0}; i < 10000; ++i) {
    int val{rand() % 100};
    std::lock_guard<std::mutex> lock(mtx);
    q.push(val);
    std::cout << "thread = " << std::this_thread::get_id() << "\t provided "
              << val << std::endl;
  }
}

void consumer(std::queue<int>& q) {
  for (size_t i{0}; i < 10000; ++i) {
    if (!q.empty()) {
      int val{q.front()};
      std::lock_guard<std::mutex> lock(mtx);
      q.pop();
      std::cout << "thread = " << std::this_thread::get_id() << "\t consumed "
                << val << std::endl;
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