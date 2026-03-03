#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  using jumpFunc = std::function<void()>;

  auto char_upper = [](char c) -> char {
    if (c >= 'a' && c <= 'z') {
      return c - ('a' - 'A');
    } else {
      return c;
    }
  };

  auto print_statement = [](char c) -> void { std::cout << "func " << c << std::endl; };

  std::unordered_map<char, jumpFunc> jump_table{{'A', [print_statement]() { print_statement('A'); }},
                                                {'B', [print_statement]() { print_statement('B'); }},
                                                {'C', [print_statement]() { print_statement('C'); }},
                                                {'D', [print_statement]() { print_statement('D'); }},
                                                {'X', []() { std::cout << "Bye!" << std::endl; }}};

  std::string input{};
  while (true) {
    std::cout << "What to do? (A/B/C/D/X) > ";
    std::getline(std::cin, input);

    if (input.empty()) continue;

    if (input.size() > 1) {
      std::cout << "Response is too long!" << std::endl;
      continue;
    }

    auto result = jump_table.find(char_upper(input[0]));
    if (result != jump_table.end()) {
      result->second();
      if (result->first == 'X') break;
    } else {
      std::cout << "Invalid response" << std::endl;
      continue;
    }
  }

  return 0;
}