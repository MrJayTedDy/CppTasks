#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <span>
#include <vector>

template <typename T, std::size_t BufferSize>
class BufferedIterator {
 public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::output_iterator_tag;

  template <typename Callable>
  explicit BufferedIterator(Callable&& cb)
      : callback_(std::forward<Callable>(cb)), index_(0) {}

  BufferedIterator(const BufferedIterator& other)
      : buffer_(), callback_(other.callback_), index_(0) {
    // Copy constructor creates a fresh buffer - source keeps its state
  }

  BufferedIterator(BufferedIterator&& other)
      : buffer_(std::move(other.buffer_)),
        callback_(std::move(other.callback_)),
        index_(other.index_) {
    other.index_ = 0;
  }

  BufferedIterator& operator=(const BufferedIterator& other) {
    if (this != &other) {
      flush();  // Flush current buffer
      callback_ = other.callback_;
      index_ = 0;  // Start with empty buffer
      // Note: we don't copy other's buffer content - copy assignment creates
      // fresh state
    }
    return *this;
  }

  BufferedIterator& operator=(BufferedIterator&& other) {
    if (this != &other) {
      flush();
      callback_ = std::move(other.callback_);
      buffer_ = std::move(other.buffer_);
      index_ = other.index_;
      other.index_ = 0;
    }
    return *this;
  }

  ~BufferedIterator() { flush(); }

  BufferedIterator& operator*() { return *this; }
  BufferedIterator& operator++() { return *this; }
  BufferedIterator& operator++(int) { return *this; }

  BufferedIterator& operator=(const T& value) {
    buffer_[index_++] = value;
    if (index_ == BufferSize) {
      flush();
    }
    return *this;
  }

 private:
  void flush() {
    if (index_ > 0) {
      callback_(std::span<const T>(buffer_.data(), index_));
      index_ = 0;
    }
  }

  std::array<T, BufferSize> buffer_;
  std::function<void(std::span<const T>)> callback_;
  std::size_t index_;
};

namespace tests {

void test_basic_batching() {
  std::cout << "Test 1: Basic batching functionality\n";

  std::vector<std::vector<int>> batches;
  auto collector = [&](std::span<const int> batch) {
    batches.emplace_back(batch.begin(), batch.end());
  };

  BufferedIterator<int, 3> it(collector);
  *it = 1;
  ++it;
  *it = 2;
  ++it;
  *it = 3;
  ++it;

  assert(batches.size() == 1);
  assert(batches[0] == std::vector<int>({1, 2, 3}));
  std::cout << "✓ Batching works correctly\n\n";
}

void test_partial_flush_on_destruction() {
  std::cout << "Test 2: Partial flush on destruction\n";

  std::vector<std::vector<int>> batches;
  auto collector = [&](std::span<const int> batch) {
    batches.emplace_back(batch.begin(), batch.end());
  };

  {
    BufferedIterator<int, 5> it(collector);
    *it = 10;
    ++it;
    *it = 20;
    ++it;
  }
  assert(batches.size() == 1);
  assert(batches[0] == std::vector<int>({10, 20}));
  std::cout << "✓ Partial buffer flushes on destruction\n\n";
}

void test_empty_iterator_no_flush() {
  std::cout << "Test 3: Empty iterator doesn't flush\n";

  std::vector<std::vector<int>> batches;
  auto collector = [&](std::span<const int> batch) {
    batches.emplace_back(batch.begin(), batch.end());
  };

  {
    BufferedIterator<int, 3> it(collector);
  }

  assert(batches.size() == 0);
  std::cout << "✓ Empty buffer doesn't trigger flush\n\n";
}

void test_safe_copy_semantics() {
  std::cout << "Test 4: Safe copy semantics (no data duplication)\n";

  std::vector<std::vector<int>> batches;
  auto collector = [&](std::span<const int> batch) {
    batches.emplace_back(batch.begin(), batch.end());
  };

  {
    BufferedIterator<int, 3> it1(collector);
    *it1 = 100;
    ++it1;
    *it1 = 200;
    ++it1;

    BufferedIterator<int, 3> it2(collector);
    it2 = it1;

    *it2 = 300;
    ++it2;
    *it2 = 400;
    ++it2;
    *it2 = 500;
    ++it2;
  }  // Both destructors run here

  assert(batches.size() == 2);
  assert(batches[0] == std::vector<int>({300, 400, 500}));
  assert(batches[1] == std::vector<int>({100, 200}));

  std::cout << "✓ Copy assignment is safe - no data duplication\n\n";
}

void test_move_semantics() {
  std::cout << "Test 5: Move semantics transfer buffer state\n";

  std::vector<std::vector<int>> batches;
  auto collector = [&](std::span<const int> batch) {
    batches.emplace_back(batch.begin(), batch.end());
  };

  BufferedIterator<int, 4> it1(collector);
  *it1 = 1;
  ++it1;
  *it1 = 2;
  ++it1;

  BufferedIterator<int, 4> it2 = std::move(it1);
  *it2 = 3;
  ++it2;
  *it2 = 4;
  ++it2;

  assert(batches.size() == 1);
  assert(batches[0] == std::vector<int>({1, 2, 3, 4}));
  std::cout << "✓ Move constructor preserves buffer state\n\n";
}

void test_std_copy_integration() {
  std::cout << "Test 6: Integration with std::copy\n";

  std::vector<int> source = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  std::vector<int> collected;

  auto collector = [&](std::span<const int> batch) {
    collected.insert(collected.end(), batch.begin(), batch.end());
  };

  std::copy(source.begin(), source.end(), BufferedIterator<int, 4>(collector));

  assert(collected == source);
  std::cout << "✓ Works correctly with std::copy algorithm\n\n";
}

void test_different_buffer_sizes() {
  std::cout << "Test 7: Different buffer sizes\n";

  std::vector<std::vector<int>> batches;
  auto collector = [&](std::span<const int> batch) {
    batches.emplace_back(batch.begin(), batch.end());
  };

  // Test buffer size 1 (immediate flush)
  {
    BufferedIterator<int, 1> it(collector);
    *it = 42;
    ++it;
  }

  assert(batches.size() == 1);
  assert(batches[0] == std::vector<int>({42}));

  batches.clear();

  // Test large buffer
  {
    BufferedIterator<int, 100> it(collector);
    for (int i = 0; i < 50; ++i) {
      *it = i;
      ++it;
    }
  }

  assert(batches.size() == 1);
  assert(batches[0].size() == 50);

  std::cout << "✓ Different buffer sizes work correctly\n\n";
}

void test_callback_flexibility() {
  std::cout << "Test 8: Callback flexibility\n";

  int call_count = 0;

  auto counter = [&call_count](std::span<const int> batch) {
    call_count += static_cast<int>(batch.size());
  };

  {
    BufferedIterator<int, 3> it(counter);
    *it = 1;
    ++it;
    *it = 2;
    ++it;
    *it = 3;
    ++it;
    *it = 4;
    ++it;
  }

  assert(call_count == 4);
  std::cout << "✓ Callback flexibility works\n\n";
}

void test_iterator_requirements() {
  std::cout << "Test 9: Output iterator requirements\n";

  std::vector<int> collected;
  auto collector = [&](std::span<const int> batch) {
    collected.insert(collected.end(), batch.begin(), batch.end());
  };

  BufferedIterator<int, 5> it(collector);

  auto& ref1 = *it;
  auto& ref2 = ++it;
  auto copy = it++;
  ref1 = 42;

  assert(&ref1 == &it);
  assert(&ref2 == &it);

  std::cout << "✓ Satisfies output iterator requirements\n\n";
}

void run_all_tests() {
  std::cout << "=== Running BufferedIterator Unit Tests ===\n\n";

  test_basic_batching();
  test_partial_flush_on_destruction();
  test_empty_iterator_no_flush();
  test_safe_copy_semantics();
  test_move_semantics();
  test_std_copy_integration();
  test_different_buffer_sizes();
  test_callback_flexibility();
  test_iterator_requirements();

  std::cout << "All tests passed! BufferedIterator works correctly.\n\n";
}

}  // namespace tests

//  Implement the BufferedIterator class that satisfies the output_iterator concept.
//  Its constructor must accept any object that can be called with an appropriate std::array.
//  The iterator shall collect at most SIZE (defined in the template) values and pass them to the callable in batches.
//  Implement unit tests for the class.

int main() {
  tests::run_all_tests();

  constexpr std::size_t count = 25;
  std::vector<int> numbers(count);
  std::iota(numbers.begin(), numbers.end(), 0);

  auto writeBatch = [batch = 1u](const auto& arr) mutable {
    std::cout << "batch " << batch << ": ";
    ++batch;
    std::copy(arr.begin(), arr.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\n";
  };
  constexpr std::size_t bufferSize = 10;

  std::copy(numbers.begin(), numbers.end(),
            BufferedIterator<decltype(numbers)::value_type, bufferSize>(
                std::move(writeBatch)));
  // expected output:
  // batch 1: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  // batch 2: 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // batch 3: 20, 21, 22, 23, 24,

  return 0;
}
