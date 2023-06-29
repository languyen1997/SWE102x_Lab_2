#include <iostream>
#include <memory>
#include <string>

class Product {
 public:
  virtual ~Product() = default;
  [[nodiscard]] virtual std::string Operation() const = 0;
};

class ConcreteProduct1 : public Product {
 public:
  [[nodiscard]] std::string Operation() const override {
    return "{Result of the ConcreteProduct1}";
  }
};

class ConcreteProduct2 : public Product {
 public:
  [[nodiscard]] std::string Operation() const override {
    return "{Result of the ConcreteProduct2}";
  }
};

class Creator {
 public:
  virtual ~Creator() = default;
  [[nodiscard]] virtual std::unique_ptr<Product> FactoryMethod() const = 0;

  [[nodiscard]] std::string SomeOperation() const {
    std::unique_ptr<Product> product = this->FactoryMethod();
    std::string result =
        "Creator: The same creator's code has just worked with " +
        product->Operation();
    return result;
  };
};

class ConcreteCreator1 : public Creator {
 public:
  [[nodiscard]] std::unique_ptr<Product> FactoryMethod() const override {
    return std::make_unique<ConcreteProduct1>();
  }
};

class ConcreteCreator2 : public Creator {
 public:
  [[nodiscard]] std::unique_ptr<Product> FactoryMethod() const override {
    return std::make_unique<ConcreteProduct2>();
  }
};

void ClientCode(Creator const &creator) {
  std::cout
      << "Client: I'm not aware of the creator's class, but it still works.\n"
      << creator.SomeOperation() << std::endl;
}

int main() {
  {
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    ConcreteCreator1 creator;
    ClientCode(creator);
    std::cout << std::endl;
  }
  {
    std::cout << "App: Launched with the ConcreteCreator2.\n";
    ConcreteCreator2 creator;
    ClientCode(creator);
    std::cout << std::endl;
  }
}