# Tech Spec: Lambda Jump Table (Command Dispatcher)

## 1. Context & Problem Statement
The primary goal of this project is to master **Modern C++ Lambda Expressions** through the implementation of a high-performance "Jump Table". This pattern replaces traditional branching logic (`if-else`, `switch`) with a hash-map lookup, achieving $O(1)$ average time complexity. 

The project focuses on understanding how lambdas function as anonymous objects (closures) and how they manage their internal state.

## 2. Functional Requirements

### 2.1. Command Mapping & Normalization
* **Input Handling**: The dispatcher must accept both uppercase (`A-D`) and lowercase (`a-d`) characters.
* **Output Consistency**: Regardless of the input case, the output must strictly display the **uppercase** version (e.g., entering 'a' results in `func A`).
* **Exit Logic**: The commands `X` or `x` must trigger a "Bye!" message and terminate the execution loop.

### 2.2. Input Validation
* **Empty Input**: If the user provides an empty string (e.g., via `std::getline`), the program must ignore it and prompt again.
* **Length Violation**: If the input exceeds a single character, the program must print `Response is too long!`.
* **Undefined Commands**: Any character not present in the jump table should trigger an `Invalid response` message.

## 3. Technical Implementation & Lambda Practice

### 3.1. Lambda Expression Usage
* **Custom Normalization**: Instead of using standard library functions, a custom lambda (`char_upper`) is implemented to handle manual ASCII-level character transformation (`c - ('a' - 'A')`).
* **Higher-Order Logic**: The project demonstrates lambda composition, where one lambda (`print_statement`) is captured and invoked inside other specialized closures within the table.
* **Capture Strategy**: Practice of **Capture by Value** `[print_statement]` to ensure that each dispatcher entry has its own copy of the callable utility, preventing dangling reference issues during the loop's lifetime.

### 3.2. Architecture
* **Standard**: C++20.
* **Core Container**: `std::unordered_map<char, std::function<void()>>`.
* **Input Method**: `std::getline` for robust handling of empty lines and white spaces.
* **Type Erasure**: Utilizing `std::function` to store different lambda closure types in a single uniform container.



## 4. Key Learning Objectives
* **Closure Mechanics**: Understanding how the compiler generates a unique class for each lambda.
* **Capture Clauses**: Differentiating between stateless and stateful lambdas.
* **Mapping Complexity**: Ensuring $O(1)$ lookup while maintaining clean, readable code.
* **Manual Memory/Bitwise Operations**: Practicing ASCII math within a functional context.

## 5. Success Criteria
1.  All inputs ('a' and 'A') consistently trigger the uppercase output (`func A`).
2.  The program effectively handles empty inputs without crashing (Bounds Checking).
3.  The `jump_table` lookup remains efficient and free of side effects (no accidental key insertion).
4.  Lambda captures are handled safely, demonstrating an understanding of **Object Lifetime**.