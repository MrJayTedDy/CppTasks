#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

std::condition_variable cv;
std::mutex mtx;

void producer(std::queue<int>& q) {
  for (int i{0}; i < 1000; ++i) {
    int val = rand() % 100;
    {
      std::unique_lock<std::mutex> lock(mtx);
      q.push(val);
    }
    cv.notify_one();
    std::cout << "thread = " << std::this_thread::get_id()
              << "\t Provided: " << val << std::endl;
  }
}

void consumer(std::queue<int>& q) {
  for (int i{0}; i < 1000; ++i) {
    std::unique_lock<std::mutex> ulck(mtx);
    cv.wait(ulck, [&q]() { return !q.empty(); });
    int val = q.front();
    q.pop();
    ulck.unlock();
    std::cout << "thread = " << std::this_thread::get_id()
              << "\t Consumed: " << val << std::endl;
  }
}

int main() {
  std::queue<int> dataQueue;
  std::thread producer_thread(producer, std::ref(dataQueue));
  std::thread consumer_thread(consumer, std::ref(dataQueue));
  producer_thread.join();
  consumer_thread.join();
}