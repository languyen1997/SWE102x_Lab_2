#include <array>
#include <iostream>
#include <memory>
#include <string>

class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
 public:
  virtual void VisitConcreteComponentA(
      ConcreteComponentA const *element) const = 0;
  virtual void VisitConcreteComponentB(
      ConcreteComponentB const *element) const = 0;
};

class Component {
 public:
  virtual ~Component() = default;
  virtual void Accept(Visitor *visitor) const = 0;
};

class ConcreteComponentA : public Component {
 public:
  void Accept(Visitor *visitor) const override {
    visitor->VisitConcreteComponentA(this);
  }

  [[nodiscard]] std::string ExclusiveMethodOfConcreteComponentA() const {
    return "A";
  }
};

class ConcreteComponentB : public Component {
 public:
  void Accept(Visitor *visitor) const override {
    visitor->VisitConcreteComponentB(this);
  }

  [[nodiscard]] std::string SpecialMethodOfConcreteComponentB() const {
    return "B";
  }
};

class ConcreteVisitor1 : public Visitor {
 public:
  void VisitConcreteComponentA(
      ConcreteComponentA const *element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA()
              << " + ConcreteVisitor1\n";
  }

  void VisitConcreteComponentB(
      ConcreteComponentB const *element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB()
              << " + ConcreteVisitor1\n";
  }
};

class ConcreteVisitor2 : public Visitor {
 public:
  void VisitConcreteComponentA(
      ConcreteComponentA const *element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA()
              << " + ConcreteVisitor2\n";
  }

  void VisitConcreteComponentB(
      ConcreteComponentB const *element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB()
              << " + ConcreteVisitor2\n";
  }
};

void ClientCode(std::array<std::unique_ptr<Component>, 2> const &components,
                Visitor *visitor) {
  for (auto const &component : components) {
    component->Accept(visitor);
  }
}

int main() {
  std::array<std::unique_ptr<Component>, 2> components = {
      std::make_unique<ConcreteComponentA>(),
      std::make_unique<ConcreteComponentB>(),
  };
  std::cout << "The client code works with all visitors via the base Visitor "
               "interface:\n";
  auto visitor1 = std::make_unique<ConcreteVisitor1>();
  ClientCode(components, visitor1.get());
  std::cout << "\n";
  std::cout << "It allows the same client code to work with different types of "
               "visitors:\n";
  auto visitor2 = std::make_unique<ConcreteVisitor2>();
  ClientCode(components, visitor2.get());
}
