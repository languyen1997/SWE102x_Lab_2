#include <iostream>
#include <memory>
#include <string>

class BaseComponent;
class Mediator {
 public:
  virtual void Notify(BaseComponent *sender, std::string event) const = 0;
};

class BaseComponent {
 protected:
  Mediator *mediator_;

 public:
  explicit BaseComponent(Mediator *mediator = nullptr) : mediator_(mediator) {}

  void SetMediator(Mediator *mediator) { this->mediator_ = mediator; }
};

class Component1 : public BaseComponent {
 public:
  void DoA() {
    std::cout << "Component 1 does A.\n";
    this->mediator_->Notify(this, "A");
  }

  void DoB() {
    std::cout << "Component 1 does B.\n";
    this->mediator_->Notify(this, "B");
  }
};

class Component2 : public BaseComponent {
 public:
  void DoC() {
    std::cout << "Component 2 does C.\n";
    this->mediator_->Notify(this, "C");
  }

  void DoD() {
    std::cout << "Component 2 does D.\n";
    this->mediator_->Notify(this, "D");
  }
};

class ConcreteMediator : public Mediator {
 private:
  Component1 *component1_;
  Component2 *component2_;

 public:
  ConcreteMediator(Component1 *component1, Component2 *component2)
      : component1_(component1), component2_(component2) {
    this->component1_->SetMediator(this);
    this->component2_->SetMediator(this);
  }

  void Notify(BaseComponent *sender, std::string event) const override {
    (void)sender;
    if (event == "A") {
      std::cout << "Mediator reacts on A and triggers following operations:\n";
      this->component2_->DoC();
    }
    if (event == "D") {
      std::cout << "Mediator reacts on D and triggers following operations:\n";
      this->component1_->DoB();
      this->component2_->DoC();
    }
  }
};

void ClientCode() {
  auto comp1 = std::make_unique<Component1>();
  auto comp2 = std::make_unique<Component2>();
  auto mediator = std::make_unique<ConcreteMediator>(comp1.get(), comp2.get());
  std::cout << "Client trigger operation A.\n";
  comp1->DoA();
  std::cout << "\n";
  std::cout << "Client trigger operation D.\n";
  comp2->DoD();
}

int main() { ClientCode(); }
