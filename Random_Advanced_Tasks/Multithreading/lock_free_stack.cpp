#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class LockFreeStack {
 public:
  LockFreeStack() : head(nullptr) {}
  // [ C ] ... [ A ] -> [ B ] -> nullptr
  void push(const T& data) {
    Node* new_node = new Node(data);

    new_node->next = head.load(std::memory_order_relaxed);

    while (!head.compare_exchange_weak(new_node->next, new_node,
                                       std::memory_order_release,
                                       std::memory_order_relaxed));
  }
  // [ C ] -> [ A ] -> [ B ] -> nullptr
  bool pop(const T& data) {
    Node* old_head = head.load(std::memory_order_relaxed);  // C

    do {
      if (old_head == nullptr) {
        return false;  // Стек порожній
      }
    } while (!head.compare_exchange_weak(old_head, old_head->next,
                                         std::memory_order_release,
                                         std::memory_order_relaxed));

    auto result = old_head->data_;
    delete old_head;
    return true;
  }

 private:
  struct Node {
    T data_;
    Node* next;

    Node(const T& data) : next(nullptr), data_(data) {}
  };

  std::atomic<Node*> head;
};

LockFreeStack<int> stack;

void pusher() {
  for (int i = 0; i < 10000; ++i) {
    stack.push(i);
  }
}

void popper() {
  int value;
  for (int i = 0; i < 10000; ++i) {
    while (!stack.pop(value));  // Активно витягуємо
  }
}

int main() {
  std::thread t1(pusher);
  std::thread t2(popper);
  std::thread t3(pusher);
  std::thread t4(popper);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  // Перевіримо, чи стек порожній в кінці
  int value;
  if (stack.pop(value)) {
    std::cout << "Stack is not empty, something is wrong.\n";
  } else {
    std::cout << "Stack is empty, everything worked correctly.\n";
  }

  return 0;
}