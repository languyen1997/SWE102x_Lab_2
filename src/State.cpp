#include <iostream>
#include <memory>
#include <typeinfo>

class Context;

class State {
 protected:
  Context *context_;

 public:
  virtual ~State() = default;

  void SetContext(Context *context) { this->context_ = context; }

  virtual void Handle1() = 0;
  virtual void Handle2() = 0;
};

class Context {
  std::unique_ptr<State> state_ = nullptr;

 public:
  explicit Context(State *state) { this->TransitionTo(state); }

  void TransitionTo(State *state) {
    std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
    this->state_.reset(state);
    this->state_->SetContext(this);
  }

  void Request1() { this->state_->Handle1(); }
  void Request2() { this->state_->Handle2(); }
};

class ConcreteStateA : public State {
 public:
  void Handle1() override;

  void Handle2() override { std::cout << "ConcreteStateA handles request2.\n"; }
};

class ConcreteStateB : public State {
 public:
  void Handle1() override {
    std::cout << "ConcreteStateB handles request1.\n";
    std::cout << "ConcreteStateB wants to change the state of the context.\n";
    this->context_->TransitionTo(new ConcreteStateA);
  }

  void Handle2() override { std::cout << "ConcreteStateB handles request2.\n"; }
};

void ConcreteStateA::Handle1() {
  std::cout << "ConcreteStateA handles request1.\n";
  std::cout << "ConcreteStateA wants to change the state of the context.\n";
  this->context_->TransitionTo(new ConcreteStateB);
}

void ClientCode() {
  auto *context = new Context(new ConcreteStateA);
  context->Request1();
  context->Request2();
  delete context;
}

int main() { ClientCode(); }
