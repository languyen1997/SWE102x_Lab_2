#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

enum Type {
  kPrototype1,
  kPrototype2,
};

class Prototype {
 protected:
  std::string prototype_name_;
  float prototype_field_;

 public:
  Prototype() = default;
  explicit Prototype(std::string prototype_name)
      : prototype_name_(std::move(prototype_name)) {}

  virtual ~Prototype() = default;
  [[nodiscard]] virtual std::unique_ptr<Prototype> Clone() const = 0;
  virtual void Method(float prototype_field) {
    this->prototype_field_ = prototype_field;
    std::cout << "Call Method from " << prototype_name_
              << " with field : " << prototype_field << std::endl;
  }
};

class ConcretePrototype1 : public Prototype {
 private:
  float concrete_prototype_field1_;

 public:
  ConcretePrototype1(std::string prototype_name, float concrete_prototype_field)
      : Prototype(std::move(prototype_name)),
        concrete_prototype_field1_(concrete_prototype_field) {}

  [[nodiscard]] std::unique_ptr<Prototype> Clone() const override {
    return std::make_unique<ConcretePrototype1>(*this);
  }
};

class ConcretePrototype2 : public Prototype {
 private:
  float concrete_prototype_field2_;

 public:
  ConcretePrototype2(std::string prototype_name, float concrete_prototype_field)
      : Prototype(std::move(prototype_name)),
        concrete_prototype_field2_(concrete_prototype_field) {}

  [[nodiscard]] std::unique_ptr<Prototype> Clone() const override {
    return std::make_unique<ConcretePrototype2>(*this);
  }
};

class PrototypeFactory {
 private:
  std::unordered_map<Type, std::unique_ptr<Prototype>, std::hash<int>>
      prototypes_;

 public:
  PrototypeFactory() {
    prototypes_[Type::kPrototype1] =
        std::make_unique<ConcretePrototype1>("PROTOTYPE_1", 50.F);
    prototypes_[Type::kPrototype2] =
        std::make_unique<ConcretePrototype2>("PROTOTYPE_2", 60.F);
  }

  std::unique_ptr<Prototype> CreatePrototype(Type type) {
    return prototypes_[type]->Clone();
  }
};

void Client(PrototypeFactory &prototype_factory) {
  {
    std::cout << "Let's create a Prototype 1\n";
    std::unique_ptr<Prototype> prototype =
        prototype_factory.CreatePrototype(Type::kPrototype1);
    prototype->Method(90);
  }
  std::cout << '\n';

  {
    std::cout << "Let's create a Prototype 2\n";
    std::unique_ptr<Prototype> prototype =
        prototype_factory.CreatePrototype(Type::kPrototype2);
    prototype->Method(10);
  }
}

int main() {
  PrototypeFactory prototype_factory;
  Client(prototype_factory);
}
