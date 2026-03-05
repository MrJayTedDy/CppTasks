#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class MyVector {
 public:
  // Default Constructor
  MyVector() = default;

  // Constructor
  MyVector(size_t size, const T& data = T()) : size_(size), capacity_(size) {
    data_ = new T[size];
    for (size_t index{0}; index < size_; ++index) {
      data_[index] = data;
    }
  }

  // Contructor with initializer_list
  MyVector(std::initializer_list<T> data) : data_(new T[data.size()]), size_(data.size()), capacity_(data.size()) {
    size_t index{0};
    for (const auto& elem : data) {
      data_[index] = elem;
      ++index;
    }
  }

  // Copy constructor
  MyVector(const MyVector& other) : size_{other.size_}, capacity_{other.capacity_}, data_{new T[other.capacity_]} {
    for (size_t index{0}; index < size_; ++index) {
      data_[index] = other.data_[index];
    }
  }

  // Move constructor
  MyVector(MyVector&& other) noexcept : size_{other.size_}, capacity_{other.capacity_}, data_{other.data_} {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  // Copy assignment operator
  MyVector& operator=(const MyVector& other) {
    if (this != &other) {
      T* new_data = new T[other.size_];
      for (size_t index{0}; index < other.size_; ++index) {
        new_data[index] = other.data_[index];
      }
      delete[] data_;
      data_ = new_data;
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    return *this;
  }

  // Move assignment operator
  MyVector& operator=(MyVector&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      data_ = other.data_;

      other.size_ = 0;
      other.capacity_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }

  // Destructor
  ~MyVector() {
    if (data_ != nullptr) {
      delete[] data_;
    }
    size_ = 0;
    capacity_ = 0;
  }
  // opeartor []
  T& operator[](size_t index) { return data_[index]; }

  // operator [] for const
  const T& operator[](size_t index) const { return data_[index]; }

  // at
  T& at(size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of bounds");
    return data_[index];
  }

  const T& at(size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of bounds");
    return data_[index];
  }

  // push back
  void push_back(const T& data) {
    if (size_ >= capacity_) {
      size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
      this->reserve(new_capacity);
    }
    data_[size_] = data;
    ++size_;
  }

  // pop_back
  void pop_back() {
    if (size_ != 0) {
      --size_;
    }
  }

  // clear
  void clear() { size_ = 0; }

  // size()
  size_t size() const { return size_; }

  // capacity()
  size_t capacity() const { return capacity_; }

  // reserve
  void reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
      T* new_data = new T[new_capacity];

      for (size_t index{0}; index < size_; ++index) {
        new_data[index] = std::move_if_noexcept(data_[index]);
      }
      delete[] data_;
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

 private:
  T* data_{nullptr};
  size_t size_{0};
  size_t capacity_{0};
};

int main() {
  MyVector<int> v{4, 10};
  MyVector<double> v1{5};
  MyVector v2{1, 2, 3, 4, 5};
  MyVector<double> v3{1.0, 2.0};
  MyVector<int> v4{};
  MyVector<int> v5;

  std::cout << "v.size(): " << v.size() << std::endl;
  std::cout << "v.capacity(): " << v.capacity() << std::endl;
  std::cout << "v4.capacity(): " << v4.capacity() << std::endl;

  MyVector<int> test_v;
  for (int i = 0; i < 10; ++i) {
    test_v.push_back(i * 10);
    std::cout << "Added " << i * 10 << " | Size: " << test_v.size() << " | Cap: " << test_v.capacity() << std::endl;
  }
  try {
    std::cout << "Element at 100: " << test_v.at(100) << std::endl;
  } catch (const std::out_of_range& e) {
    std::cerr << "Caught expected error: " << e.what() << std::endl;
  }
}