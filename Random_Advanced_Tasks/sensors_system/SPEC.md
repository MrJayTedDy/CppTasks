# Project Spec: IoT Sensor Management System

## 1. Context & Problem Statement
This project explores the evolution of resource management in C++. It requires implementing a system that manages various sensor types while ensuring robustness against runtime errors and preventing memory leaks in a polymorphic environment.

## 2. Project Goals
* Understand the transition from raw pointers to smart pointers.
* Implement Exception Safety using RAII (Resource Acquisition Is Initialization).

## 3. Functional Requirements
* **Polymorphic Sensors**: An abstract base class `Sensor` and derived types like `TemperatureSensor`.
* **Error Handling**: Throwing custom exceptions (`SensorConnectionError`) when reading from malfunctioning hardware.
* **Management Layer**: A `SensorManager` class to perform group operations and automated cleanup.

## 4. Technical Constraints
* **Stage 1-2**: Manual array management using `new` and `delete`. Use of `std::vector` or smart pointers is strictly prohibited.
* **Stage 3**: Implementation of the "Rule of Five" (Copy/Move constructors and assignment operators).
* **Stage 4**: Refactoring the system to use `std::unique_ptr` and `std::vector` to achieve the "Rule of Zero".

## 5. Success Criteria
* Zero memory leaks, verified through destructor logging and resource tracking.
* Proper exception propagation that allows the system to continue polling even if one sensor fails.