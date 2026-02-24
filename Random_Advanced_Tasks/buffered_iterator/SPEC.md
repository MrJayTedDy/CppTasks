# Project Spec: Buffered Batch Iterator

## 1. Context & Problem Statement
In high-throughput systems (networking/disk I/O), writing single elements is inefficient. This project implements a specialized iterator that accumulates data in a buffer and sends it as a "batch" once a specific threshold is reached.

## 2. Project Goals
* Implement a class that strictly satisfies the `std::output_iterator` concept.
* Utilize templates to create a generic solution for various data types and buffer sizes.

## 3. Functional Requirements
* **Buffering**: Collect elements of type `T` in an internal fixed-size array.
* **Callback Execution**: Trigger a callable object (lambda or function) when the buffer is full or upon iterator destruction (flush).
* **STL Compatibility**: Ensure the iterator works seamlessly with standard algorithms like `std::copy`.

## 4. Technical Constraints
* Use of `std::array` for fixed-size internal storage and `std::span` for safe data passing.
* Implementation of Move Semantics to transfer buffer state between iterator instances safely.

## 5. Success Criteria
* Guaranteed "flush" of remaining data in the destructor.
* Successful integration with the STL algorithm library, evidenced by unit tests.