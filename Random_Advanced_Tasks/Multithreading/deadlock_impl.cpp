#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct Account {
  int balance;
  std::mutex mtx;
};

void transfer(Account& from, Account& to, int amount) {
  std::scoped_lock lock(from.mtx, to.mtx);
  if (from.balance < amount) {
    std::cout << "Not enough balance" << std::endl;
    return;
  }

  from.balance -= amount;
  to.balance += amount;
}

int main() {
  Account account1{1000};
  Account account2{1000};

  std::thread t1(transfer, std::ref(account1), std::ref(account2), 455);
  std::thread t2(transfer, std::ref(account2), std::ref(account1), 645);
  t1.join();
  t2.join();
  std::cout << "Account 1 balance:" << account1.balance << std::endl;
  std::cout << "Account 2 balance:" << account2.balance << std::endl;
}