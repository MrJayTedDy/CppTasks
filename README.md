# C++ Mastery: Academic Exercises & Advanced Systems Design

This repository is a comprehensive roadmap for mastering C++, covering everything from fundamental academic problems to advanced system architecture. The projects are organized into two distinct tracks: structured textbook exercises and custom-designed advanced components.

## 📂 Repository Structure

### 1. [Exercises_Michael_D_Adams](./Exercises_Michael_D_Adams/)
Solutions and implementations for the book **"Exercises for Programming in C++"** by Michael D. Adams.
* **Focus**: Systematic coverage of the C++ language core and the Standard Library.
* **Documentation**: See the [internal README](./Exercises_Michael_D_Adams/README.md) for the detailed file and chapter structure.

### 2. [Random Advanced tasks](./Random_Advanced_Tasks/)
A collection of complex, mentor-guided projects and self-designed components. Each project follows a professional "Feature Specification" approach.

#### 🛠 Projects & Learning Path:

* **[IoT Sensor Management System](./Random_Advanced_Tasks/sensors_system/SPEC.md)**
    * **Goal**: Evolution from low-level manual memory management to Modern C++ RAII.
    * **Concepts**: `new`/`delete`, Exception Safety, Rule of Five, `std::unique_ptr`, `std::vector`.

* **[Static ID Manager](./Random_Advanced_Tasks/id_manager/SPEC.md)**
    * **Goal**: Managing global state and unique object identification.
    * **Concepts**: Static members, Operator Overloading (prefix/postfix), Object lifecycle tracking.

* **[Buffered Batch Iterator](./Random_Advanced_Tasks/buffered_iterator/SPEC.md)**
    * **Goal**: High-performance output iterator for batch data processing.
    * **Concepts**: STL Iterator Concepts, Templates, `std::span`, Move Semantics.

* **[DSP Core & Dynamic Pipeline](./Random_Advanced_Tasks/dsp_core/SPEC.md)**
    * **Goal**: Designing a digital signal processing engine.
    * **Concepts**: Value Semantics, Operator Overloading, Dynamic vs. Static Polymorphism (CRTP).

---

## 🧭 Engineering Philosophy

Each feature and component in this repository is analyzed through three core engineering perspectives:
1.  **Syntax & Interface**: How does it look? (API design, keywords, and code readability).
2.  **Mechanisms**: How does it work? (Under-the-hood behavior, memory layout, vtables, and code generation).
3.  **Use Cases**: When to use it? (Contextual application, trade-offs, and selecting the right tool for the job).

## 🛠 Tech Stack
* **Language Standards**: C++20 / C++23.
* **Methodology**: Focus on "The Rule of Zero", Resource Safety (RAII), and Zero-cost Abstractions.
* **Tools**: Designed for modern Clang/GCC compilers with strict warning levels.