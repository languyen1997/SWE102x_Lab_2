#include <algorithm>
#include <iostream>
#include <string>

class Target {
 public:
  virtual ~Target() = default;

  [[nodiscard]] virtual std::string Request() const {
    return "Target: The default target's behavior.";
  }
};

class Adaptee {
 public:
  [[nodiscard]] std::string SpecificRequest() const {
    return ".eetpadA eht fo roivaheb laicepS";
  }
};

class Adapter : public Target, public Adaptee {
 public:
  [[nodiscard]] std::string Request() const override {
    std::string to_reverse = SpecificRequest();
    std::reverse(to_reverse.begin(), to_reverse.end());
    return "Adapter: (TRANSLATED) " + to_reverse;
  }
};

void ClientCode(Target const &target) { std::cout << target.Request(); }

int main() {
  std::cout << "Client: I can work just fine with the Target objects:\n";
  Target target;
  ClientCode(target);
  std::cout << "\n\n";
  Adaptee adaptee;
  std::cout << "Client: The Adaptee class has a weird interface. See, I don't "
               "understand it:\n";
  std::cout << "Adaptee: " << adaptee.SpecificRequest();
  std::cout << "\n\n";
  std::cout << "Client: But I can work with it via the Adapter:\n";
  Adapter adapter;
  ClientCode(adapter);
  std::cout << "\n";
}