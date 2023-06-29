#include <iostream>
#include <memory>
#include <string>

class Implementation {
 public:
  virtual ~Implementation() = default;
  [[nodiscard]] virtual std::string OperationImplementation() const = 0;
};

class ConcreteImplementationA : public Implementation {
 public:
  [[nodiscard]] std::string OperationImplementation() const override {
    return "ConcreteImplementationA: Here's the result on the platform A.\n";
  }
};

class ConcreteImplementationB : public Implementation {
 public:
  [[nodiscard]] std::string OperationImplementation() const override {
    return "ConcreteImplementationB: Here's the result on the platform B.\n";
  }
};

class Abstraction {
 protected:
  Implementation *implementation_;

 public:
  explicit Abstraction(Implementation *implementation)
      : implementation_(implementation) {}

  virtual ~Abstraction() = default;

  [[nodiscard]] virtual std::string Operation() const {
    return "Abstraction: Base operation with:\n" +
           this->implementation_->OperationImplementation();
  }
};

class ExtendedAbstraction : public Abstraction {
 public:
  explicit ExtendedAbstraction(Implementation *implementation)
      : Abstraction(implementation) {}

  [[nodiscard]] std::string Operation() const override {
    return "ExtendedAbstraction: Extended operation with:\n" +
           this->implementation_->OperationImplementation();
  }
};

void ClientCode(Abstraction const &abstraction) {
  std::cout << abstraction.Operation();
}

int main() {
  std::unique_ptr<Implementation> implementation =
      std::make_unique<ConcreteImplementationA>();
  std::unique_ptr<Abstraction> abstraction =
      std::make_unique<Abstraction>(implementation.get());
  ClientCode(*abstraction);
  std::cout << std::endl;

  implementation = std::make_unique<ConcreteImplementationB>();
  abstraction = std::make_unique<ExtendedAbstraction>(implementation.get());
  ClientCode(*abstraction);
}
