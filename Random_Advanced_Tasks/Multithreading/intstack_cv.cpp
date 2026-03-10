#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Global mutex for protecting std::cout across threads
static inline std::mutex cout_mtx;

class IntStack {
 public:
  IntStack() = default;
  IntStack(const IntStack&) = delete;
  IntStack& operator=(const IntStack&) = delete;

  void push(int x) {
    std::scoped_lock lock(m_);
    v_.push_back(x);
    size_t sz = v_.size();
    {
      std::scoped_lock cout_lock(cout_mtx);
      std::cout << "[" << std::this_thread::get_id() << "] push(" << x
                << ") size=" << sz << "\n";
    }
    c_.notify_one();
  }

  int pop() {
    std::unique_lock<std::mutex> lock(m_);
    c_.wait(lock, [this] { return !v_.empty(); });
    int x = v_.back();
    v_.pop_back();
    size_t sz = v_.size();
    {
      std::scoped_lock cout_lock(cout_mtx);
      std::cout << "[" << std::this_thread::get_id() << "] pop() -> " << x
                << " size=" << sz << "\n";
    }
    return x;
  }

 private:
  std::vector<int> v_;
  mutable std::mutex m_;
  std::condition_variable c_;
};

int main() {
  constexpr int numIters = 1000;
  IntStack s;

  // Producer: додає непарні числа 1,3,5,...
  std::jthread producer([&] {
    for (int i = 0; i < numIters; ++i) {
      s.push(2 * i + 1);
    }
  });

  // Consumer: читає і виводить numIters значень
  std::jthread consumer([&] {
    for (int i = 0; i < numIters; ++i) {
      int v = s.pop();
      {
        std::scoped_lock cout_lock(cout_mtx);
        std::cout << "[" << std::this_thread::get_id() << "] consumed main -> "
                  << v << "\n";
      }
    }
  });

  return 0;
}