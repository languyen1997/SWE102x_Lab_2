#include <iostream>
#include <memory>
#include <string>

class Component {
 public:
  virtual ~Component() = default;
  [[nodiscard]] virtual std::string Operation() const = 0;
};

class ConcreteComponent : public Component {
 public:
  [[nodiscard]] std::string Operation() const override {
    return "ConcreteComponent";
  }
};

class Decorator : public Component {
 protected:
  Component *component_;

 public:
  explicit Decorator(Component *component) : component_(component) {}

  [[nodiscard]] std::string Operation() const override {
    return this->component_->Operation();
  }
};

class ConcreateDecoratorA : public Decorator {
 public:
  explicit ConcreateDecoratorA(Component *component) : Decorator(component) {}

  [[nodiscard]] std::string Operation() const override {
    return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
  }
};

class ConcreateDecoratorB : public Decorator {
 public:
  explicit ConcreateDecoratorB(Component *component) : Decorator(component) {}

  [[nodiscard]] std::string Operation() const override {
    return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
  }
};

void ClientCode(Component *component) {
  std::cout << "RESULT: " << component->Operation();
}

int main() {
  auto simple = std::make_unique<ConcreteComponent>();
  std::cout << "Client: I've got a simple component:\n";
  ClientCode(simple.get());
  std::cout << "\n\n";

  auto decorator1 = std::make_unique<ConcreateDecoratorA>(simple.get());
  auto decorator2 = std::make_unique<ConcreateDecoratorB>(decorator1.get());
  std::cout << "Client: Now I've got a decorated component:\n";
  ClientCode(decorator2.get());
  std::cout << "\n";
}
