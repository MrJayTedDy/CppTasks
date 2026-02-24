#include <iostream>

class IDManager {
 public:
  // Constructor
  IDManager() : id_(current_available_id_) {
    ++current_available_id_;
    ++object_counter_;
  }

  // Copy Constructor
  IDManager(const IDManager& other) {
    static_cast<void>(other);
    id_ = current_available_id_++;
    ++object_counter_;
  }

  // Copy Assignement
  IDManager& operator=(const IDManager& other) {
    static_cast<void>(other);
    return *this;
  }

  // Destructor
  ~IDManager() { --object_counter_; }

  // Post-increment
  IDManager& operator++() {
    id_ += 1;
    return *this;
  }

  // Pre-increment
  IDManager operator++(int /* some_value*/) { return ++(*this); }

  // Getters
  unsigned int getID() const { return id_; }
  static unsigned int getObjectCounter() { return object_counter_; }
  static unsigned int getCurrentAvailableID() { return current_available_id_; }

 private:
  unsigned int id_;
  static unsigned int current_available_id_;
  static unsigned int object_counter_;
};

unsigned int IDManager::current_available_id_ = 0;
unsigned int IDManager::object_counter_ = 0;

void testID() {
  std::cout << "Object Counter Start: " << IDManager::getObjectCounter() << std::endl;      // Should be 0
  std::cout << "Avalilable ID Start: " << IDManager::getCurrentAvailableID() << std::endl;  // Should be 0
  {
    IDManager first_id{};
    IDManager second_id = first_id;                                                            // Copy constructor test
    std::cout << "Object Counter Inside block: " << first_id.getObjectCounter() << std::endl;  // Should be 2
    std::cout << "Current Available ID Inside block: " << first_id.getCurrentAvailableID() << std::endl;  // Should be 2
    std::cout << "First ID Inside block: " << first_id.getID() << std::endl;                              // Should be 0
    std::cout << "Second ID Inside block: " << second_id.getID() << std::endl;                            // Should be 1

    IDManager third_id{};
    third_id = second_id;  // Copy assignment operator test
    std::cout << "Object Counter After assignment: " << third_id.getObjectCounter() << std::endl;  // Should be 2
    std::cout << "Current Available ID After assignment: " << first_id.getCurrentAvailableID()
              << std::endl;  // Should be 2
    std::cout << "Third ID After assignment: " << third_id.getID()
              << std::endl;  // Should be 2 because copy assignement will
                             // increase the counter by 1;
  }
  // All counter should be destryed after {} block
  std::cout << "Object Counter at the End: " << IDManager::getObjectCounter() << std::endl;  // Should be 0
  // Unique ID should be used for the next id object
  std::cout << "Available ID at the End: " << IDManager::getCurrentAvailableID() << std::endl;  // Should be 2
}

int main() {
  testID();
  return 0;
}