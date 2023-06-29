#include <iostream>
#include <memory>
#include <string>

class Subsystem1 {
 public:
  [[nodiscard]] std::string Operation1() const {
    return "Subsystem1: Ready!\n";
  }

  [[nodiscard]] std::string OperationN() const { return "Subsystem1: Go!\n"; }
};

class Subsystem2 {
 public:
  [[nodiscard]] std::string Operation1() const {
    return "Subsystem2: Get ready!\n";
  }

  [[nodiscard]] std::string OperationZ() const { return "Subsystem2: Fire!\n"; }
};

class Facade {
 protected:
  std::unique_ptr<Subsystem1> subsystem1_;
  std::unique_ptr<Subsystem2> subsystem2_;

 public:
  explicit Facade(std::unique_ptr<Subsystem1> subsystem1 = nullptr,
                  std::unique_ptr<Subsystem2> subsystem2 = nullptr)
      : subsystem1_(subsystem1 ? std::move(subsystem1)
                               : std::make_unique<Subsystem1>()),
        subsystem2_(subsystem2 ? std::move(subsystem2)
                               : std::make_unique<Subsystem2>()) {}

  std::string Operation() {
    std::string result = "Facade initialize subsystems:\n";
    result += this->subsystem1_->Operation1();
    result += this->subsystem2_->Operation1();
    result += "Facade orders subsystems to perform the action:\n";
    result += this->subsystem1_->OperationN();
    result += this->subsystem2_->OperationZ();
    return result;
  }
};

void ClientCode(Facade *facade) { std::cout << facade->Operation(); }

int main() {
  auto subsystem1 = std::make_unique<Subsystem1>();
  auto subsystem2 = std::make_unique<Subsystem2>();
  auto facade =
      std::make_unique<Facade>(std::move(subsystem1), std::move(subsystem2));
  ClientCode(facade.get());
}
