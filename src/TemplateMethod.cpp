#include <iostream>
#include <memory>

class AbstractClass {
 public:
  void TemplateMethod() const {
    this->BaseOperation1();
    this->RequiredOperation1();
    this->BaseOperation2();
    this->Hook1();
    this->RequiredOperation2();
    this->BaseOperation3();
    this->Hook2();
  }

 protected:
  void BaseOperation1() const {
    std::cout << "AbstractClass says: I am doing the bulk of the work\n";
  }
  void BaseOperation2() const {
    std::cout << "AbstractClass says: But I let subclasses override some "
                 "operations\n";
  }
  void BaseOperation3() const {
    std::cout
        << "AbstractClass says: But I am doing the bulk of the work anyway\n";
  }

  virtual void RequiredOperation1() const = 0;
  virtual void RequiredOperation2() const = 0;

  virtual void Hook1() const {}
  virtual void Hook2() const {}
};

class ConcreteClass1 : public AbstractClass {
 protected:
  void RequiredOperation1() const override {
    std::cout << "ConcreteClass1 says: Implemented Operation1\n";
  }
  void RequiredOperation2() const override {
    std::cout << "ConcreteClass1 says: Implemented Operation2\n";
  }
};

class ConcreteClass2 : public AbstractClass {
 protected:
  void RequiredOperation1() const override {
    std::cout << "ConcreteClass2 says: Implemented Operation1\n";
  }
  void RequiredOperation2() const override {
    std::cout << "ConcreteClass2 says: Implemented Operation2\n";
  }
  void Hook1() const override {
    std::cout << "ConcreteClass2 says: Overriden Hook1\n";
  }
};

void ClientCode(AbstractClass *class_) { class_->TemplateMethod(); }

int main() {
  std::cout << "Same client code can work with different subclasses:\n";
  auto concrete_class1 = std::make_unique<ConcreteClass1>();
  ClientCode(concrete_class1.get());
  std::cout << "\n";
  std::cout << "Same client code can work with different subclasses:\n";
  auto concrete_class2 = std::make_unique<ConcreteClass2>();
  ClientCode(concrete_class2.get());
}
