#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

// Завдання: Реалізуйте асинхронне обчислення факторіалу
/**
 * Реалізуйте функцію async_factorial - Done
 * Додайте обробку помилок (наприклад, для від'ємних чисел) - Done
 * Виміряйте час виконання - Done
 * Додайте timeout для future.wait_for() - Done
 */

std::mutex mtx;

long long factorial(short n) {
  long long result = 1;
  for (short i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

void async_factorial(std::promise<long long> prom, short n) {
  auto start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(std::chrono::seconds(3));

  try {
    if (n < 0) {
      throw std::invalid_argument(
          "Factorial doesn't work for negative numbers");
    }
    if (n > 20) {
      throw std::overflow_error("Number too large for long long");
    }
    prom.set_value(factorial(n));
  } catch (...) {
    prom.set_exception(std::current_exception());
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  {
    std::lock_guard lock(mtx);
    std::cout << "Duration time: " << duration << std::endl;
  }
}

int main() {
  std::promise<long long> prom;
  std::future<long long> fut = prom.get_future();

  short n = 20;
  {
    std::lock_guard lock(mtx);
    std::cout << "Computing factorial of " << n << "..." << std::endl;
  }
  std::thread t(async_factorial, std::move(prom), n);

  auto status = fut.wait_for(std::chrono::milliseconds(3100));
  if (status == std::future_status::ready) {
    try {
      long long result = fut.get();
      std::lock_guard lock(mtx);
      std::cout << "Factorial: " << result << std::endl;
    } catch (const std::invalid_argument& e) {
      std::lock_guard lock(mtx);
      std::cout << "Error: " << e.what() << std::endl;
    } catch (const std::overflow_error& e) {
      std::lock_guard<std::mutex> lock(mtx);
      std::cout << "Overflow error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
      std::lock_guard<std::mutex> lock(mtx);
      std::cout << "Unexpected error: " << e.what() << std::endl;
    }
  } else {
    std::lock_guard lock(mtx);
    std::cout << "Timeout! The task still completes" << std::endl;
  }

  t.join();
  return 0;
}