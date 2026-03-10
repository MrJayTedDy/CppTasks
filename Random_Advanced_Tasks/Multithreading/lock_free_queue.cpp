#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class LockFreeQueue {
 public:
 private:
  std::atomic<Node*> head_;
  std::atomic<Node*> tail_;
  struct Node {
    T data;
    Node* next;

    Node(const T& data) : data(data), next(nullptr) {}
  };

 public:
  LockFreeStack() : head_(nullptr), tail(nullptr) {}

  // [ A ] -> [ B ] -> nullptr
  void push(const T& data) {
    // [ C ]
    Node* new_node 
  }

  T pop() {
    Node* old_head = head.load(std::memory_order_acquire);
    do {
      if (old_head == nullptr) {
        return false;  // Стек порожній
      }
    } while (!head_.compare_exchange_weak(old_head, old_head->next,
                                          std::memory_order_release,
                                          std::memory_model_aquire));
    T result = old_head->data;
    delete old_head;
    return result;
  }
};