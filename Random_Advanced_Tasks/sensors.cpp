// ============================================================================
// Concepts to practice:
// - Inheritance and Pure Virtual Functions (Interfaces)
// - Manual Memory Management (new / delete)
// - Error Handling (try / catch / custom exceptions)
// - Basic RAII principles (Resource Acquisition Is Initialization) in Manager
// ============================================================================

// Develop an IoT Sensor Management System.
// The code must not use smart pointers or std::vector yet. You must manage
// arrays and objects manually using 'new' and 'delete'.
//
// 1. Develop a custom exception class named 'SensorConnectionError'.
//    a. It must inherit from std::exception.
//    b. It must accept a string message in the constructor.
//    c. It must override the 'what()' method to return the error message.
//
// 2. Develop an abstract base class named 'Sensor'.
//    a. It must have a protected string member for the sensor ID.
//    b. It must have a constructor that initializes the sensor ID.
//    c. It must have a pure virtual function 'double readValue()'.
//    d. It must have a virtual destructor that prints a message indicating
//       which sensor ID is being destroyed.
//
// 3. Develop a derived class named 'TemperatureSensor'.
//    a. It must inherit from 'Sensor'.
//    b. It must implement 'readValue()'. This method should simulate reading
//       data. If the sensor ID is "BROKEN_TEMP", it must throw a
//       'SensorConnectionError'. Otherwise, return a dummy temperature value.
//
// 4. Develop a class named 'SensorManager' to demonstrate basic RAII.
//    a. It must contain a dynamically allocated array of Sensor pointers
//       (Sensor** sensors_) and an integer to track the current capacity and
//       count.
//    b. The constructor should initialize an empty array of a given capacity
//       using 'new Sensor*[capacity]'.
//    c. Implement a method 'void addSensor(Sensor* s)'. If the array is full,
//       ignore the addition (for simplicity in this step).
//    d. Implement a method 'void pollAll()'. It must iterate through all
//       added sensors and call 'readValue()'.
//    e. Inside 'pollAll()', wrap the 'readValue()' call in a try/catch block
//       to catch 'SensorConnectionError'. If caught, print the error but do
//       not stop the loop.
//    f. The destructor MUST iterate through the array, call 'delete' on each
//       Sensor pointer, and then delete the array itself to prevent memory
//       leaks.

#include <exception>
#include <iostream>
#include <string>

class SensorConnectionError : public std::exception {
 public:
  explicit SensorConnectionError(const std::string& message) : msg_(message) {}

  const char* what() const noexcept override { return msg_.c_str(); }

 private:
  std::string msg_;
};

// [Implementation of Sensor, TemperatureSensor, and SensorManager]
class Sensor {
 public:
  explicit Sensor(const std::string& id) : id_(id) { std::cout << "A " << id_ << " sensor is created!" << std::endl; }
  virtual double readValue() const = 0;
  virtual ~Sensor() { std::cout << "A " << id_ << " is destroyed!" << std::endl; }

 protected:
  std::string id_;
};

class TemperatureSensor : public Sensor {
 public:
  explicit TemperatureSensor(const std::string& id) : Sensor(id) {};
  virtual double readValue() const override {
    if (id_ == "BROKEN_TEMP") {
      throw SensorConnectionError(id_);
    } else {
      return dummy_temp_value;
    }
  }

 private:
  const double dummy_temp_value{2.4};
};

// class SensorManager {
//  public:
//   SensorManager(size_t capacity) : capacity_(capacity) {
//     sensors_ = new Sensor*[capacity];
//     counter_ = 0;
//   }

//   void addSensor(Sensor* s) {
//     if (counter_ < capacity_) {
//       sensors_[counter_] = s;
//       ++counter_;
//     }
//   }

//   void pollAll() {
//     for (size_t index{0}; index < counter_; ++index) {
//       try {
//         sensors_[index]->readValue();
//       } catch (SensorConnectionError& error) {
//         std::cout << "Wrong sensor ID: " << error.what() << std::endl;
//       }
//     }
//   }

//   ~SensorManager() {
//     for (size_t index{0}; index < counter_; ++index) {
//       delete sensors_[index];
//     }
//     delete[] sensors_;
//   }

//  private:
//   Sensor** sensors_;
//   size_t capacity_;
//   size_t counter_;
// };

// ============================================================================
// PART 2: THE RULE OF FIVE (Copy & Move Semantics)
// ============================================================================
//
// Background:
// Real sensors often produce large chunks of data (e.g., waveforms, images).
// Copying this data is expensive. We need a wrapper class that manages a
// dynamic array of integers, implementing proper Copy and Move semantics.
//
// 5. Develop a class named 'DataBuffer'.
//    a. It must hold a raw pointer 'int* data_' and a 'size_t size_'.
//    b. Constructor: Accepts 'size', allocates 'new int[size]', and fills it
//       with dummy data (e.g., 0, 1, 2...). Log "Buffer Constructor".
//    c. Destructor: Deletes the memory. Log "Buffer Destructor".
//
// 6. Implement Copy Semantics (The Rule of Three part).
//    a. Copy Constructor: Creates a NEW array of the same size and copies
//       values from the source (Deep Copy). Log "Buffer Copy Constructor".
//    b. Copy Assignment Operator (operator=):
//       - Check for self-assignment (this == &other).
//       - Delete old memory.
//       - Allocate new memory and copy values.
//       - Return *this.
//       - Log "Buffer Copy Assignment".
//
// 7. Implement Move Semantics (The optimization part).
//
//    a. Move Constructor: "Steals" the pointer from the source object.
//       - The source object's pointer MUST be set to nullptr.
//       - The source object's size MUST be set to 0.
//       - Log "Buffer Move Constructor".
//       - Mark as 'noexcept'.
//    b. Move Assignment Operator (operator=):
//       - Check for self-assignment.
//       - Delete own old memory.
//       - Steal pointers/size from source.
//       - Nullify source.
//       - Log "Buffer Move Assignment".
//       - Mark as 'noexcept'.
//
// 8. Integration Task.
//    a. Add a function 'void testDataBuffer()' to main.
//    b. Create a DataBuffer 'b1(10)'.
//    c. Create 'b2 = b1' (Test Copy).
//    d. Create 'b3(std::move(b1))' (Test Move).
//       (After this, 'b1' should be empty/null).
//    e. Print the size of b1, b2, and b3 to verify.

// [Your implementation of DataBuffer goes here]

class DataBuffer {
 public:
  DataBuffer(size_t size) : size_(size), data_(new int[size]) {
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = i;
    }

    std::cout << "DataBuffer Contructed!" << std::endl;
  }

  // Copy Constructor
  DataBuffer(const DataBuffer& other) : data_(new int[other.size_]), size_(other.size_) {
    for (size_t index{0}; index < size_; ++index) {
      data_[index] = other.data_[index];
    }

    std::cout << "DataBuffer Copy Constructor" << std::endl;
  }

  // Copy assignment
  DataBuffer& operator=(const DataBuffer& other) {
    if (this != &other) {
      int* new_data = new int[other.size_];
      for (size_t index{0}; index != other.size_; ++index) {
        new_data[index] = other.data_[index];
      }
      delete[] data_;
      data_ = new_data;
      size_ = other.size_;
    }
    std::cout << "DataBuffer Copy Assignment" << std::endl;
    return *this;
  }

  // Move constructor
  DataBuffer(DataBuffer&& other) noexcept : data_{other.data_}, size_{other.size_} {
    other.data_ = nullptr;
    other.size_ = 0;
    std::cout << "DataBuffer Move constructor" << std::endl;
  }

  // Move assignment
  DataBuffer& operator=(DataBuffer&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      other.data_ = nullptr;
      other.size_ = 0;
    }

    std::cout << "DataBuffer Move Assignment" << std::endl;
    return *this;
  }

  ~DataBuffer() {
    size_ = 0;
    delete[] data_;
  }

  void printDataBuffer() {
    std::cout << "DataBuffer size: " << size_ << std::endl;
    if ((data_ != nullptr) && (size_ != 0)) {
      for (size_t index{0}; index != size_; ++index) {
        std::cout << data_[index] << " ";
      }
      std::cout << std::endl;
    } else {
      std::cout << "Buffer is empty!" << std::endl;
    }
  }

 private:
  int* data_;
  size_t size_;
};

// ============================================================================
// STAGE 3: Advanced Error Handling and Strict RAII
// Concepts to practice:
// - Stack Unwinding: Understanding how destructors are called during exceptions.
// - Strict RAII: Using local objects to manage external resources (e.g., ports).
// - Exception Propagation: Choosing where to catch an error.
// ============================================================================

// 1. Develop a wrapper class named 'PortConnection'.
//    a. It should NOT inherit from 'Sensor'. It is a standalone utility.
//    b. It must have a private string member 'portName_'.
//    c. Constructor: Accepts a string name and prints "Port [name] opened".
//    d. Destructor: Prints "Port [name] closed".
//
// 2. Modify 'SensorManager::pollAll()':
//    a. At the very beginning of the method, create a local instance of
//       'PortConnection' on the stack (e.g., PortConnection port("COM1")).
//    b. Keep the current logic with try/catch inside the loop.
//    c. Observe the output: When is the port closed?
//
// 3. Logical Experiment (Task for you to analyze):
//    a. What happens if you remove the try/catch block from 'pollAll'
//       and instead wrap the call 'manager.pollAll()' in 'main'?
//    b. Will the 'PortConnection' destructor still be called if
//       'TemperatureSensor' throws 'SensorConnectionError'?
//    c. Try to implement this and verify your hypothesis.

/* PollALL modification

--- Starting Sensor System Test ---
A TEMP_LIVING_ROOM sensor is created!
A BROKEN_TEMP sensor is created!
A TEMP_KITCHEN sensor is created!

Polling sensors:
Port COM1 opened
Wrong sensor ID: BROKEN_TEMP
Port COM1 closed

Destroying manager and cleaning up:
A TEMP_LIVING_ROOM is destroyed!
A BROKEN_TEMP is destroyed!
A TEMP_KITCHEN is destroyed!
--- End of Test ---


*/

/*

--- Starting Sensor System Test ---
A TEMP_LIVING_ROOM sensor is created!
A BROKEN_TEMP sensor is created!
A TEMP_KITCHEN sensor is created!

Polling sensors:
Port COM1 opened
libc++abi: terminating due to uncaught exception of type SensorConnectionError: BROKEN_TEMP

This what is happened when I removed try-catch block at all- std::terminate was called.

*/

/*

--- Starting Sensor System Test ---
A TEMP_LIVING_ROOM sensor is created!
A BROKEN_TEMP sensor is created!
A TEMP_KITCHEN sensor is created!

Polling sensors:
Port COM1 opened
Port COM1 closed
A TEMP_LIVING_ROOM is destroyed!
A BROKEN_TEMP is destroyed!
A TEMP_KITCHEN is destroyed!
BROKEN_TEMP


When we moved the try-catch block into the main - RAII worked perfectly

*/

class PortConnection {
 public:
  PortConnection(std::string port_name) : port_name_(port_name) {
    std::cout << "Port " << port_name_ << " opened" << std::endl;
  }

  ~PortConnection() { std::cout << "Port " << port_name_ << " closed" << std::endl; }

 private:
  std::string port_name_;
};

// ============================================================================
// STAGE 4: Modernization & The Rule of Zero
//
// Concepts to practice:
// - Ownership Semantics (Exclusive ownership with std::unique_ptr).
// - RAII with STL Containers (std::vector for resource management).
// - Transfer of ownership (std::move in modern interfaces).
// - The Rule of Zero (Simplifying class design by delegating cleanup).
// - Factory functions (std::make_unique for exception safety).
// ============================================================================
//
// OBJECTIVE:
// Refactor the existing 'SensorManager' and the testing suite to eliminate
// all manual memory management. The code must follow Modern C++ standards.
//
// REQUIREMENTS:
// 1. Transition 'SensorManager' from manual array management to a suitable
//    STL container that automates lifecycle management of its elements.
// 2. Ensure that sensors are managed via smart pointers that enforce
//    exclusive ownership.
// 3. Update the sensor addition mechanism to explicitly handle the
//    transfer of ownership from the caller to the manager.
// 4. Apply the "Rule of Zero": remove all manual implementation of
//    special member functions (destructors, copy/move logic) in 'SensorManager'.
// 5. Modernize the test suite by using standard factory functions for
//    object creation.
//
// CONSTRAINTS:
// - Total ban on 'new', 'delete', 'new[]', and 'delete[]'.
// - No manual tracking of size or capacity.
// ============================================================================

class SensorManager {
 public:
  void addSensor(std::unique_ptr<Sensor> s) { sensors_.push_back(std::move(s)); }

  void pollAll() {
    for (const auto& sensor : sensors_) {
      try {
        sensor->readValue();
      } catch (SensorConnectionError& error) {
        std::cout << "Wrong sensor ID: " << error.what() << std::endl;
      }
    }
  }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;
};

void testSensorSystemNew() {
  std::cout << "--- Starting Sensor System Test ---" << std::endl;

  {
    SensorManager manager;  // Capacity of 5

    manager.addSensor(std::make_unique<TemperatureSensor>("TEMP_LIVING_ROOM"));
    manager.addSensor(std::make_unique<TemperatureSensor>("BROKEN_TEMP"));  // Will throw on read
    manager.addSensor(std::make_unique<TemperatureSensor>("TEMP_KITCHEN"));

    std::cout << "\nPolling sensors:" << std::endl;
    manager.pollAll();

    std::cout << "\nDestroying manager and cleaning up:" << std::endl;
  }  // manager goes out of scope here, destructor should clean up all 'new' calls

  std::cout << "--- End of Test ---" << std::endl;
}

void testDataBuffer() {
  DataBuffer b1(10);
  DataBuffer b2 = b1;
  DataBuffer b3(std::move(b1));

  std::cout << "\nTesting Assignments:" << std::endl;
  DataBuffer b4(5);
  b4 = b2;
  DataBuffer b5(3);
  b5 = std::move(b2);

  b1.printDataBuffer();
  b2.printDataBuffer();
  b3.printDataBuffer();
  b4.printDataBuffer();
  b5.printDataBuffer();
}

int main() {
  // testSensorsSystems();
  // testDataBuffer();

  // try {
  //   testSensorSystem();
  // } catch (SensorConnectionError& e) {
  //   std::cout << e.what() << std::endl;
  // }

  testSensorSystemNew();
  return 0;
}