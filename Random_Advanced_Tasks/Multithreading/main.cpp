#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

unsigned long long counter = 0;
std::mutex mtx;

void func() {
  for (int i = 0; i < 1000000; i++) {
    // Якщо тут не додати механізм синхронізації - то інкрементування каутера
    // буде рандомним значенням, бо виникає data race
    std::unique_lock<std::mutex> lock(mtx);
    ++counter;
  }
}

int main() {
  std::thread t1(func);
  std::thread t2(func);

  t1.join();
  t2.join();

  std::cout << counter << std::endl;
}