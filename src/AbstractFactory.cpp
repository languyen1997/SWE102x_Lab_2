#include <iostream>
#include <memory>
#include <string>

class AbstractProductA {
 public:
  virtual ~AbstractProductA() = default;
  [[nodiscard]] virtual std::string UsefulFunctionA() const = 0;
};

class ConcreteProductA1 : public AbstractProductA {
 public:
  [[nodiscard]] std::string UsefulFunctionA() const override {
    return "The result of the product A1.";
  }
};

class ConcreteProductA2 : public AbstractProductA {
 public:
  [[nodiscard]] std::string UsefulFunctionA() const override {
    return "The result of the product A2.";
  }
};

class AbstractProductB {
 public:
  virtual ~AbstractProductB() = default;
  [[nodiscard]] virtual std::string UsefulFunctionB() const = 0;
  [[nodiscard]] virtual std::string AnotherUsefulFunctionB(
      AbstractProductA const &collaborator) const = 0;
};

class ConcreteProductB1 : public AbstractProductB {
 public:
  [[nodiscard]] std::string UsefulFunctionB() const override {
    return "The result of the product B1.";
  }

  [[nodiscard]] std::string AnotherUsefulFunctionB(
      AbstractProductA const &collaborator) const override {
    const std::string result = collaborator.UsefulFunctionA();
    return "The result of the B1 collaborating with ( " + result + " )";
  }
};

class ConcreteProductB2 : public AbstractProductB {
 public:
  [[nodiscard]] std::string UsefulFunctionB() const override {
    return "The result of the product B2.";
  }

  [[nodiscard]] std::string AnotherUsefulFunctionB(
      AbstractProductA const &collaborator) const override {
    const std::string result = collaborator.UsefulFunctionA();
    return "The result of the B2 collaborating with ( " + result + " )";
  }
};

class AbstractFactory {
 public:
  [[nodiscard]] virtual std::unique_ptr<AbstractProductA> CreateProductA()
      const = 0;
  [[nodiscard]] virtual std::unique_ptr<AbstractProductB> CreateProductB()
      const = 0;
};

class ConcreteFactory1 : public AbstractFactory {
 public:
  [[nodiscard]] std::unique_ptr<AbstractProductA> CreateProductA()
      const override {
    return std::make_unique<ConcreteProductA1>();
  }

  [[nodiscard]] std::unique_ptr<AbstractProductB> CreateProductB()
      const override {
    return std::make_unique<ConcreteProductB1>();
  }
};

class ConcreteFactory2 : public AbstractFactory {
 public:
  [[nodiscard]] std::unique_ptr<AbstractProductA> CreateProductA()
      const override {
    return std::make_unique<ConcreteProductA2>();
  }

  [[nodiscard]] std::unique_ptr<AbstractProductB> CreateProductB()
      const override {
    return std::make_unique<ConcreteProductB2>();
  }
};

void ClientCode(AbstractFactory const &factory) {
  std::unique_ptr<AbstractProductA> product_a = factory.CreateProductA();
  std::unique_ptr<AbstractProductB> product_b = factory.CreateProductB();
  std::cout << product_b->UsefulFunctionB() << '\n';
  std::cout << product_b->AnotherUsefulFunctionB(*product_a) << '\n';
}

int main() {
  std::cout << "Client: Testing client code with the first factory type:\n";
  ConcreteFactory1 factory1;
  ClientCode(factory1);
  std::cout << std::endl;
  std::cout
      << "Client: Testing the same client code with the second factory type:\n";
  ConcreteFactory2 factory2;
  ClientCode(factory2);
  std::cout << std::endl;
}
