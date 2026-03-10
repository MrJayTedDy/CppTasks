#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct Account {
  int balance;
  std::mutex mtx;
};

void transfer(Account& from, Account& to, int amount) {
  std::lock_guard<std::mutex> lock1(from.mtx);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::lock_guard<std::mutex> lock2(to.mtx);
  if (from.balance < amount) {
    std::cout << "Not enough balance" << std::endl;
    return;
  }

  from.balance -= amount;
  to.balance += amount;
  std::cout << "From balance:" << from.balance << std::endl;
  std::cout << "To balance:" << to.balance << std::endl;
}

int main() {
  Account account1{1000};
  Account account2{1000};

  std::thread t1(transfer, std::ref(account1), std::ref(account2), 455);
  std::thread t2(transfer, std::ref(account2), std::ref(account1), 645);
  t1.join();
  t2.join();
}