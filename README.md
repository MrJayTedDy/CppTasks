# C++ Tasks

This repository contains solutions to various C++ programming tasks, focusing on advanced concepts and system design.

## Exercises Michael D. Adams
This section contains solutions to exercises from the book "Exercises for Programming in C++" by Michael D. Adams. It provides systematic coverage of the C++ language core and the Standard Library through structured textbook exercises.

## Random Advanced Tasks

### 1. Lambda Jump Table
Write a program that reads user input and dispatches to different actions based on the input character. Support commands A, B, C, D for printing messages and X for exiting the program. Normalize input to uppercase.

### 2. IoT Sensor Management System
Develop an IoT Sensor Management System using manual memory management without smart pointers or std::vector.
- Create a custom exception class named 'SensorConnectionError' that inherits from std::exception, accepts a string message, and overrides the what() method.
- Develop an abstract base class named 'Sensor' with a protected string member for sensor ID, a constructor to initialize it, a pure virtual function 'double readValue()', and a virtual destructor that prints the sensor ID being destroyed.
- Develop a derived class named 'TemperatureSensor' that inherits from 'Sensor' and implements 'readValue()'. If the sensor ID is "BROKEN_TEMP", throw 'SensorConnectionError'; otherwise, return a dummy temperature value.
- Develop a class named 'SensorManager' for basic RAII. It should contain a dynamically allocated array of Sensor pointers and track capacity and count. Constructor initializes an empty array of given capacity. Implement 'void addSensor(Sensor* s)' to add sensors if not full. Implement 'void pollAll()' to call 'readValue()' on all sensors, catching 'SensorConnectionError' and printing errors without stopping. Destructor must delete all sensors and the array to prevent leaks.

### 3. Static ID Manager
Create a class that manages unique IDs for objects using static members. Track the current available ID and object count. Implement prefix and postfix increment operators. Ensure copy constructor assigns new unique IDs. Track object lifecycle in constructor and destructor.

### 4. Buffered Batch Iterator
Create a templated output iterator that buffers elements and processes them in batches. Use STL iterator concepts and std::span for efficiency. Implement move semantics for the iterator.

### 5. Generic Numeric Transform
[Task description not available - file is empty]

## MySTL

### Custom Vector Implementation
Implement a custom vector class that mimics std::vector functionality. Support dynamic memory management, constructors, destructor, assignment operators, and basic operations like push_back, size, and capacity. Handle memory reallocation and element copying/moving.
* **Tools**: Designed for modern Clang/GCC compilers with strict warning levels (`-Wall -Wextra -Wpedantic`).