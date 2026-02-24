# Project Spec: Static ID Manager

## 1. Context & Problem Statement
This task focuses on class state management and the object lifecycle. The goal is to create a mechanism where every class instance receives a unique ID and the system tracks the number of currently active objects.

## 2. Project Goals
* Practice working with `static` members and methods to manage global class state.
* Distinguish between the global ID increment and the active object counter.

## 3. Functional Requirements
* **Unique ID**: Each new object is assigned the next available sequential ID.
* **Instance Counting**: Real-time tracking of the current number of created and destroyed objects.
* **Copy Behavior**: Upon copying, the new object must receive a new unique ID rather than duplicating the original ID.

## 4. Technical Constraints
* Implementation of both prefix and postfix increment (`operator++`).
* Proper use of static methods to access the internal class counters.

## 5. Success Criteria
* The `object_counter` correctly decrements when objects go out of scope.
* Deep copies or assignments do not lead to identifier collisions.