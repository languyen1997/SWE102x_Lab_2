#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Product1 {
 public:
  std::vector<std::string> parts_;

  void ListParts() const {
    std::cout << "Product parts: ";
    for (auto const &part : parts_) {
      if (part == parts_.back()) {
        std::cout << part;
      } else {
        std::cout << part << ", ";
      }
    }

    std::cout << "\n\n";
  }
};

class Builder {
 public:
  virtual ~Builder() = default;
  virtual void ProducePartA() const = 0;
  virtual void ProducePartB() const = 0;
  virtual void ProducePartC() const = 0;
};

class ConcreteBuilder1 : public Builder {
 private:
  std::unique_ptr<Product1> product_;

 public:
  ConcreteBuilder1() { this->Reset(); }

  void Reset() { this->product_ = std::make_unique<Product1>(); }

  void ProducePartA() const override {
    this->product_->parts_.emplace_back("PartA1");
  }

  void ProducePartB() const override {
    this->product_->parts_.emplace_back("PartB1");
  }

  void ProducePartC() const override {
    this->product_->parts_.emplace_back("PartC1");
  }

  std::unique_ptr<Product1> GetProduct() {
    auto result = std::move(this->product_);
    this->Reset();
    return result;
  }
};

class Director {
  Builder *builder_;

 public:
  void SetBuilder(Builder *builder) { this->builder_ = builder; }

  void BuildMinimumViableProduct() { this->builder_->ProducePartA(); }

  void BuildFullFeaturedProduct() {
    this->builder_->ProducePartA();
    this->builder_->ProducePartB();
    this->builder_->ProducePartC();
  }
};

void ClientCode(Director &director) {
  auto builder = std::make_unique<ConcreteBuilder1>();
  director.SetBuilder(builder.get());

  {
    std::cout << "Standard basic product:\n";
    director.BuildMinimumViableProduct();

    auto product = builder->GetProduct();
    product->ListParts();
  }

  {
    std::cout << "Standard full featured product:\n";
    director.BuildFullFeaturedProduct();

    auto product = builder->GetProduct();
    product->ListParts();
  }

  {
    std::cout << "Custom product:\n";
    builder->ProducePartA();
    builder->ProducePartC();

    auto product = builder->GetProduct();
    product->ListParts();
  }
}

int main() {
  Director director;
  ClientCode(director);
}
